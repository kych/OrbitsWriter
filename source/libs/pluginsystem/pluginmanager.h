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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>

#include <commons/singleton.h>

#include "pluginsystem/pluginsystem_global.h"

QT_BEGIN_NAMESPACE
template<typename K, typename V> class QHash;
QT_END_NAMESPACE

/*!
  The PluginSystem namespace provides classes that belong to the core plugin system.

  The basic plugin system contains the plugin manager and its supporting classes,
  and the Plugin interface that must be implemented by plugin providers.
 */
namespace PluginSystem
{

class PluginSpec;
class PluginCollection;

namespace Internal
{
class PluginManagerPrivate;
} // end of namespace PluginSystem::Internal

/*!
  Managers for plugins.
 */
class PLUGINSYSTEM_EXPORT PluginManager : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(PluginManager)
public:
    /*!
      Tries to load all the plugins that were previously found
      when setting the plugin search paths.

      The plugin specs of the plugins can be used to retrieve error
      and state information about individual plugins.
     */
    void loadPlugins() const;

    /*!
      List of all plugin specifications that have been found in the plugin search paths.

      The plugin specifications contain the information from the plugins' xml description files
      and the current state of the plugins. If a plugin's library has been already successfully loaded,
      the plugin specification has a reference to the created plugin instance as well.
     */
    QList<PluginSpec *> plugins() const;

    /*!
      The plugin collections.
     */
    QHash<QString, PluginCollection *> pluginCollections() const;

    /*!
      Returns true if any plugin has errors even though it is enabled.

      Most useful to call after loadPlugins().
     */
    bool hasError() const;

    /*!
      Sets the plugin search paths, i.e. the file system paths where the plugin manager
      looks for plugin descriptions.

      All given \a paths and their sub directory trees are searched for plugin xml description files.
     */
    void setPluginPaths(const QStringList &paths);

public slots:
    /*!
      Unload and deletes all plugins.
     */
    void unloadPlugins() const;

signals:
    /*!
      Emits when the list of available plugins has changed.
     */
    void pluginsChanged();

private:
    //! Constructs an instance of PluginManager.
    PluginManager();
    //! Destroys the instance of PluginManager.
    ~PluginManager();

    Internal::PluginManagerPrivate *d;
    friend class Internal::PluginManagerPrivate;
}; // end of class PluginSystem::PluginManager

} // end of namespace PluginSystem

#endif // PLUGINMANAGER_H
