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

// ========== PluginSystem::Plugin ========== //

Plugin::Plugin() :
    d(new PluginPrivate(this))
{
}

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

PluginSpec *Plugin::pluginSpec() const
{
    return d->pluginSpec;
}

void Plugin::addObject(QObject *obj)
{
    d->objectPool->addObject(obj);
}

void Plugin::addAutoReleasedObject(QObject *obj)
{
    d->addedObjectsInReverseOrder.prepend(obj);
    d->objectPool->addObject(obj);
}

void Plugin::removeObject(QObject *obj)
{
    d->objectPool->removeObject(obj);
}

// ========== PluginSystem::Internal::PluginPrivate ========== //

PluginPrivate::PluginPrivate(Plugin *plugin) :
    objectPool(Commons::ObjectPool::instance()),
    q(plugin)
{
}
