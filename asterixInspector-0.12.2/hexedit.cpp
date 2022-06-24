/*
 * This application is free software and is released under the terms of
 * the BSD license. See LICENSE file for details.
 *
 * Copyright (c) 2010 Volker Poplawski (volker@openbios.org)
 */
#include "hexedit.h"

#include "global.h"

const int H_MARGIN = 4;

HexEdit::HexEdit(QWidget *parent)
 : QAbstractScrollArea(parent),
   m_data(0),
   m_offsetColumnMode( SIZE32 )
{
  setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  setFocusPolicy(Qt::StrongFocus);  // accept mouse and tab focus
  QFont myfont( "monospace" );
  myfont.setStyleHint(QFont::TypeWriter);
  myfont.setKerning(false);
  setFont( myfont );

  setMouseTracking(true);

  switch (m_offsetColumnMode)
  {
    case SIZE32:
      m_sizeHint = fontMetrics().boundingRect( " 01234567: 00000000 00000000 00000000 00000000    0123456789abcdef " ).size();
      break;

    case SIZE64:
      m_sizeHint = fontMetrics().boundingRect( " 0123456789abcdef: 00000000 00000000 00000000 00000000    0123456789abcdef " ).size();
      break;

    default:
      m_sizeHint = fontMetrics().boundingRect( " 00000000 00000000 00000000 00000000    0123456789abcdef " ).size();
      break;
  }
  m_cursorSize = fontMetrics().boundingRect('0').size() + QSize(2, 3);

  m_sizeHint.rwidth() += (2 * H_MARGIN) + (2*frameWidth()) + verticalScrollBar()->sizeHint().width();

  m_sizeHint.rheight() = (fontMetrics().lineSpacing() * 16) + (2*frameWidth());  // extend to 16 lines
  setMinimumHeight( fontMetrics().lineSpacing() * 4 + (2*frameWidth()) );
  setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Expanding );
  setBaseSize( m_sizeHint );
  setSizeIncrement( 0, fontMetrics().lineSpacing() );

  m_cursorPosition = 0;
}


HexEdit::~HexEdit()
{
}


void HexEdit::setData(const uchar * data, quint64 size)
{
  m_data = data;
  m_dataSize = size;
  m_lineCount = m_dataSize / 16;
  if (m_dataSize % 16 != 0)
    m_lineCount++;

  verticalScrollBar()->setValue( 0 );
  updateScrollbar();

  setHighlight( 0, 0 );

  m_cursorPosition = 0;
  emit cursorPosition( m_cursorPosition );
}


