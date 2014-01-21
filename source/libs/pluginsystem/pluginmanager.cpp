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

#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QFileInfoList>
#include <QMutex>
#include <QTime>

#include "plugincollection.h"
#include "pluginmanager.h"
#include "pluginmanager_p.h"
#include "pluginspec.h"
#include "pluginspec_p.h"

/*!
  \namespace PluginSystem
  The PluginSystem namespace provides classes that belong to the core plugin system.

  The basic plugin system contains the plugin manager and its supporting classes,
  and the Plugin interface that must be implemented by plugin providers.
 */

using namespace PluginSystem;
using namespace PluginSystem::Internal;

static bool lessThanByPluginName(const PluginSpec *one, const PluginSpec *two)
{
    return one->name() < two->name();
}

// ========== PluginManager ========== //

/*!
  \class PluginManager
  Managers for plugins.
 */

GET_INSTANCE(PluginManager)

/*!
  Constructs an instance of PluginManager.
 */
PluginManager::PluginManager() :
    d(new Internal::PluginManagerPrivate(this))
{
}

/*!
  Destroys the instance of PluginManager.
 */
PluginManager::~PluginManager()
{
    delete d;
    d = 0;
}

/*!
  Tries to load all the plugins that were previously found
  when setting the plugin search paths.

  The plugin specs of the plugins can be used to retrieve error
  and state information about individual plugins.
 */
void PluginManager::loadPlugins() const
{
    d->loadPlugins();
}

/*!
  Unload and deletes all plugins.
 */
void PluginManager::unloadPlugins() const
{
    d->unloadPlugins();
}

/*!
  List of all plugin specifications that have been found in the plugin search paths.

  The plugin specifications contain the information from the plugins'
  xml description files and the current state of the plugins.
  If a plugin's library has been already successfully loaded,
  the plugin specification has a reference to the created plugin instance as well.
 */
QList<PluginSpec *> PluginManager::plugins() const
{
    return d->pluginSpecs;
}

/*!
  Returns true if any plugin has errors even though it is enabled.

  Most useful to call after loadPlugins().
 */
bool PluginManager::hasError() const
{
    foreach (PluginSpec *spec, plugins()) {
        // only show errors on startup if plugin is enabled.
        if (spec->hasError() && spec->isEnabled() && !spec->isDisabledIndirectly()) {
            return true;
        }
    }
    return false;
}

/*!
  The plugin collections.
 */
QHash<QString, PluginCollection *> PluginManager::pluginCollections() const
{
    return d->pluginCategories;
}

/*!
  Sets the plugin search paths, i.e. the file system paths where the plugin manager
  looks for plugin descriptions.

  All given \a paths and their sub directory trees are searched for plugin xml description files.
 */
void PluginManager::setPluginPaths(const QStringList &paths)
{
    d->setPluginPaths(paths);
}

// ========== PluginSystem::Internal::PluginManagerPrivate ========== //

/*!
  \class PluginSystem::Internal::PluginManagerPrivate
  The PluginManagerPrivate class is the private implementation of PluginManager.
 */

PluginManagerPrivate::PluginManagerPrivate(PluginManager *pluginManager) :
    extension(QLatin1String("spec")),
    delayedInitializeTimer(0),
    shutdownEventLoop(0),
    q(pluginManager)
{
#ifdef QT_DEBUG
    initProfiling();
#endif
}

PluginManagerPrivate::~PluginManagerPrivate()
{
    qDeleteAll(pluginSpecs);
    qDeleteAll(pluginCategories);
}

void PluginManagerPrivate::loadPlugins()
{
    QList<PluginSpec *> queue = loadQueue();
    foreach (PluginSpec *spec, queue) {
        loadPlugin(spec, PluginSpec::Loaded);
    }
    foreach (PluginSpec *spec, queue) {
        loadPlugin(spec, PluginSpec::Initialized);
    }
    QListIterator<PluginSpec *> it(queue);
    it.toBack();
    while (it.hasPrevious()) {
        PluginSpec *spec = it.previous();
        loadPlugin(spec, PluginSpec::Running);
        if (spec->state() == PluginSpec::Running) {
            delayedInitializeQueue.append(spec);
        }
    }
    emit q->pluginsChanged();

//    delayedInitializeTimer = new QTimer;
//    delayedInitializeTimer->setInterval(DELAYED_INITIALIZE_INTERVAL);
//    delayedInitializeTimer->setSingleShot(true);
//    connect(delayedInitializeTimer, SIGNAL(timeout()),
//            this, SLOT(nextDelayedInitialize()));
    //    delayedInitializeTimer->start();
}

void PluginManagerPrivate::unloadPlugins()
{
    stopAll();
    if (!asynchronousPlugins.isEmpty()) {
        shutdownEventLoop = new QEventLoop;
        shutdownEventLoop->exec();
    }
    deleteAll();
//    if (!allObjects.isEmpty()) {
//        qDebug() << "There are" << allObjects.size() << "objects left in the plugin manager pool: " << allObjects;
//    }
}

