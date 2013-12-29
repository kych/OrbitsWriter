/*-------------------------------------------------
 *
 * OrbitsWriter - an Offline Blog Writer
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

#include <QAction>
#include <QDebug>
#include <QEvent>
#include <QTextStream>

#include "command.h"
#include "command_p.h"

using namespace Core;
using namespace Core::Internal;

/*!
  \class Core::Command
  The Command class represents an action like a menu item, tool button, etc.
  You don't create Command objects directly, instead use
  ActionManager::registerAction() to register an action and retrieve a Command.
  The Command object represents the user visible action and its properties.
  If multiple actions are registered with the same ID (but different contexts)
  the returned Command is the shared one between these actions.

  A Command has two basic properties: A default shortcut and a default text.
  The default shortcut is a key sequence that the user can use to trigger
  the active action that the Command represents. If the default text is empty,
  the text of the visible action is used.

  The user visible action is updated to represent the state of the active action
  (if any). For performance reasons only the enabled and visible state
  are considered by default though. You can tell a Command to also update
  the actions icon and text by setting the corresponding
  Command::CommandAttribute().

  If there is no active action, the default behavior of the visible action
  is to be disabled.
 */

/*!
  \enum Command::CommandAttribute
  Defines how the user visible action is updated when the active action changes.

  The default is to update the enabled and visible state, and to disable the
  user visible action when there is no active action.
 */

/*!
  \var Command::CommandAttribute Command::CA_Hide
  When there is no active action, hide the user "visible" action,
  instead of just disabling it.
 */

/*!
  \var Command::CommandAttribute Command::CA_UpdateText
   Also update the actions text.
 */

/*!
  \var Command::CommandAttribute Command::CA_UpdateIcon
  Also update the actions icon.
 */

/*!
  \var Command::CommandAttribute Command::CA_NonConfigurable
  Flag to indicate that the keyboard shortcut of this Command
  should not be configurable by the user.
 */

/*!
  \fn void Command::setAttribute(Command::CommandAttribute attr)
  Add the \a attr to the attributes of this Command.

  \sa CommandAttribute
  \sa removeAttribute()
  \sa hasAttribute()
*/

/*!
  \fn void Command::removeAttribute(Command::CommandAttribute attr)
  Remove the \a attr from the attributes of this Command.

  \sa CommandAttribute
  \sa setAttribute()
*/

/*!
  \fn bool Command::hasAttribute(Command::CommandAttribute attr) const
  Returns if the Command has the \a attr set.

  \sa CommandAttribute
  \sa removeAttribute()
  \sa setAttribute()
*/

/*!
  \fn Id Command::id() const
  Returns the id of this Command.
 */

/*!
  \fn void Command::setDescription(const QString &text)
  Sets the \a text to represent the Command.

  If you don't set this, the current text from the user visible action is taken
  (which is OK in many cases).
 */

/*!
  \fn QString Command::description() const
  Returns the text that is used to present this Command to the user.

  \sa setDescription()
 */

/*!
  \fn QAction * Command::action() const
  Returns the user visible action for this Command.
 */

/*!
  \fn QWidget * Command::widget() const
  Returns the widget for this Command if the registered action is
  a QWidgetAction instance, otherwise return 0.
 */

/*!
  \fn Context Command::context() const
  Returns current context of this Command.
 */

/*!
  \fn void Command::setDefaultKeySequence(const QKeySequence &key)
  Sets the default keyboard shortcut that should be used to activate thie Command.

  \sa defaultKeySequence()
 */

/*!
  \fn QKeySequence Command::defaultKeySequence() const
  Returns the default keyboard shortcut that can be used to activate this Command.

  \sa setDefaultKeySequence()
 */

/*!
  \fn QKeySequence Command::keySequence() const
  Returns the current keyboard shortcut assigned to this Command.

  \sa defaultKeySequence()
 */

/*!
  \fn void Command::setKeySequence(const QKeySequence &key)
  Sets shortcut for this Command.
 */

/*!
  \fn bool Command::isActive() const
  Returns if the Command has an active action.
 */

