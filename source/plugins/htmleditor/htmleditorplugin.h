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

#ifndef HTMLEDITORPLUGIN_H
#define HTMLEDITORPLUGIN_H

#include <pluginsystem/plugin.h>

namespace HtmlEditor {

namespace Internal
{
class HtmlEditorPluginPrivate;
} // end of namespace HtmlEditor::Internal

class HtmlEditorPlugin : public PluginSystem::Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.galaxyworld.orbitswriter.plugin" FILE "HtmlEditor.json")
public:
    HtmlEditorPlugin();
    ~HtmlEditorPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void dependenciesInitialized();

private:
    Internal::HtmlEditorPluginPrivate *d;
    friend class Internal::HtmlEditorPluginPrivate;
}; // end of class HtmlEditor::HtmlEditorPlugin

} // end of namesapce HtmlEditor

#endif // HTMLEDITORPLUGIN_H
