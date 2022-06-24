/*
 * This application is free software and is released under the terms of
 * the BSD license. See LICENSE file for details.
 *
 * Copyright (c) 2010 Volker Poplawski (volker@openbios.org)
 */
#include "dataItemWidget.h"

#include "uap.h"
#include "global.h"


extern Uap* g_uap;

DataItemWidget::DataItemWidget(QWidget *parent) :
    QWidget(parent)
{
  QVBoxLayout* vbox = new QVBoxLayout;
  setLayout(vbox);

  QFile cssFile(":/dataItem.css");
  if (not cssFile.open(QIODevice::ReadOnly))
    qFatal("CSS not found.");

  m_css = cssFile.readAll();

  m_webView = new QWebView(0);
  m_webView->settings()->setFontFamily(QWebSettings::StandardFont, QFont().family());
  m_webView->settings()->setFontSize(QWebSettings::DefaultFontSize, QFont().pointSize() * 1.4);
  m_webView->setMinimumSize(100, 88);
  m_webView->resize(100, 88);
  layout()->addWidget(m_webView);
}


void DataItemWidget::setDataItem(const AsterixBlock* block, const AsterixRecord* record, const AsterixDataItem *dataItem)
{
  if (block == 0)
    return;

  if (g_uap->selectedUapCategory(block->category()) == 0)
    return;  // unknown cat

  const UapDataItem& uapDataItem = *dataItem->uapDataItem();
  const int ofs = uapDataItem.format() == UapDataItem::REPETIVE ? 1 : 0;

  QString html;

  html += "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en_US\" lang=\"en_US\">";

  html += "<head>";
  html += "<style type=\"text/css\"><!--";
  html += m_css;
  html += "--></style></head>";


  html += "<body>";
  html += "<h3>" + uapDataItem.id() + "&nbsp;&nbsp;" + uapDataItem.name() + "</h3>";
  if (not uapDataItem.definition().isEmpty())
      html += "<p>Definition: " + uapDataItem.definition() + "</p>";

  html += "<p><table border=\"0\" style=\"margin-left:26px\">";

  if (uapDataItem.format() == UapDataItem::REPETIVE)
  {
    const int numreps = ((const uchar*)dataItem->data())[0];    // repetition factor is in first byte of Data Item
    if (numreps * uapDataItem.length() > dataItem->length())
    {
      logWarn("To many repetitions in REPETIVE Data Item");
    }
    else
    {
      for (int repetiveIndex = 0; repetiveIndex < numreps; ++repetiveIndex)
      {
        html += "<tr><td style=\"border-top:10px solid transparent;\" colspan=\"3\"><h4>Repetition no. " + QString::number(repetiveIndex+1) + " of " + QString::number(numreps) + "</h4><td></tr>";
        html += printFields(dataItem, repetiveIndex);
      }
    }
  }
  else
  {
    html += printFields(dataItem);
  }

  html += "</table></p>";

  if (not uapDataItem.description().isEmpty())
      html += "<p>" + uapDataItem.description() + "</p>";

  html += "</body></html>";

  m_webView->setHtml(html);
}


QString DataItemWidget::printFields(const AsterixDataItem* dataItem, int repetiveIndex)
{
  QString html;

  const UapDataItem& uapDataItem = *dataItem->uapDataItem();

  const int len = uapDataItem.length();

  foreach (const UapField& uapField, uapDataItem.m_fields)
  {
      // in fields of VARIABLE format not all fields are necessesary present
      if (not uapDataItem.fieldPresent(dataItem->data(), uapField))
          continue;

      html += "<tr><td style=\"border-top:2px solid transparent;\" colspan=\"3\">" + printItemBitTable(*dataItem, uapField, repetiveIndex) + "</td></tr>";

      html += "<tr>";
      html += "<td style=\"vertical-align:top;\">" + (uapField.name().isEmpty() ? "<i>nn</i>" : uapField.name()) + "</td>";
      html += "<td style=\"vertical-align:top; padding-left:6px; padding-right:6px\"><u>";

      QByteArray field = dataItem->bitfield(uapField.octed() + repetiveIndex*len, len, uapField.msb()-1, uapField.lsb()-1);
      int value = 0;

      switch (uapField.format())
      {
          case UapField::ICAO6STR:
          {
              QString s = AsterixDataItem::decodeIcaoStr((const uchar*)field.constData());
              html += "\"" + s.replace(' ', "&nbsp;") + "\"";
          }
          break;

          case UapField::UINTEGER:
          {
              value = bitfieldToUInt(field);
              html += QString("%1").arg(value) + "</u>";
              html += "&nbsp;&nbsp;(<u>" + applyUnitAndScale(uapField, value) + "</u>)<u>";
          }
          break;

          case UapField::INTEGER:
          {
              value = bitfieldToInt(field, uapField.msb()-1 - (((uapField.lsb()-1)/8)*8) );
              html += QString("%1").arg(value) + "</u>";
              html += "&nbsp;&nbsp;(<u>" + applyUnitAndScale(uapField, value) + "</u>)<u>";
          }
          break;

          case UapField::OCTAL:
          {
              value = bitfieldToUInt(field);
              html += QString("0%1").arg(value, 0, 8);
          }
          break;

          case UapField::ASCII:
          {
              QString s = QString::fromLatin1(field);
              s.replace(" ", "&nbsp;");
              html += "\"" + s + "\"";
          }
          break;

          case UapField::HEX:
          default:
          {
              value = bitfieldToUInt(field);
              html += "0x";
              for (int i=0; i < field.size(); i++)
                  html += QString("%1").arg((uint)((uchar)field[i]), 2, 16, QChar('0'));
          }
          break;
      }

      // line += dataItem->decode(uapField.m_octed, uapField.msb(), uapField.lsb());
      html += "</u></td>";

      html += "<td style=\"vertical-align:top; padding:0;\">";

      if (not uapField.desc().isEmpty())
          html += uapField.desc();

      html += printEnumeration(uapField, value);

      html += "</td></tr>";
  }

  return html;
}



