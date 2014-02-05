/*-------------------------------------------------
 *
 * OrbitsWriter - an Offline Blog Writer
 *
 * Copyright (C) 2013 devbean@galaxyworld.org
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

#ifndef HTMLEDITWIDGET_H
#define HTMLEDITWIDGET_H

#include <QWidget>

namespace HtmlEdit {

namespace Internal {
class HtmlEditWidgetPrivate;
} // end of namespace HtmlEdit::Internal

class HtmlSourceEdit;
class HtmlVisualEdit;

class HtmlEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HtmlEditWidget(QWidget *parent = 0);
    ~HtmlEditWidget();

    HtmlSourceEdit * sourceEdit() const;
    HtmlVisualEdit * visualEdit() const;

private:
    Internal::HtmlEditWidgetPrivate *d;
    friend class Internal::HtmlEditWidgetPrivate;
};

} // end of namespace HtmlEdit

#endif // HTMLEDITWIDGET_H
