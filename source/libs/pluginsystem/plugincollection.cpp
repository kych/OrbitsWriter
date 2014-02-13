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

#include "plugincollection.h"

/*!
  \class PluginCollection
  The plugin collection for a category.
 */

using namespace PluginSystem;

/*!
  Constructs an instance of PluginCollection with category name \a name.
 */
PluginCollection::PluginCollection(const QString& name) :
    m_name(name)
{
}

/*!
  Destroys the instance of this colletion.
 */
PluginCollection::~PluginCollection()
{
}

/*!
  The plugin category name.
 */
QString PluginCollection::name() const
{
    return m_name;
}

/*!
  Adds plugin \a spec to this collection.
 */
void PluginCollection::addPlugin(PluginSpec *spec)
{
    m_plugins.append(spec);
}

/*!
  Removes the plugin \a spec to this collection.
 */
void PluginCollection::removePlugin(PluginSpec *spec)
{
    m_plugins.removeOne(spec);
}

/*!
  All plugins of this collection.
 */
QList<PluginSpec *> PluginCollection::plugins() const
{
    return m_plugins;
}
