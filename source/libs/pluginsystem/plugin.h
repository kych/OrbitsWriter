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

#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#if QT_VERSION >= 0x050000
#    include <QtPlugin>
#endif
#include "pluginsystem/pluginsystem_global.h"

QT_BEGIN_NAMESPACE
class QString;
class QStringList;
QT_END_NAMESPACE

namespace PluginSystem
{

class PluginSpec;

namespace Internal
{
class PluginPrivate;
class PluginSpecPrivate;
}

/*!
  Base class for all plugins.

  The Plugin class is an abstract class that must be implemented once for each plugin.
  A plugin consists of two parts: a description file, and a library
  that at least contains the Plugin implementation.
 */
class PLUGINSYSTEM_EXPORT Plugin : public QObject
{
    Q_OBJECT
public:
    enum ShutdownFlag {
        SynchronousShutdown,
        AsynchronousShutdown
    };

    /*!
      Constructs an instance of plugin.
     */
    Plugin();

    /*!
      Destroys the instance of this plugin.
     */
    ~Plugin();

    /*!
      Calls after the plugin has been loaded and the Plugin instance has been created.

      The initialize methods of plugins that depend on this plugin are called after
      the initialize method of this plugin has been called. Plugins should initialize their
      internal state in this method. Returns if initialization of successful.

      If it wasn't successful, the \a errorString should be set to a user-readable message
      describing the reason.

     \sa dependenciesInitialized()
    */
    virtual bool initialize(const QStringList &arguments, QString *errorString) = 0;

    /*!
      Calls after the Plugin::initialize() method has been called,
      and after both the Plugin::initialize() and Plugin::dependenciesInitialized()
      methods of plugins that depend on this plugin have been called.

      In this method, the plugin can assume that plugins that depend on
      this plugin are fully 'up and running'. It is a good place to look in the
      object pool for objects that have been provided by dependent plugins.

      \sa initialize()
     */
    virtual void dependenciesInitialized() = 0;

    /*!
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
    virtual ShutdownFlag aboutToShutdown()
    {
        return SynchronousShutdown;
    }

    /*!
      Returns the PluginSpec corresponding to this plugin.

      This is not available in the constructor.
     */
    PluginSpec *pluginSpec() const;

    /*!
      Convenience method that registers \a obj in the objects pool
      by just calling ObjectPool::addObject().
    */
    void addObject(QObject *obj);

    /*!
      Convenience method for registering \a obj in the objects pool.

      Usually, registered objects must be removed from the object pool and deleted by hand.
      Objects added to the pool via addAutoReleasedObject() are automatically removed
      and deleted in reverse order of registration when the Plugin instance is destroyed.

      \sa PluginManager::addObject()
    */
    void addAutoReleasedObject(QObject *obj);

    /*!
      Convenience method that unregisters \a obj from the objects pool
      by just calling ObjectPool::removeObject().
    */
    void removeObject(QObject *obj);

signals:
    /*!
      Sent by the plugin implementation after a asynchronous shutdown
      is ready to proceed with the shutdown sequence.

      \sa aboutToShutdown()
     */
    void asynchronousShutdownFinished();

private:
    Internal::PluginPrivate *d;
    friend class Internal::PluginPrivate;
    friend class Internal::PluginSpecPrivate;
}; // end of class PluginSystem::Plugin

} // end of namespace PluginSystem

// The macros Q_EXPORT_PLUGIN, Q_EXPORT_PLUGIN2 become obsolete in Qt 5.
#if QT_VERSION >= 0x050000
#    if defined(Q_EXPORT_PLUGIN)
#        undef Q_EXPORT_PLUGIN
#        undef Q_EXPORT_PLUGIN2
#    endif
#    define Q_EXPORT_PLUGIN(plugin)
#    define Q_EXPORT_PLUGIN2(function, plugin)
#else
#    define Q_PLUGIN_METADATA(x)
#endif

#endif // PLUGIN_H
