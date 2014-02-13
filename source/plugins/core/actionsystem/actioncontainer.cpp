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

#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QTimer>
#include <QToolBar>
#include <QWidgetAction>

#include "actioncontainer.h"
#include "actioncontainer_p.h"
#include "actionmanager.h"
#include "command.h"
#include "core/coreconstants.h"

using namespace Core;
using namespace Core::Internal;

/*!
  \class Core::ActionContainer
  The ActionContainer class represents a menu, menu bar or tool bar.

  You should not create instances of this class directly, but use the
  ActionManager::createMenu() and ActionManager::createMenuBar() methods instead.
  Retrieves existing action containers for an ID
  with ActionManager::actionContainer().

  You could group menus and items together by defining groups,
  this groups are instances of ActionGroup. In order to define an action group,
  use ActionContainer::appendActionGroup() method then add menus/actions
  to these groups. If no custom groups are defined, an action container
  could add to three default groups Core::Constants::G_DEFAULT_ACTIONGROUP_1,
  Core::Constants::G_DEFAULT_ACTIONGROUP_2
  and Core::Constants::G_DEFAULT_ACTIONGROUP_3.

  Actions usually an instance of QAction. If a widget is needed, QWidgetAction
  could be used.
 */

/*!
  \fn Id ActionContainer::id() const
  Returns the id of this container.
 */

/*!
  \fn QMenu * ActionContainer::menu() const
  Returns the QMenu instance that is represented by this action container,
  or 0 if this action container does not represent a menu.
 */

/*!
  \fn QMenuBar * ActionContainer::menuBar() const
  Returns the QMenuBar instance that is represented by this action container,
  or 0 if this action container does not represent a menu bar.
 */

/*!
  \fn QToolBar * ActionContainer::toolBar() const
  Returns the QToolBar instance that is represented by this action container,
  or 0 if this action container does not represent a tool bar.
 */

/*!
  \fn void ActionContainer::appendActionGroup(const Id &group)
  Adds an action group with the given identifier \a group to the action container.

  Using groups you can segment your action container into logical parts
  and add actions directly to these parts.

  \sa insertActionGroup()
  \sa addAction()
  \sa addMenu()
 */

/*!
  \fn void ActionContainer::insertActionGroup(const Id &before, const Id &group)
  Inserts an action group with the given identifier \a group to
  the action container before the group \a before.

  \sa appendActionGroup()
 */

/*!
  \fn QAction * ActionContainer::actionAt(const Id &group) const
  Returns an action representing the \a group.
 */

/*!
  \fn void ActionContainer::addAction(Command *cmd, const Id &groupId = Id())
  Adds the \a cmd to this action container.

  The action may be a menu item or a tool bar button, according to the container implemation.
  The action is added as the last item of the specified \a group.

  \sa appendActionGroup()
  \sa addMenu()
  \sa addWidget()
 */

/*!
  \fn void ActionContainer::addMenu(ActionContainer *menu, const Id &group = Id())
  Adds the \a menu to this action container.

  The menu may be a submenu or a tool bar button with menu, according to the container implemation.
  The menu is added as the last item of the specified \a group.

  \sa appendActionGroup()
  \sa addAction()
  \sa addWidget()
 */

/*!
  \fn void ActionContainer::addMenu(ActionContainer *before, ActionContainer *menu, const Id &group = Id())
  Adds the \a menu to this action container.

  The menu may be a submenu or a tool bar button with menu, according to the container implemation.
  The menu is inserted before the position \a before of the specified \a group.

  \sa appendActionGroup()
  \sa addAction()
  \sa addWidget()
 */

/*!
  \fn Command * ActionContainer::addSeparator(const Context &context, const Id &group = Id(), QAction **outSeparator = 0)
  Adds a separator to the end of the given \a group to the action container.

  The created separator action is returned through \a outSeparator.

  Returns the created Command for the separator.
 */

// ========== Core::Internal::AbstractActionContainer ========== //

/*!
  \class Core::Internal::AbstractActionContainer
  The AbstractActionContainer class is the base for all action containers.

  This class supports management for action groups and other common functions.
  If special containers need be added, they should extends this base class.
 */

