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

#include <commons/objectpool.h>

#include "plugin.h"
#include "plugin_p.h"

using namespace PluginSystem;
using namespace PluginSystem::Internal;

/*!
  \class PluginSystem::Plugin
  Base class for all plugins.

  The Plugin class is an abstract class that must be implemented once for each plugin.
  A plugin consists of two parts: a description file, and a library
  that at least contains the Plugin implementation.
 */

// ========== PluginSystem::Plugin ========== //

/*!
  Constructs an instance of plugin.
 */
Plugin::Plugin() :
    d(new PluginPrivate(this))
{
}

/*!
  Destroys the instance of this plugin.
 */
Plugin::~Plugin()
{
    foreach (QObject *obj, d->addedObjectsInReverseOrder) {
        d->objectPool->removeObject(obj);
    }
    qDeleteAll(d->addedObjectsInReverseOrder);
    d->addedObjectsInReverseOrder.clear();
    delete d;
    d = 0;
}

/*!
  Returns the PluginSpec corresponding to this plugin.

  This is not available in the constructor.
 */
PluginSpec *Plugin::pluginSpec() const
{
    return d->pluginSpec;
}

/*!
  Convenience method that registers \a obj in the objects pool
  by just calling ObjectPool::addObject().
*/
void Plugin::addObject(QObject *obj)
{
    d->objectPool->addObject(obj);
}

/*!
  Convenience method for registering \a obj in the objects pool.

  Usually, registered objects must be removed from the object pool and deleted by hand.
  Objects added to the pool via addAutoReleasedObject() are automatically removed
  and deleted in reverse order of registration when the Plugin instance is destroyed.

  \sa PluginManager::addObject()
*/
void Plugin::addAutoReleasedObject(QObject *obj)
{
    d->addedObjectsInReverseOrder.prepend(obj);
    d->objectPool->addObject(obj);
}

/*!
  Convenience method that unregisters \a obj from the objects pool
  by just calling ObjectPool::removeObject().
*/
void Plugin::removeObject(QObject *obj)
{
    d->objectPool->removeObject(obj);
}

// ========== PluginSystem::Internal::PluginPrivate ========== //

/*!
  \class PluginSystem::Internal::PluginPrivate
  The PluginPrivate class is the private implementation of Plugin.
 */

PluginPrivate::PluginPrivate(Plugin *plugin) :
    objectPool(Commons::ObjectPool::instance()),
    q(plugin)
{
}

/*!
  \fn bool Plugin::initialize(const QStringList &arguments, QString *errorString)
  Calls after the plugin has been loaded and the Plugin instance has been created.

  The initialize methods of plugins that depend on this plugin are called after
  the initialize method of this plugin has been called. Plugins should initialize their
  internal state in this method. Returns if initialization of successful.

  If it wasn't successful, the \a errorString should be set to a user-readable message
  describing the reason.

 \sa dependenciesInitialized()
*/

/*!
  \fn void Plugin::dependenciesInitialized()
  Calls after the Plugin::initialize() method has been called,
  and after both the Plugin::initialize() and Plugin::dependenciesInitialized()
  methods of plugins that depend on this plugin have been called.

  In this method, the plugin can assume that plugins that depend on
  this plugin are fully 'up and running'. It is a good place to look in the
  object pool for objects that have been provided by dependent plugins.

  \sa initialize()
 */

/*!
  \fn ShutdownFlag Plugin::aboutToShutdown()
  Called during a shutdown sequence in the same order as initialization
  before the plugins get deleted in reverse order.

  This method should be used to disconnect from other plugins,
  hide all UI, and optimize shutdown in general.
  If a plugin needs to delay the real shutdown for a while, for example if
  it needs to wait for external processes to finish for a clean shutdown,
  the plugin can return Plugin::AsynchronousShutdown from this method. This
  will keep the main event loop running after the aboutToShutdown() sequence
  has finished, until all plugins requesting AsynchronousShutdown have sent
  the asynchronousShutdownFinished() signal.

  The default implementation of this method does nothing and returns
  Plugin::SynchronousShutdown.

  Returns IPlugin::AsynchronousShutdown if the plugin needs to perform
  asynchronous actions before performing the shutdown.

  \sa asynchronousShutdownFinished()
*/

/*!
  \fn void Plugin::asynchronousShutdownFinished()
  Sent by the plugin implementation after a asynchronous shutdown
  is ready to proceed with the shutdown sequence.

  \sa aboutToShutdown()
 */
