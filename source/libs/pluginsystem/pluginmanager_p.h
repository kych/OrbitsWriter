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

#ifndef PLUGINMANAGER_P_H
#define PLUGINMANAGER_P_H

#include <QObject>
#include <QHash>
#include <QStringList>

#include "pluginsystem/pluginspec.h"

QT_BEGIN_NAMESPACE
class QEventLoop;
class QTime;
class QTimer;
QT_END_NAMESPACE

namespace PluginSystem
{

class PluginManager;
class PluginCollection;

namespace Internal
{

/*!
  The PluginManagerPrivate class is the private implementation of PluginManager.
 */
class PluginManagerPrivate : public QObject
{
    Q_OBJECT
public:
    PluginManagerPrivate(PluginManager *pluginManager);
    virtual ~PluginManagerPrivate();

    void loadPlugins();
    void unloadPlugins();
    void resolveDependencies();
    QList<PluginSpec *> loadQueue();
    void loadPlugin(PluginSpec *spec, PluginSpec::State destState);

    PluginSpec * pluginByName(const QString &name) const;

    QHash<QString, PluginCollection *> pluginCategories;

    void setPluginPaths(const QStringList &paths);

    QStringList pluginPaths;
    QString extension;
    QStringList defaultDisabledPlugins;
    QStringList disabledPlugins;
    QStringList forceEnabledPlugins;
    // delayed initialization
    QTimer *delayedInitializeTimer;
    QList<PluginSpec *> delayedInitializeQueue;
    // ansynchronous shutdown
    QList<PluginSpec *> asynchronousPlugins; // plugins that have requested async shutdown
    QEventLoop *shutdownEventLoop; // used for async shutdown

    // profiling
    void initProfiling();
    void profilingReport(const char *what, const PluginSpec *spec = 0);
    QList<PluginSpec *> pluginSpecs;
    QScopedPointer<QTime> profileTimer;
    int profileElapsedMS;
    unsigned profilingVerbosity;

private slots:
    void nextDelayedInitialize();
    void asyncShutdownFinished();

private:
    void stopAll();
    void deleteAll();
    void readPluginPaths();
    bool loadQueue(PluginSpec *spec,
                   QList<PluginSpec *> &queue,
                   QList<PluginSpec *> &circularityCheckQueue);

    PluginCollection *defaultCollection;
    PluginManager *q;
}; // end of class PluginSystem::Internal::PluginManagerPrivate

} // end of namespace PluginSystem::Internal

} // end of namespace PluginSystem

#endif // PLUGINMANAGER_P_H
