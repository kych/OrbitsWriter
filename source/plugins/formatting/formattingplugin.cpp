/*-------------------------------------------------
 *
 * OrbitsWriter - an Offline Blog Writer
 *
 * Copyright (C) 2014 devbean@galaxyworld.org
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

#include "formattingplugin.h"

namespace Formatting
{
namespace Internal
{
class FormattingPluginPrivate
{
}; // end of class Formatting::Internal::FormattingPluginPrivate
} // end of namespace Formatting::Internal

FormattingPlugin::FormattingPlugin() :
    d(new Internal::FormattingPluginPrivate)
{
}

FormattingPlugin::~FormattingPlugin()
{
    delete d;
}

bool FormattingPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    return true;
}

void FormattingPlugin::dependenciesInitialized()
{

}

} // end of namespace Formatting
