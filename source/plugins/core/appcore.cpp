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

#include <QMutex>

#include <commons/objectpool.h>
#include <commons/settingsmanager.h>

#include "appcore.h"
#include "appcore_p.h"
#include "coreconstants.h"
#include "actionsystem/actionmanager.h"
#include "actionsystem/actionmanager_p.h"

/*!
  \namespace Core
  The Core namespace contains platform services, such as application
  data manager, action manager, etc.
 */

using namespace Core;
using namespace Core::Internal;

// ========== Core::AppCore ========== //

/*!
  \class Core::AppCore
  The AppCore class allows access to the different part that make up
  the basic functionality of the whole application.

  You should never create a subclass of this class. The one and only
  instance is created by the application. You can access this instance
  from your plugin through AppCore::instance() or simply use gCore macro
  for short.
 */

/*!
  \macro gCore
  This macro expands to a global pointer referring to the unique core object.
  It is equivalent to the pointer returned by the AppCore::instance().
 */

/*!
  \fn void AppCore::coreAboutToOpen()
  Emits when the core plugin is about to show.
 */

/*!
  \fn void AppCore::coreAboutToClose()
  Emits when the core plugin is about to close.

  Plugins can do some pre-end-of-life actions when they get this signal.

  The application is guaranteed to shut down after this signal is emitted.
  It's there as an addition to the usual plugin lifecycle methods, namely
  Plugin::aboutToShutdown(), just for convenience.
 */

/*!
  \fn void AppCore::coreOpened()
  Emits after all plugins have been loaded and the main window shown.
 */

/*!
  \fn void AppCore::contextAboutToChange(WidgetContext *context)
  Sent just before a new \a context becomes the current context
  (meaning that its widget got focus).
 */

/*!
  \fn void AppCore::contextChanged(WidgetContext *context, const Context &additionalContexts)
  Sent just after a new \a context became the current context
  (meaning that its widget got focus), or if the additional context ids changed.
 */

GET_INSTANCE(AppCore)

/*!
  Constructs an instance of AppCore.

  You should never call this constructor directly.
  Use AppCore::instance() instead.
 */
AppCore::AppCore(QObject *parent) :
    QObject(parent),
    d(new AppCorePrivate(this))
{
}

/*!
  Destroys the instance of this class.
 */
AppCore::~AppCore()
{
    delete d;
}

/*!
  Returns the main window of this application.

  It is valid after core plugin initialized.
 */
QWidget *AppCore::mainWindow() const
{
    return d->mainWindow;
}

/*!
  Returns the widget context of the current main context.

  \sa AppCore::updateAdditionalContexts()
  \sa AppCore::addWidgetContext()
 */
WidgetContext *AppCore::currentWidgetContext() const
{
    return d->currentWidgetContext;
}

/*!
  Change the currently active additional contexts.

  Removes the list of additional contexts specified by \a remove and adds the
  list of additional contexts specified by \a add.

  \sa AppCore::hasContext()
 */
void AppCore::updateAdditionalContexts(const Context &remove, const Context &add)
{
    d->updateAdditionalContexts(remove, add);
}

/*!
  Returns if the given \a context is currently one of the active contexts.

  \sa AppCore::updateAdditionalContexts()
  \sa AppCore::addWidgetContext()
 */
bool AppCore::hasContext(int context) const
{
    return gActionManager->hasContext(context);
}

/*!
  Registers an additional \a context object.

  After registration this context object gets automatically the
  current context object whenever its widget gets focus.

  \sa AppCore::removeWidgetContext()
  \sa AppCore::updateAdditionalContexts()
  \sa AppCore::currentWidgetContext()
 */
void AppCore::addWidgetContext(WidgetContext *context)
{
    d->addWidgetContext(context);
}

/*!
  Unregisters a \a context object from the list of know contexts.

  \sa AppCore::addWidgetContext()
  \sa AppCore::updateAdditionalContexts()
  \sa AppCore::currentWidgetContext()
 */
void AppCore::removeWidgetContext(WidgetContext *context)
{
    d->removeWidgetContext(context);
}


// ========== Core::Internal::AppCorePrivate ========== //

/*
  \class Core::Internal::AppCorePrivate
  The AppCorePrivate class is the private implementation of AppCore.
 */

AppCorePrivate::AppCorePrivate(AppCore *ac) :
    additionalContexts(Constants::CONTEXT_GLOBAL),
    currentWidgetContext(0),
    q(ac)
{
}

WidgetContext *AppCorePrivate::widgetContext(QWidget *widget) const
{
    return widgetContexts.value(widget);
}

void AppCorePrivate::addWidgetContext(WidgetContext *context)
{
    if (!context) {
        return;
    }
    QWidget *widget = context->widget();
    if (widgetContexts.contains(widget)) {
        return;
    }
    widgetContexts.insert(widget, context);
}

void AppCorePrivate::removeWidgetContext(WidgetContext *context)
{
    if (!context) {
        return;
    }
    QWidget *widget = context->widget();
    if (!widgetContexts.contains(widget)) {
        return;
    }
    widgetContexts.remove(widget);
    if (currentWidgetContext == context) {
        updateWidgetContext(0);
    }
}

void AppCorePrivate::updateWidgetContext(WidgetContext *context)
{
    if (context == currentWidgetContext) {
        return;
    }
    emit q->contextAboutToChange(context);
    currentWidgetContext = context;
    updateContext();
}

void AppCorePrivate::resetContext()
{
    updateWidgetContext(0);
}

void AppCorePrivate::updateAdditionalContexts(const Context &remove, const Context &add)
{
    foreach (const int context, remove) {
        if (context == 0) {
            continue;
        }
        int index = additionalContexts.indexOf(context);
        if (index != -1) {
            additionalContexts.removeAt(index);
        }
    }

    foreach (const int context, add) {
        if (context == 0) {
            continue;
        }
        if (!additionalContexts.contains(context)) {
            additionalContexts.prepend(context);
        }
    }

    updateContext();
}

void AppCorePrivate::updateContext()
{
    Context contexts;

    if (currentWidgetContext) {
        contexts.add(currentWidgetContext->context());
    }
    contexts.add(additionalContexts);

    Context uniqueContexts;
    for (int i = 0; i < contexts.size(); ++i) {
        const int c = contexts.at(i);
        if (!uniqueContexts.contains(c)) {
            uniqueContexts.add(c);
        }
    }

    gActionManager->d->setContext(uniqueContexts);
    emit q->contextChanged(currentWidgetContext, additionalContexts);
}
