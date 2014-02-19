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

#ifndef DOCUMENTPARAGRAPHSTYLE_H
#define DOCUMENTPARAGRAPHSTYLE_H

#include <QMetaType>

namespace Core {

class DocumentParagraphStyle
{
public:
    DocumentParagraphStyle();
}; // end of class Core::DocumentParagraphStyle

} // end of namespace Core

Q_DECLARE_METATYPE(Core::DocumentParagraphStyle)

#endif // DOCUMENTPARAGRAPHSTYLE_H
