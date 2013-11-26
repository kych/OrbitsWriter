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

#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>

#include "DPointer"

namespace GOW
{

class ColorButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(bool colorDialog READ colorDialogEnabled WRITE setColorDialogEnabled)
public:
    ColorButton(QWidget *parent = 0, int columns = -1, bool enableColorDialog = true);
    ~ColorButton();

    void insertColor(const QColor &color, const QString &text = QString::null, int index = -1);

    QColor currentColor() const;

    QColor color(int index) const;

    void setColorDialogEnabled(bool enabled);
    bool colorDialogEnabled() const;

    void setStandardColors();

    static QColor getColor(const QPoint &point, bool allowCustomColors = true);

    void setTipIcon(const QIcon &icon);

public slots:
    void setCurrentColor(const QColor &color);

signals:
    void colorChanged(const QColor &);

protected:
    void paintEvent(QPaintEvent *e);

private slots:
    void buttonPressed(bool toggled);
    void popupClosed();

private:
    D_POINTER

}; // end of class GOW::ColorButton

} // end of namespace GOW

#endif // COLORBUTTON_H
