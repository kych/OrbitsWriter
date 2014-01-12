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

#ifndef CORELISTENER_H
#define CORELISTENER_H

#include <QObject>

namespace Core {

class Editor;

class CoreListener : public QObject
{
    Q_OBJECT
public:
    explicit CoreListener(QObject *parent = 0) : QObject(parent) {}
    virtual ~CoreListener() {}

    virtual bool editorAboutToClose(Editor * /*editor*/) { return true; }
    virtual bool mainWindowAboutToClose() { return true; }
}; // end of class Core::CoreListener

} // end of namespace Core

#endif // CORELISTENER_H
