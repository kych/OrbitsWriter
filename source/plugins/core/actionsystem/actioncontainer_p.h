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

#ifndef ACTIONCONTAINER_P_H
#define ACTIONCONTAINER_P_H

#include <QList>

#include "core/actionsystem/actioncontainer.h"
#include "core/actionsystem/command.h"

namespace Core
{
namespace Internal
{

/*!
  Action group stands for a group that actions or action containers could
  add to.
 */
struct ActionGroup
{
    ActionGroup(const Id &id) : id(id) {}
    Id id;
    QList<QObject *> items; // Command * or ActionContainer *
}; // end of struct Core::Internal::ActionGroup


class AbstractActionContainer : public ActionContainer
{
    Q_OBJECT
public:
    AbstractActionContainer(Id id);
    virtual ~AbstractActionContainer() {}

    virtual Id id() const { return m_id; }

    virtual QMenu *menu() const { return 0; }
    virtual QMenuBar *menuBar() const { return 0; }
    virtual QToolBar *toolBar() const { return 0; }

    virtual QAction * actionAt(const Id &group) const;
    virtual void appendActionGroup(const Id &group);
    virtual void insertActionGroup(const Id &before, const Id &group);

    virtual void addAction(Command *cmd, const Id &group = Id());
    virtual void addMenu(ActionContainer *menu, const Id &group = Id());
    virtual void addMenu(ActionContainer *before, ActionContainer *menu, const Id &group = Id());
    virtual Command * addSeparator(const Context &context, const Id &group = Id(), QAction **outSeparator = 0);

    virtual void insertAction(QAction *before, QAction *action) = 0;
    virtual void insertMenu(QAction *before, QMenu *menu) = 0;

    virtual void removeAction(QAction *action) = 0;
    virtual void removeMenu(QMenu *menu) = 0;

    virtual bool updateInternal() = 0;

    virtual void clear();

protected:
    bool canAddAction(Command *cmd) const;
    virtual bool canBeAddedToMenu() const = 0;
    virtual bool canBeAddedToMenuBar() const = 0;
    QList<ActionGroup> m_groups;

private slots:
    void scheduleUpdate();
    void update();
    void itemDestroyed();

private:
    QList<ActionGroup>::const_iterator findActionGroup(const Id &groupId) const;
    QAction * actionAt(QList<ActionGroup>::const_iterator group) const;

    Id m_id;
    bool m_updateRequested;
}; // end of class Core::Internal::AbstractActionContainer


class MenuBarActionContainer : public AbstractActionContainer
{
    Q_OBJECT
public:
    explicit MenuBarActionContainer(Id id);

    void setMenuBar(QMenuBar *menuBar);
    QMenuBar *menuBar() const;

    void insertAction(QAction *before, QAction *action);
    void insertMenu(QAction *before, QMenu *menu);

    void removeAction(QAction *action);
    void removeMenu(QMenu *menu);

protected:
    bool canBeAddedToMenu() const;
    bool canBeAddedToMenuBar() const;
    bool updateInternal();

private:
    QMenuBar *m_menuBar;
}; // end of class Core::Internal::MenuBarActionContainer


class MenuActionContainer : public AbstractActionContainer
{
    Q_OBJECT
public:
    explicit MenuActionContainer(Id id);

    void setMenu(QMenu *menu);
    QMenu *menu() const;

    void insertAction(QAction *before, QAction *action);
    void insertMenu(QAction *before, QMenu *menu);

    void removeAction(QAction *action);
    void removeMenu(QMenu *menu);

protected:
    bool canBeAddedToMenu() const;
    bool canBeAddedToMenuBar() const;
    bool updateInternal();

private:
    QMenu *m_menu;
}; // end of class Core::Internal::MenuActionContainer


class ToolBarActionContainer : public AbstractActionContainer
{
    Q_OBJECT
public:
    explicit ToolBarActionContainer(Id id);

    void setToolBar(QToolBar *toolBar);
    QToolBar *toolBar() const;

    void insertAction(QAction *before, QAction *action);
    void insertMenu(QAction *before, QMenu *menu);

    void removeAction(QAction *action);
    void removeMenu(QMenu *menu);

protected:
    bool canBeAddedToMenu() const;
    bool canBeAddedToMenuBar() const;
    bool updateInternal();

private:
    QToolBar *m_toolBar;
}; // end of class Core::Internal::ToolBarActionContainer

} // end of namespace Core::Internal
} // end of namespace Core

#endif // ACTIONCONTAINER_P_H
