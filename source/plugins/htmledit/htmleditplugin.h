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

#ifndef HTMLEDITPLUGIN_H
#define HTMLEDITPLUGIN_H

#include <pluginsystem/plugin.h>

namespace HtmlEdit {

namespace Internal
{
class HtmlEditPluginPrivate;
} // end of namespace HtmlEdit::Internal

class HtmlEditPlugin : public PluginSystem::Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.galaxyworld.orbitswriter.plugin" FILE "HtmlEdit.json")
public:
    HtmlEditPlugin();
    ~HtmlEditPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void dependenciesInitialized();

private:
    Internal::HtmlEditPluginPrivate *d;
    friend class Internal::HtmlEditPluginPrivate;
}; // end of class HtmlEdit::HtmlEditorPlugin

} // end of namesapce HtmlEdit

#endif // HTMLEDITPLUGIN_H
