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

#ifndef PLUGINSPEC_H
#define PLUGINSPEC_H

#include <QString>

#include "pluginsystem/pluginsystem_global.h"

QT_BEGIN_NAMESPACE
template<typename K, typename V> class QHash;
QT_END_NAMESPACE

namespace PluginSystem
{

class Plugin;

namespace Internal
{
class PluginSpecPrivate;
class PluginManagerPrivate;
} // end of namespace PluginSystem::Internal

struct PluginDependency
{
    enum Type
    {
        Required,
        Optional
    };

    PluginDependency() : type(Required) {}

    QString name;
    QString version;
    Type type;

    bool operator==(const PluginDependency &other) const;
}; // end of class PluginSystem::PluginDependency

uint qHash(const PluginDependency &value);

class PLUGINSYSTEM_EXPORT PluginSpec
{
public:
    /*!
      The state gives a hint on what went wrong in case of an error.
     */
    enum State
    {
        Invalid,
        Read,
        Resolved,
        Loaded,
        Initialized,
        Running,
        Stopped,
        Deleted
    };

    ~PluginSpec();

    // ---------- plugin properties ---------- //
    QString name() const;
    QString version() const;
    QString compatVersion() const;
    QString vendor() const;
    QString copyright() const;
    QString license() const;
    QString category() const;
    QString description() const;
    QString url() const;
    QList<PluginDependency> dependencyList() const;

    // ---------- error ---------- //
    bool hasError() const;
    QString errorString() const;

    // ---------- extend information ---------- //
    State state() const;
    QString location() const;
    QString filePath() const;

    bool provides(const QString &pluginName, const QString &version) const;
    QHash<PluginDependency, PluginSpec *> dependencySpecs() const;
    Plugin *plugin() const;

    bool isDisabledByDefault() const;
    bool isEnabled() const;
    bool isDisabledIndirectly() const;
    void setEnabled(bool value);
    void setDisabledByDefault(bool value);
    void setDisabledIndirectly(bool value);

private:
    PluginSpec();

    Internal::PluginSpecPrivate *d;
    friend class Internal::PluginSpecPrivate;
    friend class Internal::PluginManagerPrivate;
}; // end of class PluginSystem::PluginSpec

} // end of namespace PluginSystem

#endif // PLUGINSPEC_H
