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

#include <QAction>
#include <QDebug>
#include <QMenuBar>
#include <QToolBar>
#include <QSettings>

#include <commons/settingsmanager.h>

#include "core/appcore.h"
#include "actionmanager.h"
#include "actionmanager_p.h"

using namespace Core;
using namespace Core::Internal;

// ========== Core::ActionManager ========== //

/*!
  \class Core::ActionManager
  The ActionManager class is responsible for registration of menus,
  menu items and tool bar buttons.

  The ActionManager is a singleton class. You could not create the instance of
  this class but use ActionManager::instance() method
  or gCore->actionManager() instead.

  ActionManager is used for registering actions to application. You could
  group actions using <code>ActionGroup</code> and assign the same action
  different contexts (the copy/replace/undo/redo actions are different
  in different contexts, e.g.).

  \section1 Contexts

  All actions that are registered with the same Id (but different context lists)
  are considered to be overloads of the same command. Only one of
  the registered actions with the same ID is active at any time.

  If the current focus widget was registered via AppCore::addWidgetContext(),
  all the contexts returned by its WidgetContext object are active.
  In addition all contexts set via AppCore::addAdditionalContext() are active
  as well. If one of the actions was registered for one of these active contexts,
  it is the one active action, and receives \c triggered and \c toggled signals.

  The action that is visible to the user is the one returned by Command::action().
  If you provide yourself a user visible representation of your action you need
  to use Command::action() for this. When this action is invoked by the user,
  the signal is forwarded to the registered action that is valid for
  the current context.

  \section1 Registering Actions

  To register a globally active action "My Action", put the following code
  in your plugin's Plugin::initialize method:
  \code
      QAction *myAction = new QAction(tr("My Action"), this);
      Core::Command *cmd = gActionManager->registerAction(myAction,
                                                          "myplugin.myaction",
                                                          Core::Context(Core::Constants::CONTEXT_GLOBAL));
      connect(myAction, SIGNAL(triggered()), this, SLOT(performMyAction()));
  \endcode

  If you create e.g. a tool button that should represent the action you add the action
  from Command::action() to it:
  \code
      QToolButton *myButton = new QToolButton(someParentWidget);
      myButton->setDefaultAction(cmd->action());
  \endcode

  Also use the ActionManager to add items to registered action containers like
  the applications menu bar or menus in that menu bar. To do this, you could get
  the action container for a specific ID (like specified in the Core::Constants namespace)
  with a call of actionContainer(const Id&) and add your command to this container.

  Following the example adding "My Action" to the "Tools" menu would be done by
  \code
      gActionManager->actionContainer(Core::Constants::TOOLS)->addAction(cmd);
  \endcode

  \sa Core::AppCore
  \sa Core::Command
  \sa Core::ActionContainer
  \sa Core::WidgetContext
 */

/*!
  \macro gActionManager
  This macro expands to a global pointer referring to the unique action manager.
  It is equivalent to the pointer returned by the Core::ActionManager::instance().
 */

/*!
  \fn void ActionManager::commandListChanged()
  Emits when command list changed, e.g. another action added into command list
  or remove a command from the list.
 */

/*!
  \fn void ActionManager::commandAdded(const QString &id)
  Emits when a new command added with given \a id.
 */

GET_INSTANCE(ActionManager)

ActionManager::ActionManager(QObject *parent) :
    QObject(parent),
    d(new ActionManagerPrivate(this))
{
}

ActionManager::~ActionManager()
{
    delete d;
}

/*!
  Creates a new menu bar if the menu bar with the given \a id does not exist,
  otherwise the existing one will return.

  Returns a new ActionContainer that you can use to get the QMenuBar instance
  or to add menus to the menu bar. The ActionManager owns the returned ActionContainer.
 */
ActionContainer *ActionManager::createMenuBar(const Id &id)
{
    const ActionManagerPrivate::IdContainerMap::const_iterator it = d->m_idContainerMap.constFind(id);
    if (it !=  d->m_idContainerMap.constEnd()) {
        return it.value();
    }

    QMenuBar *mb = new QMenuBar; // No parent (System menu bar on Mac OS X)
    mb->setObjectName(id.toString());

    MenuBarActionContainer *mbc = new MenuBarActionContainer(id);
    mbc->setMenuBar(mb);

    d->m_idContainerMap.insert(id, mbc);
    connect(mbc, SIGNAL(destroyed()), d, SLOT(containerDestroyed()));

    return mbc;
}