void PluginManagerPrivate::stopAll()
{
//    if (delayedInitializeTimer && delayedInitializeTimer->isActive()) {
//        delayedInitializeTimer->stop();
//        delete delayedInitializeTimer;
//        delayedInitializeTimer = 0;
//    }
    QList<PluginSpec *> queue = loadQueue();
    foreach (PluginSpec *spec, queue) {
        loadPlugin(spec, PluginSpec::Stopped);
    }
}

void PluginManagerPrivate::deleteAll()
{
    QList<PluginSpec *> queue = loadQueue();
    QListIterator<PluginSpec *> it(queue);
    it.toBack();
    while (it.hasPrevious()) {
        loadPlugin(it.previous(), PluginSpec::Deleted);
    }
}

void PluginManagerPrivate::readPluginPaths()
{
    qDeleteAll(pluginCategories);
    qDeleteAll(pluginSpecs);
    pluginSpecs.clear();
    pluginCategories.clear();

    QStringList specFiles;
    QStringList searchPaths = pluginPaths;
    while (!searchPaths.isEmpty()) {
        const QDir dir(searchPaths.takeFirst());
        const QString pattern = QLatin1String("*.") + extension;
        const QFileInfoList files = dir.entryInfoList(QStringList(pattern), QDir::Files);
        foreach (const QFileInfo &file, files) {
            specFiles << file.absoluteFilePath();
        }
        const QFileInfoList dirs = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
        foreach (const QFileInfo &subdir, dirs) {
            searchPaths << subdir.absoluteFilePath();
        }
    }
    defaultCollection = new PluginCollection(QString());
    pluginCategories.insert("", defaultCollection);

    foreach (const QString &specFile, specFiles) {
        PluginSpec *spec = new PluginSpec;
        spec->d->read(specFile);

        PluginCollection *collection = 0;
        // find correct plugin collection or create a new one
        if (pluginCategories.contains(spec->category())) {
            collection = pluginCategories.value(spec->category());
        } else {
            collection = new PluginCollection(spec->category());
            pluginCategories.insert(spec->category(), collection);
        }
        if (defaultDisabledPlugins.contains(spec->name())) {
            spec->setDisabledByDefault(true);
            spec->setEnabled(false);
        }
        if (spec->isDisabledByDefault() && forceEnabledPlugins.contains(spec->name())) {
            spec->setEnabled(true);
        }
        if (!spec->isDisabledByDefault() && disabledPlugins.contains(spec->name())) {
            spec->setEnabled(false);
        }

        collection->addPlugin(spec);
        pluginSpecs.append(spec);
    }
    resolveDependencies();
    // ensure deterministic plugin load order by sorting
    qSort(pluginSpecs.begin(), pluginSpecs.end(), lessThanByPluginName);
    emit q->pluginsChanged();
}

void PluginManagerPrivate::resolveDependencies()
{
    foreach (PluginSpec *spec, pluginSpecs) {
        spec->d->resolveDependencies(pluginSpecs);
    }
    foreach (PluginSpec *spec, loadQueue()) {
        spec->d->disableIndirectlyIfDependencyDisabled();
    }
}

QList<PluginSpec *> PluginManagerPrivate::loadQueue()
{
    QList<PluginSpec *> queue;
    foreach (PluginSpec *spec, pluginSpecs) {
        QList<PluginSpec *> circularityCheckQueue;
        loadQueue(spec, queue, circularityCheckQueue);
    }
    return queue;
}

bool PluginManagerPrivate::loadQueue(PluginSpec *spec,
                                     QList<PluginSpec *> &queue,
                                     QList<PluginSpec *> &circularityCheckQueue)
{
    if (queue.contains(spec)) {
        return true;
    }
    // check for circular dependencies
    if (circularityCheckQueue.contains(spec)) {
        QString errStr = PluginManager::tr("Circular dependency detected:\n");
        int index = circularityCheckQueue.indexOf(spec);
        for (int i = index; i < circularityCheckQueue.size(); ++i) {
            errStr.append(PluginManager::tr("%1(%2) depends on\n")
                .arg(circularityCheckQueue.at(i)->name()).arg(circularityCheckQueue.at(i)->version()));
        }
        errStr.append(PluginManager::tr("%1(%2)").arg(spec->name()).arg(spec->version()));
        spec->d->setErrorString(errStr);
        return false;
    }
    circularityCheckQueue.append(spec);
    // check if we have the dependencies
    if (spec->state() == PluginSpec::Invalid || spec->state() == PluginSpec::Read) {
        queue.append(spec);
        return false;
    }

    // add dependencies
    foreach (PluginSpec *depSpec, spec->dependencySpecs()) {
        if (!loadQueue(depSpec, queue, circularityCheckQueue)) {
            spec->d->setErrorString(PluginManager::tr("Cannot load plugin %1(%2) because dependency failed to load: %3(%4)\nReason: %5")
                                    .arg(spec->name()).arg(spec->version())
                                    .arg(depSpec->name()).arg(depSpec->version())
                                    .arg(depSpec->errorString()));
            return false;
        }
    }

    // add self
    queue.append(spec);
    return true;
}