QString DataItemWidget::printEnumeration(const UapField& uapField, int value)
{
  QString html = "<table border=\"0\">";
  for (QMap<int, UapEnum>::const_iterator it = uapField.enums().begin(); it != uapField.enums().end(); ++it)
  {
    html += "<tr><td>=&nbsp;" + QString::number(it.key()) + "</td><td>";

    if (it.key() == value)
      html+= "<u>";

    html += it.value().m_desc;

    if (it.key() == value)
      html+= "</u>";

    html += "</td></tr>";
  }
  html += "</table>";
  return html;
}


QString DataItemWidget::applyUnitAndScale(const UapField& uapField, int value)
{
  QString html;

  if (uapField.unit() != UapField::UNDEFINED)
  {
    html += QString::number((double)value * uapField.scale()) + "&nbsp;" + uapField.unitStr(uapField.unit());
  }

  return html;
}




QString DataItemWidget::printItemBitTable(const AsterixDataItem &dataItem, const UapField& uapField, int repetiveIndex) const
{
  const UapDataItem& uapDataItem = *dataItem.uapDataItem();
  const uchar* data = dataItem.data();

  int len;
  if (uapDataItem.format() == UapDataItem::VARIABLE)
  {
    // special case for VARIABLE items
    // every occuring field has length as specified on the DataItem
    len = uapDataItem.length();
  }
  else if (uapDataItem.format() == UapDataItem::REPETIVE)
  {
    // special case for VARIABLE items
    // every occuring field has length as specified on the DataItem
    len = uapDataItem.length();
    // adjust position in data on the repetiveIndex requested
    data += repetiveIndex * len;
  }
  else
  {
    len = dataItem.length() - (uapField.octed()-1);
  }

  const int startbit = len*8 - uapField.msb();    // counting from left and from 0
  const int endbit = len*8 - uapField.lsb();      // counting from left and from 0

  QString s = renderBitTable(data + uapField.octed()-1, len, startbit, endbit);

  return s;
}


/**
 * @brief DataItemWidget::renderBitTable
 * @param data      pointer to first byte
 * @param numbytes  number of bytes to render
 * @param startbit  start of marker. Counting from left and starting at zero
 * @param endbit    end of marker (inklusive). Counting from left and starting at zero
 * @param skipUnusedBytes dont render bytes outside marker
 * @return html string
 */
QString DataItemWidget::renderBitTable(const uchar* data, int numbytes, int startbit, int endbit, bool skipUnusedBytes)
{
  QString s;

  const QString title = QString("Bits-%1/%2").arg(numbytes*8-startbit).arg(numbytes*8-endbit);
  s += "<table border=\"0\" cellspacing=\"0\" class=\"bittable\" title=\"" + title + "\">";

  const int startbyteidx = startbit / 8;
  const int endbyteidx   = endbit / 8;

  //
  // first row: hex values
  //

  s += "<tr>";
  for (int byteidx = 0; byteidx < numbytes; byteidx++)
  {
    if (skipUnusedBytes && (byteidx < startbyteidx || byteidx > endbyteidx))
    {
      continue;
    }

    QString byteHex = QString("%1").arg((uint)data[byteidx], 2, 16, QChar('0')).toUpper();
    s += "<td colspan=\"4\" align=\"center\" bgcolor=\"" + g_highlightColor.name() + "\">" + byteHex[0] + "</td>";
    s += "<td colspan=\"4\" align=\"center\" bgcolor=\"" + g_highlightColor.name() + "\">" + byteHex[1] + "</td>";
    if (byteidx < numbytes - 1)  // not at last byte
    {
      s += "<td style=\"padding-left:4px;\"></td>";
    }
  }
  s += "</tr>";

  //
  // second row: bit values with marker
  //

  s += "<tr>";
  for (int byteidx = 0; byteidx < numbytes; byteidx++)
  {
    if (skipUnusedBytes && (byteidx < startbyteidx || byteidx > endbyteidx))
    {
      continue;
    }

    // convert byte value to string of eight '1'/'0'
    QString byteBin = QString("%1").arg((uint)data[byteidx], 8, 2, QChar('0'));

    bool insideMarker = false;
    for (int b = 0; b < 8; b++)
    {
      const int bitpos = byteidx*8 + b;
      if (bitpos == startbit && bitpos == endbit)
      {
        // mark single bit
        s += "<td class=\"both\">" + byteBin[b] + "</td>";
      }
      else if (bitpos == startbit && bitpos < endbit)
      {
        // begin marking of bits
        s += "<td class=\"left\">" + byteBin[b] + "</td>";
        insideMarker = true;
      }
      else if (bitpos > startbit && bitpos < endbit)
      {
        // continue marking of bits
        s += "<td class=\"mid\">" + byteBin[b] + "</td>";
      }
      else if (bitpos > startbit && bitpos == endbit)
      {
        // end marking of bits
        s += "<td class=\"right\">" + byteBin[b] + "</td>";
        insideMarker = false;
      }
      else
      {
        // no marking
        s += "<td class=\"node\">" + byteBin[b] + "</td>";
      }
    }

    if (byteidx < numbytes - 1)  // not at last byte
    {
      if (insideMarker)
        // continue marker to next byte
        s += "<td class=\"mid\"></td>";
      else
        s += "<td style=\"padding-bottom:0;\" ></td>";
    }
  }

  s += "</tr>";
  s += "</table>";

  return s;
}