void HexEdit::paintEvent(QPaintEvent * event)
{
  if (m_data == 0)
  {
    return;
  }

  QPainter painter( viewport() );

  int cw = fontMetrics().width( '0' );
  int ch = fontMetrics().height();
  int hexX = H_MARGIN;
  if (m_offsetColumnMode == SIZE32)
    hexX = H_MARGIN + (8 + 2) * cw;
  else if (m_offsetColumnMode == SIZE64)
    hexX = H_MARGIN + (16 + 2) * cw;
  int asciiX = hexX + H_MARGIN + 16*2*cw + 6*cw;

  /*
   * draw fileoffset background rect
   */
  painter.save();
  painter.setBrush( QBrush( QColor( 230, 230, 230 ) ) );
  painter.setPen( Qt::NoPen );

  painter.drawRect( 0, event->rect().top(), hexX-cw, event->rect().bottom() );

  painter.restore();

  int linesVisible = viewport()->height() / fontMetrics().lineSpacing();
  int firstToBeUpdatedLine = qMax(event->rect().top() / fontMetrics().lineSpacing() - 1, 0);
  int lastToBeUpdatedLine = qMax(event->rect().bottom() / fontMetrics().lineSpacing() + 1, linesVisible);

  const uchar* firstLineAddress = m_data + (verticalScrollBar()->value() * 16);  // address  of first visible line

  // handle line by line
  for ( int i = firstToBeUpdatedLine; i < lastToBeUpdatedLine; i++ )
  {
    const uchar* lineAddress = firstLineAddress + i*16;   // address of first byte in line

    /*
     * build pos string
     */
    QString posStr = QString("%1").arg(quint64(verticalScrollBar()->value() * 16 + i*16), 16, 16, QChar('0'));
    if (m_offsetColumnMode == SIZE32)
    {
      posStr = posStr.right(8);
      posStr += ": ";
    } else if (m_offsetColumnMode == SIZE64)
    {
      posStr += ": ";
    } else if (m_offsetColumnMode == NONE)
    {
      posStr = "";
    }


    int bytesInLine = 16;    // a complete line has 16 bytes, the very last line has [1..16]
    if (verticalScrollBar()->value() + i + 1 == m_lineCount)
    {
      // this is the last line
      bytesInLine = m_dataSize % 16;
      if (bytesInLine == 0)
        bytesInLine = 16;
    }
    else if (verticalScrollBar()->value() + i + 1 > m_lineCount)
    {
      bytesInLine = 0;
    }

    /*
     * build hex and ascii columns
     */
    QString line;
    QString asciiColumn;
    for ( int o = 0; o < 16; o++ )
    {
      if (o < bytesInLine)
      {
        quint8 b = *(lineAddress + o);
        QString bs = QString("%1").arg((b), 2, 16, QChar('0'));
        line += bs;

        QChar c( b );
        if (c.isPrint())
          asciiColumn += b;
        else
          asciiColumn += QChar(0x00b7); // QChar(QChar::ReplacementCharacter);

      }
      else
      {
        line += "  ";
        asciiColumn += " ";
      }

      if (o % 4 == 3)
        line += " ";

    }

    int textBaseLineY = ((i+1) * fontMetrics().lineSpacing()) - fontMetrics().leading() - 3;

    /*
     * if this is a highlighted line, draw background
     */
    QRect highlightHex(-1, textBaseLineY+3, 0, -ch);
    QRect highlightAscii(-1, textBaseLineY+3, 0, -ch);
    if (m_highlightStart <= lineAddress      // below or exactly at first line byte
        &&
        m_highlightStart + m_highlightSize > lineAddress) // end of highlight in or behind line
    {
      highlightHex.setLeft( hexX );
      highlightAscii.setLeft( asciiX );
    }
    else if (m_highlightStart > lineAddress
             &&
             m_highlightStart < lineAddress + 16) // somewhere in line
    {
      int hightligtLeftOffset = m_highlightStart - lineAddress;
      highlightHex.setLeft( hexX + hightligtLeftOffset*cw*2 + (hightligtLeftOffset/4)*cw );
      highlightAscii.setLeft( asciiX + hightligtLeftOffset*cw );
    }

    if (m_highlightStart + m_highlightSize > lineAddress
        &&
        m_highlightStart + m_highlightSize < lineAddress + 16)
    {
      // end of highlight region within line
      int highlightRightOffset = m_highlightStart + m_highlightSize - lineAddress;
      highlightHex.setRight( hexX + highlightRightOffset * cw * 2  + ((highlightRightOffset-1)/4)*cw );
      highlightAscii.setRight( asciiX + highlightRightOffset * cw );
    }
    else
    {
      // end of highlight exactly at last byte or behind line
      highlightHex.setRight( hexX + 16 * cw * 2 + 3*cw );
      highlightAscii.setRight( asciiX + 16 * cw );
    }

    if (m_highlightStart != 0 && highlightHex.left() > 0)
    {
      painter.save();
      painter.setPen( Qt::NoPen );
      painter.setBrush( g_highlightColor );
      painter.drawRect( highlightHex );
      painter.drawRect( highlightAscii );
      painter.restore();
    }

    /*
     * if the cursor positioned in this line, draw it
     */
    if (m_cursorPosition / 16 == verticalScrollBar()->value() + i)
    {
      int cursorLineOffset = m_cursorPosition % 16;
      int hexCursorPos = (cursorLineOffset * 2 * cw) + ((cursorLineOffset / 4) * cw);
      int asciiCursorPos = (cursorLineOffset *  cw);
      painter.save();
      painter.setPen( Qt::gray );
      painter.drawRect( hexX+ hexCursorPos -1, textBaseLineY - m_cursorSize.height()+1, m_cursorSize.width()*2, m_cursorSize.height() );  // hex cursor
      painter.drawRect( asciiX + asciiCursorPos -1, textBaseLineY - m_cursorSize.height()+1, m_cursorSize.width(), m_cursorSize.height() ); // ascii cursor
      painter.restore();
    }


    painter.drawText( H_MARGIN, textBaseLineY, posStr + line.toUpper() ); // draw line address column + hex column

    painter.drawText( asciiX, textBaseLineY, asciiColumn );
  }
}


QSize HexEdit::sizeHint() const
{
  return m_sizeHint;
}

void HexEdit::resizeEvent(QResizeEvent * event)
{
  QAbstractScrollArea::resizeEvent( event );
  updateScrollbar();
}


/* configures vscrollbar so that its value reflects the
 * first to be displayed line
 *
 */
void HexEdit::updateScrollbar()
{
  verticalScrollBar()->setRange( 0, m_lineCount - visibleLines() );
  viewport()->update();
}


void HexEdit::scrollContentsBy(int dx, int dy)
{
  int pixeldy = dy * fontMetrics().lineSpacing();;
  if (qAbs(pixeldy) < viewport()->height())
  {
    viewport()->scroll( 0, pixeldy );
  }
  else
  {
    viewport()->update();
  }
}