/*!
  Creates a new menu if the menu with the given \a id does not exist,
  otherwise the existing one will return.

  Returns a new ActionContainer that you can use to get the QMenu instance
  or to add menu items to the menu. The ActionManager owns the returned ActionContainer.
  Add your menu to some other menu or a menu bar via the
  ActionManager::actionContainer() and ActionContainer::addMenu() methods.
*/
ActionContainer *ActionManager::createMenu(const Id &id)
{
    const ActionManagerPrivate::IdContainerMap::const_iterator it = d->m_idContainerMap.constFind(id);
    if (it != d->m_idContainerMap.constEnd()) {
        return it.value();
    }

    QMenu *m = new QMenu(gCore->mainWindow());
    m->setObjectName(QLatin1String(id.name()));

    MenuActionContainer *mc = new MenuActionContainer(id);
    mc->setMenu(m);

    d->m_idContainerMap.insert(id, mc);
    connect(mc, SIGNAL(destroyed()), d, SLOT(containerDestroyed()));

    return mc;
}

/*!
  Creates a new tool bar if the tool bar with the given \a id does not exist,
  otherwise the existing one will return.

  Returns a new ActionContainer that you can use to get the QToolBar instance
  or to add actions to the tool bar. The ActionManager owns the returned ActionContainer.
 */
ActionContainer *ActionManager::createToolBar(const Id &id)
{
    const ActionManagerPrivate::IdContainerMap::const_iterator it = d->m_idContainerMap.constFind(id);
    if (it !=  d->m_idContainerMap.constEnd()) {
        return it.value();
    }

    QToolBar *tb = new QToolBar(gCore->mainWindow());
    tb->setObjectName(id.toString());

    ToolBarActionContainer *tbc = new ToolBarActionContainer(id);
    tbc->setToolBar(tb);

    d->m_idContainerMap.insert(id, tbc);
    connect(tbc, SIGNAL(destroyed()), d, SLOT(containerDestroyed()));

    return tbc;
}

/*!
  Makes an \a action known to the system under the specified \a id.

  Returns an action command object that represents the action in the application and is
  owned by the ActionManager.

  This function will emit commandListChanged() and commandAdded(QString) signals.
 */
Command *ActionManager::registerAction(QAction *action, const Id &id, const Context &context)
{
    Action *act = d->overridableAction(id);
    if (act) {
        act->addOverrideAction(action, context);
        emit commandListChanged();
        emit commandAdded(id.toString());
    }
    return act;
}

/*!
  Makes a \a widget known to the system under the specified \a id.

  Returns a widget action command object that represents the widget in the application and is
  owned by the ActionManager.

  This function will emit commandListChanged() and commandAdded(QString) signals.
 */
Command *ActionManager::registerWidget(QWidget *widget, const Id &id, const Context &context)
{
    WidgetAction *widgetAction = 0;
    if (AbstractCommand *c = d->m_idCommandMap.value(id, 0)) {
        widgetAction = qobject_cast<WidgetAction *>(c);
        if (!widgetAction) {
            qWarning() << "registerWidget: id" << id.name()
                       << "is registered with a different command type.";
            return c;
        }
    } else {
        widgetAction = new WidgetAction(id);
        d->m_idCommandMap.insert(id, widgetAction);
    }

    if (widgetAction->widget()) {
        qWarning() << "registerWidget: widget already registered, id" << id.name() << ".";
        return widgetAction;
    }

    if (!d->hasContext(context)) {
        widget->setEnabled(false);
    }
    widget->setObjectName(id.toString());
    widget->setParent(gCore->mainWindow());
    widgetAction->setWidget(widget);

    if (context.isEmpty()) {
        widgetAction->setContext(Context(0));
    } else {
        widgetAction->setContext(context);
    }

    emit commandListChanged();
    emit commandAdded(id.toString());

    return widgetAction;
}

/*!
  Returns the ActionCommand object that is known to the system under the given \a id.

  \sa ActionManager::registerAction()
 */
Command *ActionManager::command(const Id &id) const
{
    const ActionManagerPrivate::IdCommandMap::const_iterator it = d->m_idCommandMap.constFind(id);
    if (it == d->m_idCommandMap.constEnd()) {
        return 0;
    }
    return it.value();
}

/*!
  Returns all action commands that have been registered.
 */
QList<Command *> ActionManager::commands() const
{
    QList<Command *> result;
    foreach (Command *cmd, d->m_idCommandMap.values()) {
        result << cmd;
    }
    return result;
}

/*!
  Returns the ActionContainter object that is know to the system under the given \a id.

  \sa ActionManager::menu()
  \sa ActionManager::menuBar()
*/
ActionContainer *ActionManager::actionContainer(const Id &id) const
{
    const ActionManagerPrivate::IdContainerMap::const_iterator it = d->m_idContainerMap.constFind(id);
    if (it == d->m_idContainerMap.constEnd()) {
        qDebug() << "No such action container with ID" << id.toString();
        return 0;
    }
    return it.value();
}