AbstractActionContainer::AbstractActionContainer(Id id) :
    m_id(id),
    m_updateRequested(false)
{
    appendActionGroup(Constants::G_DEFAULT_ACTIONGROUP_1);
    appendActionGroup(Constants::G_DEFAULT_ACTIONGROUP_2);
    appendActionGroup(Constants::G_DEFAULT_ACTIONGROUP_3);
}

QAction * AbstractActionContainer::actionAt(const Id &group) const
{
    QList<ActionGroup>::const_iterator it = findActionGroup(group);
    if (it == m_groups.constEnd()) {
        return 0;
    }
    return actionAt(it);
}

void AbstractActionContainer::appendActionGroup(const Id &group)
{
    m_groups.append(ActionGroup(group));
}

void AbstractActionContainer::insertActionGroup(const Id &before, const Id &group)
{
    QList<ActionGroup>::iterator it = m_groups.begin();
    while (it != m_groups.end()) {
        if (it->id == before) {
            m_groups.insert(it, ActionGroup(group));
            break;
        }
        ++it;
    }
}

void AbstractActionContainer::addAction(Command *cmd, const Id &group)
{
    if (!canAddAction(cmd)) {
        return;
    }

    const Id actualgroupId = group.isValid() ? group : Id(Constants::G_DEFAULT_ACTIONGROUP_2);
    QList<ActionGroup>::const_iterator groupId = findActionGroup(actualgroupId);
    if (groupId == m_groups.constEnd()) {
        qDebug() << "Can't find group"
                 << group.name() << "in container" << id().name();
        return;
    }
    QAction *beforeAction = actionAt(groupId);
    m_groups[groupId - m_groups.constBegin()].items.append(cmd);

    connect(cmd, SIGNAL(activeStateChanged()), this, SLOT(scheduleUpdate()));
    connect(cmd, SIGNAL(destroyed()), this, SLOT(itemDestroyed()));

    insertAction(beforeAction, cmd->action());
    scheduleUpdate();
}

void AbstractActionContainer::addMenu(ActionContainer *menu, const Id &group)
{
    AbstractActionContainer *abstractContainer = static_cast<AbstractActionContainer *>(menu);
    if (!abstractContainer->canBeAddedToMenu()) {
        return;
    }

    MenuActionContainer *container = static_cast<MenuActionContainer *>(abstractContainer);
    const Id actualgroupId = group.isValid() ? group : Id(Constants::G_DEFAULT_ACTIONGROUP_2);
    QList<ActionGroup>::const_iterator groupId = findActionGroup(actualgroupId);
    if (groupId == m_groups.constEnd()) {
        return;
    }
    QAction *beforeAction = actionAt(groupId);
    m_groups[groupId - m_groups.constBegin()].items.append(menu);

    connect(menu, SIGNAL(destroyed()), this, SLOT(itemDestroyed()));
    insertMenu(beforeAction, container->menu());
    scheduleUpdate();
}

void AbstractActionContainer::addMenu(ActionContainer *before, ActionContainer *menu, const Id &group)
{
    AbstractActionContainer *abstractContainer = static_cast<AbstractActionContainer *>(menu);
    if (!abstractContainer->canBeAddedToMenu()) {
        return;
    }

    MenuActionContainer *container = static_cast<MenuActionContainer *>(abstractContainer);
    const Id actualgroupId = group.isValid() ? group : Id(Constants::G_DEFAULT_ACTIONGROUP_2);
    QList<ActionGroup>::const_iterator groupId = findActionGroup(actualgroupId);
    if (groupId == m_groups.constEnd()) {
        return;
    }
    QAction *beforeAction = before->menu()->menuAction();
    m_groups[groupId - m_groups.constBegin()].items.append(menu);

    connect(menu, SIGNAL(destroyed()), this, SLOT(itemDestroyed()));
    insertMenu(beforeAction, container->menu());
    scheduleUpdate();
}

Command *AbstractActionContainer::addSeparator(const Context &context, const Id &group, QAction **outSeparator)
{
    static int separatorIdCount = 0;
    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    Command *cmd = gActionManager->registerAction(separator,
                                                  Id(QString::fromLatin1("%1.Separator.%2")
                                                     .arg(id().toString()).arg(++separatorIdCount)),
                                                  context);
    addAction(cmd, group);
    if (outSeparator) {
        *outSeparator = separator;
    }
    return cmd;
}