void PluginManagerPrivate::loadPlugin(PluginSpec *spec, PluginSpec::State destState)
{
    if (spec->hasError() || spec->state() != destState - 1) {
        return;
    }

    // don't load disabled plugins.
    if ((spec->isDisabledIndirectly() || !spec->isEnabled()) && destState == PluginSpec::Loaded) {
        return;
    }

    switch (destState) {
    case PluginSpec::Running:
        profilingReport(">initializePlugins", spec);
        spec->d->initializePlugins();
        profilingReport("<initializePlugins", spec);
        return;
    case PluginSpec::Deleted:
        profilingReport(">kill", spec);
        spec->d->kill();
        profilingReport("<kill", spec);
        return;
    default:
        break;
    }
    // check if dependencies have loaded without error
    QHashIterator<PluginDependency, PluginSpec *> it(spec->dependencySpecs());
    while (it.hasNext()) {
        it.next();
        if (it.key().type == PluginDependency::Optional) {
            continue;
        }
        PluginSpec *depSpec = it.value();
        if (depSpec->state() != destState) {
            spec->d->setErrorString(PluginManager::tr("Cannot load plugin %1(%2) because dependency failed to load: %3(%4)\nReason: %5")
                                    .arg(spec->name()).arg(spec->version())
                                    .arg(depSpec->name()).arg(depSpec->version())
                                    .arg(depSpec->errorString()));
            return;
        }
    }
    switch (destState) {
    case PluginSpec::Loaded:
        profilingReport(">load", spec);
        spec->d->load();
        profilingReport("<load", spec);
        break;
    case PluginSpec::Initialized:
        profilingReport(">initializePlugin", spec);
        spec->d->initializePlugin();
        profilingReport("<initializePlugin", spec);
        break;
    case PluginSpec::Stopped:
        profilingReport(">stop", spec);
        if (spec->d->stop() == Plugin::AsynchronousShutdown) {
            asynchronousPlugins << spec;
            connect(spec->plugin(), SIGNAL(asynchronousShutdownFinished()),
                    this, SLOT(asyncShutdownFinished()));
        }
        profilingReport("<stop", spec);
        break;
    default:
        break;
    }
}

PluginSpec *PluginManagerPrivate::pluginByName(const QString &name) const
{
    foreach (PluginSpec *spec, pluginSpecs) {
        if (spec->name() == name) {
            return spec;
        }
    }
    return 0;
}

void PluginManagerPrivate::setPluginPaths(const QStringList &paths)
{
    pluginPaths = paths;
//    readSettings();
    readPluginPaths();
}

void PluginManagerPrivate::initProfiling()
{
    if (profileTimer.isNull()) {
        profileTimer.reset(new QTime);
        profileTimer->start();
        profileElapsedMS = 0;
        qDebug("Profiling started");
    } else {
        profilingVerbosity++;
    }
}

void PluginManagerPrivate::profilingReport(const char *what, const PluginSpec *spec)
{
    if (!profileTimer.isNull()) {
        const int absoluteElapsedMS = profileTimer->elapsed();
        const int elapsedMS = absoluteElapsedMS - profileElapsedMS;
        profileElapsedMS = absoluteElapsedMS;
        if (spec) {
            qDebug("%-22s %-22s %8dms (%8dms)", what, qPrintable(spec->name()), absoluteElapsedMS, elapsedMS);
        } else {
            qDebug("%-45s %8dms (%8dms)", what, absoluteElapsedMS, elapsedMS);
        }
    }
}

void PluginManagerPrivate::nextDelayedInitialize()
{
//    while (!delayedInitializeQueue.isEmpty()) {
//        PluginSpec *spec = delayedInitializeQueue.takeFirst();
//        profilingReport(">delayedInitialize", spec);
//        bool delay = spec->d->delayedInitialize();
//        profilingReport("<delayedInitialize", spec);
//        if (delay)
//            break; // do next delayedInitialize after a delay
//    }
//    if (delayedInitializeQueue.isEmpty()) {
//        delete delayedInitializeTimer;
//        delayedInitializeTimer = 0;
//        emit q->initializationDone();
//    } else {
//        delayedInitializeTimer->start();
//    }
}

void PluginManagerPrivate::asyncShutdownFinished()
{
    Plugin *plugin = qobject_cast<Plugin *>(sender());
    Q_ASSERT(plugin);
    asynchronousPlugins.removeAll(plugin->pluginSpec());
    if (asynchronousPlugins.isEmpty()) {
        shutdownEventLoop->exit();
    }
}

/*!
  \fn void PluginManager::pluginsChanged()
  Emits when the list of available plugins has changed.
 */
