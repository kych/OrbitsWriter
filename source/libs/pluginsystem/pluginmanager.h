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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>

#include <commons/singleton.h>

#include "pluginsystem/pluginsystem_global.h"

QT_BEGIN_NAMESPACE
template<typename K, typename V> class QHash;
QT_END_NAMESPACE

namespace PluginSystem
{

class PluginSpec;
class PluginCollection;

namespace Internal
{
class PluginManagerPrivate;
} // end of namespace PluginSystem::Internal

class PLUGINSYSTEM_EXPORT PluginManager : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(PluginManager)
public:
    void loadPlugins() const;

    QList<PluginSpec *> plugins() const;
    QHash<QString, PluginCollection *> pluginCollections() const;

    bool hasError() const;

    void setPluginPaths(const QStringList &paths);

public slots:
    void unloadPlugins() const;

signals:
    void pluginsChanged();

private:
    PluginManager();
    ~PluginManager();

    Internal::PluginManagerPrivate *d;
    friend class Internal::PluginManagerPrivate;
}; // end of class PluginSystem::PluginManager

} // end of namespace PluginSystem

#endif // PLUGINMANAGER_H
