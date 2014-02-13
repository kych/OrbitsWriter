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
#include <QStringList>

#include "coreplugin.h"
#include "mainwindow.h"

namespace Core {

namespace Internal {

class CorePluginPrivate
{
public:
    MainWindow *mainWindow;
}; // end of class CorePluginPrivate
}

CorePlugin::CorePlugin() :
    d(new Internal::CorePluginPrivate)
{
    d->mainWindow = new MainWindow;
}

CorePlugin::~CorePlugin()
{
}

bool CorePlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    const bool success = d->mainWindow->init(errorString);
    return success;
}

void CorePlugin::dependenciesInitialized()
{
    d->mainWindow->prepareToShow();
}

}

Q_EXPORT_PLUGIN2(CorePlugin, Core::CorePlugin)
