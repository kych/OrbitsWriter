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

class PLUGINSYSTEM_EXPORT Plugin : public QObject
{
    Q_OBJECT
public:
    enum ShutdownFlag {
        SynchronousShutdown,
        AsynchronousShutdown
    };

    Plugin();
    ~Plugin();

    virtual bool initialize(const QStringList &arguments, QString *errorString) = 0;
    virtual void dependenciesInitialized() = 0;

    virtual ShutdownFlag aboutToShutdown()
    {
        return SynchronousShutdown;
    }

    PluginSpec *pluginSpec() const;

    void addObject(QObject *obj);
    void addAutoReleasedObject(QObject *obj);
    void removeObject(QObject *obj);

signals:
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
