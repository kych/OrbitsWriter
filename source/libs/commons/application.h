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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>

#include "../extern/qtsingleapplication/qtsingleapplication.h"
#include "commons_global.h"

namespace Commons
{

class COMMONS_EXPORT Application : public ExternLib::QtSingleApplication
{
    Q_OBJECT
public:
    Application(const QString &id, int &argc, char **argv);
}; // end of class Commons::Application

} // end of namespace Commons

#endif // APPLICATION_H
