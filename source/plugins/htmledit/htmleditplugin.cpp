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

#include <QMainWindow>
#include <QtPlugin>

#include <core/appcore.h>

#include "htmleditplugin.h"
#include "htmleditwidget.h"
#include "htmleditorfactory.h"

namespace HtmlEdit
{
namespace Internal
{
class HtmlEditPluginPrivate
{
}; // end of class HtmlEdit::Internal::HtmlEditPluginPrivate
} // end of namespace HtmlEdit::Internal

HtmlEditPlugin::HtmlEditPlugin() :
    d(new Internal::HtmlEditPluginPrivate)
{
}

HtmlEditPlugin::~HtmlEditPlugin()
{
    delete d;
}

bool HtmlEditPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);
    addAutoReleasedObject(new HtmlEditorFactory(this));
    return true;
}

void HtmlEditPlugin::dependenciesInitialized()
{
}

} // end of namespace HtmlEdit

Q_EXPORT_PLUGIN2(HtmlEditPlugin, HtmlEdit::HtmlEditPlugin)