/*!
  \fn void Command::keySequenceChanged()
  Emits when the keyboard shortcut assigned to this Command changes,
  e.g. when the user sets it in the keyboard shortcut settings dialog.
 */

/*!
  \fn void Command::activeStateChanged()
  Emits when the active state of this Command changes.
 */

static QString __msgActionWarning(QAction *newAction, int k, QAction *oldAction)
{
    QString msg;
    QTextStream str(&msg);
    str << "addOverrideAction " << newAction->objectName() << '/' << newAction->text()
         << ": Action ";
    if (oldAction)
        str << oldAction->objectName() << '/' << oldAction->text();
    str << " is already registered for context " << k << ' '
        << Core::Id::fromUniqueIdentifier(k).toString()
        << '.';
    return msg;
}

// ========== Core::Internal::AbstractCommand ========== //

AbstractCommand::AbstractCommand(Id id) :
    m_id(id),
    m_isKeyInitialized(false),
    m_attributes(0)
{
}

Id AbstractCommand::id() const
{
    return m_id;
}

void AbstractCommand::setDescription(const QString &text)
{
    m_defaultDesc = text;
}

QString AbstractCommand::description() const
{
    if (!m_defaultDesc.isEmpty()) {
        return m_defaultDesc;
    }
    if (action()) {
        QString text = action()->text();
        text.remove(QRegExp(QLatin1String("&(?!&)")));
        if (!text.isEmpty()) {
            return text;
        }
    }
    return id().toString();
}

void AbstractCommand::setDefaultKeySequence(const QKeySequence &key)
{
    if (!m_isKeyInitialized) {
        setKeySequence(key);
    }
    m_defaultKey = key;
}

QKeySequence AbstractCommand::defaultKeySequence() const
{
    return m_defaultKey;
}

void AbstractCommand::setKeySequence(const QKeySequence &key)
{
    Q_UNUSED(key)
    m_isKeyInitialized = true;
}

void AbstractCommand::setAttribute(CommandAttribute attr)
{
    m_attributes |= attr;
}

void AbstractCommand::removeAttribute(CommandAttribute attr)
{
    m_attributes &= ~attr;
}

bool AbstractCommand::hasAttribute(CommandAttribute attr) const
{
    return (m_attributes & attr);
}


// ========== Core::Internal::Action ========== //

Action::Action(Id id) :
    AbstractCommand(id),
    m_action(new ProxyAction(this)),
    m_active(false),
    m_contextInitialized(false)
{
    m_action->setShortcutVisibleInToolTip(true);
    connect(m_action, SIGNAL(changed()), this, SLOT(updateActiveState()));
}

void Action::setKeySequence(const QKeySequence &key)
{
    AbstractCommand::setKeySequence(key);
    m_action->setShortcut(key);
    emit keySequenceChanged();
}

QKeySequence Action::keySequence() const
{
    return m_action->shortcut();
}

QAction *Action::action() const
{
    return m_action;
}

bool Action::isActive() const
{
    return m_active;
}

void Action::setCurrentContext(const Context &context)
{
    m_context = context;

    QAction *currentAction = 0;
    for (int i = 0; i < m_context.size(); ++i) {
        if (QAction *a = m_contextActionMap.value(m_context.at(i), 0)) {
            currentAction = a;
            break;
        }
    }

    m_action->setAction(currentAction);
    updateActiveState();
}

void Action::addOverrideAction(QAction *action, const Context &context)
{
    if (isEmpty()) {
        m_action->initialize(action);
    }
    if (context.isEmpty()) {
        m_contextActionMap.insert(0, action);
    } else {
        for (int i = 0; i < context.size(); ++i) {
            int k = context.at(i);
            if (m_contextActionMap.contains(k)) {
                qWarning("%s", qPrintable(__msgActionWarning(action, k, m_contextActionMap.value(k, 0))));
            }
            m_contextActionMap.insert(k, action);
        }
    }
    setCurrentContext(m_context);
}

