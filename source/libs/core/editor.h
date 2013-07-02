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

#ifndef EDITOR_H
#define EDITOR_H

QT_FORWARD_DECLARE_CLASS(QString);

namespace GOW
{

enum TextAlignment
{
    AlignJustify,
    AlignLeft,
    AlignRight,
    AlignCenter
};

class Editor
{
public:
    virtual void textBold(bool bold) = 0;
    virtual void textItalic(bool italic) = 0;
    virtual void textUnderline(bool underline) = 0;
    virtual void textStrikeOut(bool strike) = 0;

    virtual void textAlign(TextAlignment alignment) = 0;
    virtual void textFontFamily(const QString &family) = 0;
    virtual void textFontSize(int size) = 0;
}; // end of class GOW::Editor
} // end of namespace GOW

#endif // EDITOR_H
