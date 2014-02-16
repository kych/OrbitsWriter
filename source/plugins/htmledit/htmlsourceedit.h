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

#ifndef HTMLSOURCEEDIT_H
#define HTMLSOURCEEDIT_H

#include <QPlainTextEdit>

#include "htmledit_export.h"

namespace HtmlEdit {

class HTMLEDIT_EXPORT HtmlSourceEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit HtmlSourceEdit(QWidget *parent = 0);

    void setHtmlSource(const QString &source);
    QString htmlSource() const;
}; // end of class HtmlEdit::HtmlSourceEdit

} // end of namespace HtmlEdit

#endif // HTMLSOURCEEDIT_H