void Action::removeOverrideAction(QAction *action)
{
    QMutableMapIterator<int, QPointer<QAction> > it(m_contextActionMap);
    while (it.hasNext()) {
        it.next();
        if (it.value() == 0) {
            it.remove();
        } else if (it.value() == action) {
            it.remove();
        }
    }
    setCurrentContext(m_context);
}

bool Action::isEmpty() const
{
    return m_contextActionMap.isEmpty();
}

void Action::setAttribute(Command::CommandAttribute attr)
{
    AbstractCommand::setAttribute(attr);
    switch (attr) {
    case CA_Hide:
        m_action->setAttribute(ProxyAction::Hide);
        break;
    case CA_UpdateText:
        m_action->setAttribute(ProxyAction::UpdateText);
        break;
    case CA_UpdateIcon:
        m_action->setAttribute(ProxyAction::UpdateIcon);
        break;
    case CA_NonConfigurable:
        break;
    }
}

void Action::removeAttribute(Command::CommandAttribute attr)
{
    AbstractCommand::removeAttribute(attr);
    switch (attr) {
    case CA_Hide:
        m_action->removeAttribute(ProxyAction::Hide);
        break;
    case CA_UpdateText:
        m_action->removeAttribute(ProxyAction::UpdateText);
        break;
    case CA_UpdateIcon:
        m_action->removeAttribute(ProxyAction::UpdateIcon);
        break;
    case CA_NonConfigurable:
        break;
    }
}

void Action::updateActiveState()
{
    setActive(m_action->isEnabled()
              && m_action->isVisible()
              && !m_action->isSeparator());
}

void Action::setActive(bool state)
{
    if (state != m_active) {
        m_active = state;
        emit activeStateChanged();
    }
}


// ========== Core::Internal::WidgetAction ========== //

WidgetAction::WidgetAction(Id id) :
    AbstractCommand(id),
    m_widgetAction(new QWidgetAction(this)),
    m_active(false),
    m_updatingContext(false)
{
}

void WidgetAction::setKeySequence(const QKeySequence &key)
{
    AbstractCommand::setKeySequence(key);
    m_widgetAction->setShortcut(key);
    emit keySequenceChanged();
}

QKeySequence WidgetAction::keySequence() const
{
    return m_widgetAction->shortcut();
}

QAction *WidgetAction::action() const
{
    return m_widgetAction;
}

void WidgetAction::setWidget(QWidget *widget)
{
    m_widgetAction->setDefaultWidget(widget);
    widget->installEventFilter(this);
}

QWidget *WidgetAction::widget() const
{
    return m_widgetAction->defaultWidget();
}

bool WidgetAction::isActive() const
{
    return m_active;
}

void WidgetAction::setContext(const Context &context)
{
    m_context = context;
    setCurrentContext(m_context);
}

Context WidgetAction::context() const
{
    return m_context;
}

void WidgetAction::setCurrentContext(const Context &context)
{
    m_updatingContext = true;
    foreach (int ctx, m_context) {
        if (context.contains(ctx)) {
            if (!m_widgetAction->isEnabled()) {
                m_widgetAction->setEnabled(true);
                emit activeStateChanged();
            }
            return;
        }
    }
    if (m_widgetAction->isEnabled()) {
        m_widgetAction->setEnabled(false);
        emit activeStateChanged();
    }

    updateActiveState();
    m_updatingContext = false;
}

bool WidgetAction::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_widgetAction->defaultWidget()) {
        if (event->type() == QEvent::EnabledChange) {
            if (!m_updatingContext) {
                setCurrentContext(m_context);
            }
            return true;
        } else {
            return false;
        }
    } else {
        return AbstractCommand::eventFilter(obj, event);
    }
}

void WidgetAction::updateActiveState()
{
    setActive(m_widgetAction->isEnabled()
              && m_widgetAction->isVisible()
              && !m_widgetAction->isSeparator());
}

void WidgetAction::setActive(bool state)
{
    if (state != m_active) {
        m_active = state;
        emit activeStateChanged();
    }
}
