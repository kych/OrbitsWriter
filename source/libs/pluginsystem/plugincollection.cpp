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

#include "plugincollection.h"

using namespace PluginSystem;

PluginCollection::PluginCollection(const QString& name) :
    m_name(name)
{
}

PluginCollection::~PluginCollection()
{
}

QString PluginCollection::name() const
{
    return m_name;
}

void PluginCollection::addPlugin(PluginSpec *spec)
{
    m_plugins.append(spec);
}

void PluginCollection::removePlugin(PluginSpec *spec)
{
    m_plugins.removeOne(spec);
}

QList<PluginSpec *> PluginCollection::plugins() const
{
    return m_plugins;
}
