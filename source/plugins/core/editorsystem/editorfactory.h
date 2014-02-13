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

#ifndef EDITORFACTORY_H
#define EDITORFACTORY_H

#include <QObject>

#include "core/editorsystem/editor.h"
#include "core/core_global.h"

namespace Core {

class Id;

class CORE_EXPORT EditorFactory : public QObject
{
    Q_OBJECT
public:
    explicit EditorFactory(QObject *parent = 0);
    virtual ~EditorFactory() {}

    virtual Editor * createEditor(QWidget *parent) = 0;
    virtual Id id() const = 0;

}; // end of class Core::EditorFactory

} // end of namespace Core

#endif // EDITORFACTORY_H