void AbstractActionContainer::clear()
{
    QMutableListIterator<ActionGroup> it(m_groups);
    while (it.hasNext()) {
        ActionGroup &group = it.next();
        foreach (QObject *item, group.items) {
            if (Command *command = qobject_cast<Command *>(item)) {
                removeAction(command->action());
                disconnect(command, SIGNAL(activeStateChanged()), this, SLOT(scheduleUpdate()));
                disconnect(command, SIGNAL(destroyed()), this, SLOT(itemDestroyed()));
            } else if (ActionContainer *container = qobject_cast<ActionContainer *>(item)) {
                container->clear();
                disconnect(container, SIGNAL(destroyed()), this, SLOT(itemDestroyed()));
                removeMenu(container->menu());
            }
        }
        group.items.clear();
    }
    scheduleUpdate();
}

QList<ActionGroup>::const_iterator AbstractActionContainer::findActionGroup(const Id &groupId) const
{
    QList<ActionGroup>::const_iterator it = m_groups.constBegin();
    while (it != m_groups.constEnd()) {
        if (it->id == groupId) {
            break;
        }
        ++it;
    }
    return it;
}

QAction *AbstractActionContainer::actionAt(QList<ActionGroup>::const_iterator group) const
{
    if (group == m_groups.constEnd()) {
        return 0;
    }
    ++group;
    while (group != m_groups.constEnd()) {
        if (!group->items.isEmpty()) {
            QObject *item = group->items.first();
            if (Command *cmd = qobject_cast<Command *>(item)) {
                return cmd->action();
            } else if (ActionContainer *container = qobject_cast<ActionContainer *>(item)) {
                if (container->menu()) {
                    return container->menu()->menuAction();
                }
            }
            break;
        }
        ++group;
    }
    return 0;
}

bool AbstractActionContainer::canAddAction(Command *cmd) const
{
    return cmd && cmd->action();
}

void AbstractActionContainer::scheduleUpdate()
{
    if (m_updateRequested) {
        return;
    }
    m_updateRequested = true;
    QTimer::singleShot(0, this, SLOT(update()));
}

void AbstractActionContainer::update()
{
    updateInternal();
    m_updateRequested = false;
}

void AbstractActionContainer::itemDestroyed()
{
    QObject *obj = sender();
    QMutableListIterator<ActionGroup> it(m_groups);
    while (it.hasNext()) {
        ActionGroup &group = it.next();
        if (group.items.removeAll(obj) > 0) {
            break;
        }
    }
}


// ========== Core::Internal::MenuBarActionContainer ========== //

/*!
  \class Core::Internal::MenuBarActionContainer
  The MenuBarActionContainer class is for menu bars.
 */

MenuBarActionContainer::MenuBarActionContainer(Id id) :
    AbstractActionContainer(id),
    m_menuBar(0)
{
}

void MenuBarActionContainer::setMenuBar(QMenuBar *menuBar)
{
    m_menuBar = menuBar;
}

QMenuBar *MenuBarActionContainer::menuBar() const
{
    return m_menuBar;
}

void MenuBarActionContainer::insertAction(QAction *before, QAction *action)
{
    m_menuBar->insertAction(before, action);
}

void MenuBarActionContainer::insertMenu(QAction *before, QMenu *menu)
{
    m_menuBar->insertMenu(before, menu);
}

void MenuBarActionContainer::removeAction(QAction *action)
{
    m_menuBar->removeAction(action);
}

void MenuBarActionContainer::removeMenu(QMenu *menu)
{
    m_menuBar->removeAction(menu->menuAction());
}

bool MenuBarActionContainer::canBeAddedToMenu() const
{
    return false;
}

bool MenuBarActionContainer::canBeAddedToMenuBar() const
{
    return false;
}

bool MenuBarActionContainer::updateInternal()
{
    bool hasItems = false;
    QList<QAction *> actions = m_menuBar->actions();
    foreach (QAction *act, actions) {
        if (act->isVisible()) {
            hasItems = true;
            break;
        }
    }
    return hasItems;
}