void HexEdit::keyPressEvent(QKeyEvent * event)
{
  switch (event->key())
  {
    case Qt::Key_Left:
      if (m_cursorPosition > 0)
      {
        m_cursorPosition--;
        ensureCursorVisible();
        emit cursorPosition( m_cursorPosition );
        viewport()->update();
      }
      break;

    case Qt::Key_Right:
      if (m_cursorPosition < m_dataSize)
      {
        m_cursorPosition++;
        ensureCursorVisible();
        emit cursorPosition( m_cursorPosition );
        viewport()->update();
      }
      break;

    case Qt::Key_Down:
      if (m_cursorPosition < m_dataSize - 16)
      {
        m_cursorPosition += 16;
        ensureCursorVisible();
        emit cursorPosition( m_cursorPosition );
        viewport()->update();
      }
      break;

    case Qt::Key_Up:
      if (m_cursorPosition >= 16)
      {
        m_cursorPosition -= 16;
        ensureCursorVisible();
        emit cursorPosition( m_cursorPosition );
        viewport()->update();
      }
      break;

    case Qt::Key_PageDown:
      if (m_cursorPosition / 16 + visibleLines() < m_dataSize / 16)
      {
        m_cursorPosition += visibleLines() * 16;
        verticalScrollBar()->setValue(m_cursorPosition / 16);
        emit cursorPosition( m_cursorPosition );
      }
      break;

    case Qt::Key_PageUp:
      if (m_cursorPosition / 16 - visibleLines() >= 0)
      {
        m_cursorPosition -= visibleLines() * 16;
        ensureCursorVisible();
        emit cursorPosition( m_cursorPosition );
        viewport()->update();
      }
      break;

    default:
      QAbstractScrollArea::keyPressEvent( event );
      break;
  }
}

void HexEdit::ensureCursorVisible()
{
  if (m_cursorPosition / 16 < verticalScrollBar()->value())
  {
    // cursor above viewport
    verticalScrollBar()->setValue( m_cursorPosition / 16 );
  }
  else if ((m_cursorPosition / 16) + 1 > verticalScrollBar()->value() + visibleLines())
  {
    // cursor below viewport
    verticalScrollBar()->setValue( m_cursorPosition / 16 - visibleLines() + 1);
  }
}

/* number of lines visible
 * based on widget height and data size
 */
int HexEdit::visibleLines() const
{
  return viewport()->height() / fontMetrics().lineSpacing() + 1;
}


void HexEdit::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
    setCursorPosition(m_data + mousePosToOffset(event->pos()));
}


void HexEdit::mouseMoveEvent(QMouseEvent *event)
{
}


void HexEdit::mouseReleaseEvent(QMouseEvent * event)
{

}


void HexEdit::contextMenuEvent(QContextMenuEvent *e)
{
  QMenu menu;
  QAction* a = menu.addAction("Rescan from offset 0x" + QString::number(m_cursorPosition, 16));
  if (menu.exec(e->globalPos()) == a)
  {
    emit rescanRequested(m_cursorPosition);
  }
}


void HexEdit::setHighlight(const uchar * start, qint64 size, bool ensureVisible)
{
  m_highlightStart = start;
  m_highlightSize = size;

  if (ensureVisible)
    ensureHighlightVisible();

  viewport()->update();
}


void HexEdit::setCursorPosition(const uchar *pos)
{
  if (m_cursorPosition != pos - m_data)
  {
    m_cursorPosition = pos - m_data;
    viewport()->update();
    emit cursorPosition( m_cursorPosition );
  }
}


void HexEdit::ensureHighlightVisible()
{
  if (m_highlightStart == 0)
    return;

  int highlightStartLine = (m_highlightStart - m_data) / 16;
  if (highlightStartLine < verticalScrollBar()->value()
      ||
      highlightStartLine >= (verticalScrollBar()->value() + visibleLines()))
  {
    verticalScrollBar()->setValue( (m_highlightStart - m_data) / 16 );
  }
}


qint64 HexEdit::mousePosToOffset(const QPoint &pos) const
{
  qint64 firstLineOfs = (verticalScrollBar()->value() * 16);  // offset  of first visible line
  int cw = fontMetrics().width( '0' );

  int yofs = 16 * ((pos.y()-1) / fontMetrics().lineSpacing());

  int posMargin;  // size of posStr column in px

  if (m_offsetColumnMode == SIZE32)
  {
    posMargin = H_MARGIN + cw * 10;
  } else if (m_offsetColumnMode == SIZE64)
  {
    posMargin = H_MARGIN + cw * 18;
  } else if (m_offsetColumnMode == NONE)
  {
    posMargin = H_MARGIN;
  }

  int asciiX = posMargin - H_MARGIN + (4*8 + 3*1 + 4)*cw;

  int xofs;
  if (pos.x() < asciiX)
    xofs = (pos.x() - posMargin - ((pos.x() - posMargin) / (cw*9))*cw) / cw / 2;
  else
    xofs = (pos.x() - asciiX) / cw;

  return qBound((qint64)0, firstLineOfs + yofs + qBound(0, xofs, 15), m_dataSize);
}

