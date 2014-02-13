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

#ifndef COMMAND_P_H
#define COMMAND_P_H

#include <QKeySequence>
#include <QMap>
#include <QPointer>
#include <QWidgetAction>

#include "core/actionsystem/command.h"
#include "core/actionsystem/proxyaction.h"
#include "core/context.h"

namespace Core
{

namespace Internal
{

class AbstractCommand : public Command
{
    Q_OBJECT
public:
    AbstractCommand(Id id);
    virtual ~AbstractCommand() {}

    Id id() const;

    void setDescription(const QString &text);
    QString description() const;

    QAction *action() const { return 0; }
    QWidget *widget() const { return 0; }
    Context context() const { return m_context; }
    virtual void setCurrentContext(const Context &context) = 0;

    void setDefaultKeySequence(const QKeySequence &key);
    QKeySequence defaultKeySequence() const;
    void setKeySequence(const QKeySequence &key);

    void setAttribute(CommandAttribute attr);
    void removeAttribute(CommandAttribute attr);
    bool hasAttribute(CommandAttribute attr) const;

protected:
    CommandAttributes m_attributes;
    Id m_id;
    Context m_context;
    bool m_isKeyInitialized;
    QString m_defaultDesc;
    QKeySequence m_defaultKey;
}; // end of class Core::Internal::AbstractCommand


class Action : public AbstractCommand
{
    Q_OBJECT
public:
    Action(Id id);

    void setKeySequence(const QKeySequence &key);
    QKeySequence keySequence() const;

    QAction *action() const;

    bool isActive() const;

    void setCurrentContext(const Context &context);
    void addOverrideAction(QAction *action, const Context &context);
    void removeOverrideAction(QAction *action);
    bool isEmpty() const;

    void setAttribute(CommandAttribute attr);
    void removeAttribute(CommandAttribute attr);

private slots:
    void updateActiveState();

private:
    void setActive(bool state);

    ProxyAction *m_action;
    bool m_active;
    QString m_toolTip;

    QMap<int, QPointer<QAction> > m_contextActionMap;
    bool m_contextInitialized;
}; // end of class Core::Internal::Action


class WidgetAction : public AbstractCommand
{
    Q_OBJECT
public:
    WidgetAction(Id id);

    void setKeySequence(const QKeySequence &key);
    QKeySequence keySequence() const;

    QAction *action() const;
    void setWidget(QWidget *widget);
    QWidget *widget() const;

    bool isActive() const;

    void setContext(const Context &context);
    Context context() const;
    void setCurrentContext(const Context &context);

    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void updateActiveState();

private:
    void setActive(bool state);

    QWidgetAction *m_widgetAction;
    bool m_active;

    bool m_updatingContext;

//    QMap<int, QPointer<QWidgetAction> > m_contextWidgetActionMap;
//    bool m_contextInitialized;
}; // end of class Core::Internal::WidgetAction

} // end of namespace Core::Internal
} // end of namespace Core

#endif // COMMAND_P_H
