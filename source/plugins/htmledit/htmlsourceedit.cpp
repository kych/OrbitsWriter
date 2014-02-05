/*-------------------------------------------------
 *
 * OrbitsWriter - an Offline Blog Writer
 *
 * Copyright (C) 2014 devbean@galaxyworld.org
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

#include "htmlsourceedit.h"

namespace HtmlEdit
{

HtmlSourceEdit::HtmlSourceEdit(QWidget *parent) :
    QPlainTextEdit(parent)
{
    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    font.setPointSizeF(11);
    setFont(font);
}

void HtmlSourceEdit::setHtmlSource(const QString &source)
{
    setPlainText(source);
}

QString HtmlSourceEdit::htmlSource() const
{
    return toPlainText();
}

} // end of namespace HtmlEdit
