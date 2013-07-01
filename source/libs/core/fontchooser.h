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

#ifndef FONTCHOOSER_H
#define FONTCHOOSER_H

#include <QComboBox>
#include <QStyledItemDelegate>

#include <DPointer>

namespace GOW
{

class FontChooserItemDelegate : public QStyledItemDelegate
{
public:
    FontChooserItemDelegate(QObject *parent = 0);

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &,
                   const QModelIndex &index) const;
}; // end of class GOW::FontChooserItemDelegate

class FontChooser : public QComboBox
{
    Q_OBJECT
public:
    explicit FontChooser(QWidget *parent = 0);
    
signals:
    void fontFamilyChanged(const QString &family);

protected:
    void showPopup();

private:
    D_POINTER
}; // end of class GOW::FontChooser

} // end of namespace GOW

#endif // FONTCHOOSER_H
