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

#ifndef PLUGINCOLLECTION_H
#define PLUGINCOLLECTION_H

#include <QString>
#include <QList>

namespace PluginSystem
{

class PluginSpec;

/*!
  The plugin collection for a category.
 */
class PluginCollection
{
public:
    /*!
      Constructs an instance of PluginCollection with category name \a name.
     */
    explicit PluginCollection(const QString& name);

    /*!
      Destroys the instance of this colletion.
     */
    ~PluginCollection();

    /*!
      The plugin category name.
     */
    QString name() const;

    /*!
      Adds plugin \a spec to this collection.
     */
    void addPlugin(PluginSpec *spec);

    /*!
      Removes the plugin \a spec to this collection.
     */
    void removePlugin(PluginSpec *spec);

    /*!
      All plugins of this collection.
     */
    QList<PluginSpec *> plugins() const;

private:
    QString m_name;
    QList<PluginSpec *> m_plugins;
}; // end of class PluginSystem::PluginCollection

} // end of namespace PluginSystem

#endif // PLUGINCOLLECTION_H
