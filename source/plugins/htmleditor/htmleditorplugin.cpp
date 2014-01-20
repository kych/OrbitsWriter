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

#include <QtPlugin>

#include "htmleditorfactory.h"
#include "htmleditorplugin.h"

namespace HtmlEditor
{
namespace Internal
{
class HtmlEditorPluginPrivate
{
}; // end of class HtmlEditor::Internal::HtmlEditorPluginPrivate
} // end of namespace HtmlEditor::Internal

HtmlEditorPlugin::HtmlEditorPlugin() :
    d(new Internal::HtmlEditorPluginPrivate)
{
}

HtmlEditorPlugin::~HtmlEditorPlugin()
{
    delete d;
}

bool HtmlEditorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    addAutoReleasedObject(new HtmlEditorFactory(this));
    return true;
}

void HtmlEditorPlugin::dependenciesInitialized()
{
}

} // end of namespace HtmlEditor

Q_EXPORT_PLUGIN2(HtmlEditorPlugin, HtmlEditor::HtmlEditorPlugin)
