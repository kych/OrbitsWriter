/*-------------------------------------------------
 *
 * OrbitsWriter - An Offline Blog Writer
 *
 * Copyright (C) 2012 devbean@galaxyworld.org
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *-------------------------------------------------*/

#include "visualeditor.h"

namespace GOW
{

VisualEditor::VisualEditor(QWidget *parent) :
    QTextEdit(parent)
{
}

void VisualEditor::textBold(bool bold)
{
    QTextCharFormat fmt;
    fmt.setFontWeight(bold ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void VisualEditor::textItalic(bool italic)
{
    QTextCharFormat fmt;
    fmt.setFontItalic(italic);
    mergeFormatOnWordOrSelection(fmt);
}

void VisualEditor::textUnderline(bool underline)
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(underline);
    mergeFormatOnWordOrSelection(fmt);
}

void VisualEditor::textStrikeOut(bool strike)
{
    QTextCharFormat fmt;
    fmt.setFontStrikeOut(strike);
    mergeFormatOnWordOrSelection(fmt);
}

void VisualEditor::textAlign(TextAlignment alignment)
{
    switch (alignment) {
    case AlignCenter:
        setAlignment(Qt::AlignHCenter);
        break;
    case AlignJustify:
        setAlignment(Qt::AlignJustify);
        break;
    case AlignLeft:
        setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
        break;
    case AlignRight:
        setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
        break;
    }
}

void VisualEditor::textFont(const QString &family)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(family);
    mergeFormatOnWordOrSelection(fmt);
}

void VisualEditor::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}

}