/*!
  Returns true if \a context is currently one of the active contexts.
 */
bool ActionManager::hasContext(int context) const
{
    return d->m_context.contains(context);
}


// ========== Core::Internal::ActionManagerPrivate ========== //

ActionManagerPrivate::ActionManagerPrivate(ActionManager *actionManager) :
    q(actionManager)
{
}

ActionManagerPrivate::~ActionManagerPrivate()
{
    // first delete containers to avoid them reacting to action deletion
    foreach (AbstractActionContainer *container, m_idContainerMap) {
        disconnect(container, SIGNAL(destroyed()), this, SLOT(containerDestroyed()));
    }
    qDeleteAll(m_idContainerMap.values());
    qDeleteAll(m_idCommandMap.values());
}

static const char settingsGroup[] = "KeyBindings";
static const char idKey[]         = "ID";
static const char sequenceKey[]   = "KeySequence";

void ActionManagerPrivate::initialize()
{
    QSettings *settings = Commons::SettingsManager::instance()->settings();
    const int shortcuts = settings->beginReadArray(QLatin1String(settingsGroup));
    for (int i = 0; i < shortcuts; ++i) {
        settings->setArrayIndex(i);
        const QKeySequence key(settings->value(QLatin1String(sequenceKey)).toString());
        const Id id = Id(settings->value(QLatin1String(idKey)).toString());
        Command *cmd = q->command(id);
        if (cmd) {
            cmd->setKeySequence(key);
        }
    }
    settings->endArray();
}

void ActionManagerPrivate::setContext(const Context &context)
{
    // here are possibilities for speed optimization if necessary:
    // let commands (de-)register themselves for contexts
    // and only update commands that are either in old or new contexts
    m_context = context;
    IdCommandMap::const_iterator it = m_idCommandMap.constBegin();
    const IdCommandMap::const_iterator end = m_idCommandMap.constEnd();
    for (; it != end; ++it) {
        it.value()->setCurrentContext(m_context);
    }
}

bool ActionManagerPrivate::hasContext(const Context &context) const
{
    for (int i = 0; i < m_context.size(); ++i) {
        if (context.contains(m_context.at(i))) {
            return true;
        }
    }
    return false;
}

Action *ActionManagerPrivate::overridableAction(const Id &id)
{
    Action *act = 0;
    if (AbstractCommand *c = m_idCommandMap.value(id, 0)) {
        act = qobject_cast<Action *>(c);
        if (!act) {
            qWarning() << "registerAction: id" << id.name()
                       << "is registered with a different command type.";
            return 0;
        }
    } else {
        act = new Action(id);
        m_idCommandMap.insert(id, act);
        gCore->mainWindow()->addAction(act->action());
        act->action()->setObjectName(id.toString());
        act->action()->setShortcutContext(Qt::ApplicationShortcut);
        act->setCurrentContext(m_context);
    }

    return act;
}

void ActionManagerPrivate::saveSettings(QSettings *settings)
{
    settings->beginWriteArray(QLatin1String(settingsGroup));
    int count = 0;

    const IdCommandMap::const_iterator cmdMapEnd = m_idCommandMap.constEnd();
    IdCommandMap::const_iterator it = m_idCommandMap.constBegin();
    while (it != cmdMapEnd) {
        const Id id = it.key();
        AbstractCommand *cmd = it.value();
        QKeySequence key = cmd->keySequence();
        if (key != cmd->defaultKeySequence()) {
            settings->setArrayIndex(count);
            settings->setValue(QLatin1String(idKey), id.toString());
            settings->setValue(QLatin1String(sequenceKey), key.toString());
            count++;
        }
        ++it;
    }
    settings->endArray();
}

Action *ActionManagerPrivate::createAction(const Id &id)
{
    Action *act = 0;
    if (AbstractCommand *cmd = m_idCommandMap.value(id, 0)) {
        act = qobject_cast<Action *>(cmd);
        if (!act) {
            qWarning() << "registerAction: id" << id.name()
                       << "is registered with a different command type.";
            return 0;
        }
    } else {
        act = new Action(id);
        m_idCommandMap.insert(id, act);
    }
    return act;
}

void ActionManagerPrivate::containerDestroyed()
{
    AbstractActionContainer *container = static_cast<AbstractActionContainer *>(sender());
    m_idContainerMap.remove(m_idContainerMap.key(container));
}
