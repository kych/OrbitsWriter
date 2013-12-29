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

#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>

#include <commons/singleton.h>

#include "core/actionsystem/command.h"
#include "core/core_global.h"

#define gActionManager (Core::ActionManager::instance())

namespace Core
{

class ActionContainer;
class MainWindow;
class Id;

namespace Internal
{
class ActionManagerPrivate;
class AppCorePrivate;
}

class CORE_EXPORT ActionManager : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(ActionManager)
public:
    ActionContainer * createMenuBar(const Id &id);
    ActionContainer * createMenu(const Id &id);
    ActionContainer * createToolBar(const Id &id);

    Command * registerAction(QAction *action, const Id &id, const Context &context);
    Command * registerWidget(QWidget *widget, const Id &id, const Context &context);

    Command *command(const Id &id) const;

    QList<Command *> commands() const;

    ActionContainer *actionContainer(const Id &id) const;

    bool hasContext(int context) const;

signals:
    void commandListChanged();
    void commandAdded(const QString &id);

private:
    explicit ActionManager(QObject *parent = 0);
    ~ActionManager();

    Internal::ActionManagerPrivate *d;
    friend class Internal::ActionManagerPrivate;
    friend class Internal::AppCorePrivate;
    friend class MainWindow;
}; // end of class Core::ActionManager

} // end of namespace Core

#endif // ACTIONMANAGER_H