// ========== Core::Internal::MenuActionContainer ========== //

/*!
  \class Core::Internal::MenuActionContainer
  The MenuActionContainer class is for menus.
 */

MenuActionContainer::MenuActionContainer(Id id) :
    AbstractActionContainer(id),
    m_menu(0)
{
}

void MenuActionContainer::setMenu(QMenu *menu)
{
    m_menu = menu;
}

QMenu *MenuActionContainer::menu() const
{
    return m_menu;
}

void MenuActionContainer::insertAction(QAction *before, QAction *action)
{
    m_menu->insertAction(before, action);
}

void MenuActionContainer::insertMenu(QAction *before, QMenu *menu)
{
    m_menu->insertMenu(before, menu);
}

void MenuActionContainer::removeAction(QAction *action)
{
    m_menu->removeAction(action);
}

void MenuActionContainer::removeMenu(QMenu *menu)
{
    m_menu->removeAction(menu->menuAction());
}

bool MenuActionContainer::canBeAddedToMenu() const
{
    return true;
}

bool MenuActionContainer::canBeAddedToMenuBar() const
{
    return true;
}

bool MenuActionContainer::updateInternal()
{
    bool hasItems = false;
    QList<QAction *> actions = m_menu->actions();

    QList<ActionGroup>::const_iterator it = m_groups.constBegin();
    while (it != m_groups.constEnd()) {
        const ActionGroup &group = *it;
        foreach (QObject *item, group.items) {
            if (AbstractActionContainer *container = qobject_cast<AbstractActionContainer*>(item)) {
                actions.removeAll(container->menu()->menuAction());
                if (container == this) {
                    QByteArray warning = Q_FUNC_INFO + QByteArray(" container '");
                    if (this->menu()) {
                        warning += this->menu()->title().toLocal8Bit();
                    }
                    warning += "' contains itself as subcontainer";
                    qWarning("%s", warning.constData());
                    continue;
                }
                if (container->updateInternal()) {
                    hasItems = true;
                    break;
                }
            } else if (Command *act = qobject_cast<Command *>(item)) {
                actions.removeAll(act->action());
                if (act->isActive()) {
                    hasItems = true;
                    break;
                }
            } else {
                continue;
            }
        }
        if (hasItems) {
            break;
        }
        ++it;
    }
    if (!hasItems) {
        // look if there were actions added that we don't control and check if they are enabled
        foreach (const QAction *action, actions) {
            if (!action->isSeparator() && action->isEnabled()) {
                hasItems = true;
                break;
            }
        }
    }

    return hasItems;
}


// ========== Core::Internal::ToolBarActionContainer ========== //

/*!
  \class Core::Internal::ToolBarActionContainer
  The ToolBarActionContainer class is for menus.
 */

ToolBarActionContainer::ToolBarActionContainer(Id id) :
    AbstractActionContainer(id),
    m_toolBar(0)
{
}

void ToolBarActionContainer::setToolBar(QToolBar *toolBar)
{
    m_toolBar = toolBar;
}

QToolBar *ToolBarActionContainer::toolBar() const
{
    return m_toolBar;
}

void ToolBarActionContainer::insertAction(QAction *before, QAction *action)
{
    m_toolBar->insertAction(before, action);
}

void ToolBarActionContainer::insertMenu(QAction *before, QMenu *menu)
{
    m_toolBar->insertWidget(before, menu);
}

void ToolBarActionContainer::removeAction(QAction *action)
{
    m_toolBar->removeAction(action);
}

void ToolBarActionContainer::removeMenu(QMenu *menu)
{
    m_toolBar->removeAction(menu->menuAction());
}

bool ToolBarActionContainer::canBeAddedToMenu() const
{
    return false;
}

bool ToolBarActionContainer::canBeAddedToMenuBar() const
{
    return false;
}

bool ToolBarActionContainer::updateInternal()
{
    bool hasItems = false;
    QList<QAction *> actions = m_toolBar->actions();
    foreach (QAction *act, actions) {
        if (act->isVisible()) {
            hasItems = true;
            break;
        }
    }
    return hasItems;
}
