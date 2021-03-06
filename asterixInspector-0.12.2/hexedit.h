/*
 * This application is free software and is released under the terms of
 * the BSD license. See LICENSE file for details.
 *
 * Copyright (c) 2010 Volker Poplawski (volker@openbios.org)
 */
#ifndef HEXEDIT_H
#define HEXEDIT_H

#include <QAbstractScrollArea>

#include <QtGui>

class HexEdit : public QAbstractScrollArea
{
Q_OBJECT
public:
    HexEdit(QWidget *parent = 0);
    ~HexEdit();

    enum OffsetColumn { NONE, SIZE32, SIZE64 };

    void setData( const uchar* data, quint64 size );
    qint64 cursorPosition() const { return m_cursorPosition; }

    int visibleLines() const;
    virtual QSize sizeHint () const;


  public Q_SLOTS:
    void setHighlight( const uchar* start, qint64 size, bool ensureVisible = false );
    void ensureHighlightVisible();
    void setCursorPosition( const uchar* pos );

  Q_SIGNALS:
    void cursorPosition(qint64 pos);
    void rescanRequested(qint64 ofs);

  protected:
    virtual void paintEvent( QPaintEvent * event );
    virtual void resizeEvent( QResizeEvent * event );
    virtual void scrollContentsBy ( int dx, int dy );
    virtual void keyPressEvent( QKeyEvent* event );
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void contextMenuEvent(QContextMenuEvent *);

    void updateScrollbar();
    void ensureCursorVisible();
    qint64 mousePosToOffset( const QPoint& pos ) const;

    const uchar*  m_data;
    qint64        m_dataSize;

    qint64     m_cursorPosition;
    QSize      m_cursorSize;     // size of cursor rect, calculated from font size

    const uchar*  m_highlightStart;  // byte offset
    qint64        m_highlightSize;   // bytes

    int           m_lineCount;
    OffsetColumn  m_offsetColumnMode;
    QSize         m_sizeHint;

};

#endif
