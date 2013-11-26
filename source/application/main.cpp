/*-------------------------------------------------
 *
 * OrbitsWriter - An Offline Blog Writer
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

#include <QtSingleApplication>

#include <MainWindow>
#include <Version>

int main(int argc, char** argv)
{
    Extern::QtSingleApplication app(QLatin1String(Application::NAME), argc, argv);
    app.setOrganizationName(QLatin1String(Application::ORGANIZATION));
    app.setOrganizationDomain(QLatin1String(Application::ORGANIZATION_DOMAIN));
    app.setApplicationName(QLatin1String(Application::NAME));
    app.setApplicationVersion(QLatin1String(Application::VERSION_LONG));

    Orbits::MainWindow win;
    win.showMaximized();

    return app.exec();
}
