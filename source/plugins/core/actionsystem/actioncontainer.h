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

#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

#include <QObject>

#include "core/id.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMenuBar;
class QToolBar;
QT_END_NAMESPACE

namespace Core
{

class Command;
class Context;

class ActionContainer : public QObject
{
    Q_OBJECT
public:
    virtual Id id() const = 0;

    virtual QMenu *menu() const = 0;
    virtual QMenuBar *menuBar() const = 0;
    virtual QToolBar *toolBar() const = 0;

    virtual void appendActionGroup(const Id &group) = 0;
    virtual void insertActionGroup(const Id &before, const Id &group) = 0;

    virtual QAction * actionAt(const Id &group) const = 0;

    virtual void addAction(Command *cmd, const Id &groupId = Id()) = 0;
    virtual void addMenu(ActionContainer *menu, const Id &group = Id()) = 0;
    virtual void addMenu(ActionContainer *before, ActionContainer *menu,
                         const Id &group = Id()) = 0;

    virtual Command * addSeparator(const Context &context,
                                   const Id &group = Id(),
                                   QAction **outSeparator = 0) = 0;

    // This clears this menu and submenus from all actions and submenus.
    // It does not destroy the submenus and commands,
    // just removes them from their parents.
    virtual void clear() = 0;
}; // end of class Core::ActionContainer

} // end of namespace Core

#endif // ACTIONCONTAINER_H
