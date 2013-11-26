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

#ifndef FONTSIZECHOOSER_H
#define FONTSIZECHOOSER_H

#include <QComboBox>
#include <QStyledItemDelegate>

#include "DPointer"

namespace Orbits
{

class FontSizeChooser : public QComboBox
{
    Q_OBJECT
public:
    explicit FontSizeChooser(QWidget *parent = 0);
    ~FontSizeChooser();

signals:
    void fontSizeActivated(int size);
    
protected:
    void showPopup();

private:
    D_POINTER
}; // end of class Orbits::FontSizeChooser

} // end of namesapce Orbits

#endif // FONTSIZECHOOSER_H
