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

class VisualEditor::Private : public QObject
{
    Q_OBJECT
public:
    Private(VisualEditor *q_ptr) : QObject(q_ptr), q(q_ptr) {}

    void mergeFormatOnWordOrSelection(const QTextCharFormat &format)
    {
        QTextCursor cursor = q->textCursor();
        if (!cursor.hasSelection()) {
            cursor.select(QTextCursor::WordUnderCursor);
        }
        cursor.mergeCharFormat(format);
        q->mergeCurrentCharFormat(format);
    }

private:
    Q_POINTER(VisualEditor)
}; // end of class GOW::VisualEditor::Private


VisualEditor::VisualEditor(QWidget *parent) :
    QTextEdit(parent), d(this)
{

}

void VisualEditor::textBold(bool bold)
{
    QTextCharFormat fmt;
    fmt.setFontWeight(bold ? QFont::Bold : QFont::Normal);
    d->mergeFormatOnWordOrSelection(fmt);
}

void VisualEditor::textItalic(bool italic)
{
    QTextCharFormat fmt;
    fmt.setFontItalic(italic);
    d->mergeFormatOnWordOrSelection(fmt);
}

void VisualEditor::textUnderline(bool underline)
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(underline);
    d->mergeFormatOnWordOrSelection(fmt);
}

void VisualEditor::textStrikeOut(bool strike)
{
    QTextCharFormat fmt;
    fmt.setFontStrikeOut(strike);
    d->mergeFormatOnWordOrSelection(fmt);
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

void VisualEditor::textFontFamily(const QString &family)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(family);
    d->mergeFormatOnWordOrSelection(fmt);
}

void VisualEditor::textFontSize(int size)
{
    if (size > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(size);
        d->mergeFormatOnWordOrSelection(fmt);
    }
}

}

#include "visualeditor.moc"
