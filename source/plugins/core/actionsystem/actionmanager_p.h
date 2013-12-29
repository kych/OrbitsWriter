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

#ifndef ACTIONMANAGER_P_H
#define ACTIONMANAGER_P_H

#include <QHash>
#include <QObject>

#include "core/actionsystem/command_p.h"
#include "core/actionsystem/actioncontainer_p.h"

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace Core
{

class ActionManager;

namespace Internal
{

class ActionManagerPrivate : public QObject
{
    Q_OBJECT
public:
    typedef QHash<Id, AbstractCommand *> IdCommandMap;
    typedef QHash<Id, AbstractActionContainer *> IdContainerMap;

    explicit ActionManagerPrivate(ActionManager * actionManager);
    ~ActionManagerPrivate();

    void initialize();

    void setContext(const Context &context);
    bool hasContext(const Context &context) const;
    Action *overridableAction(const Id &id);
//    WidgetAction *overridableWidgetAction(const Id &id);

    void saveSettings(QSettings *settings);

    Action *createAction(const Id &id);

public slots:
    void containerDestroyed();

public:
    IdCommandMap m_idCommandMap;
    IdContainerMap m_idContainerMap;

    Context m_context;

private:
    Core::ActionManager *q;
}; // end of class Core::Internal::ActionManagerPrivate

} // end of namespace Core::Internal
} // end of namespace Core

#endif // ACTIONMANAGER_P_H
