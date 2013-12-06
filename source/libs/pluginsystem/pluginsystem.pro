#-------------------------------------------------
#
# OrbitsWriter - an Offline Blog Writer
#
# Copyright (C) 2012 devbean@galaxyworld.org
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#-------------------------------------------------

TEMPLATE = lib
TARGET   = PluginSystem
DEFINES += PLUGINSYSTEM_LIBRARY

include(../../library.pri)
include(pluginsystem_dependencies.pri)

unix:!macx:!freebsd*:LIBS += -ldl

HEADERS += \
    pluginmanager.h \
    pluginspec.h \
    pluginspec_p.h \
    pluginmanager_p.h \
    plugin.h \
    plugin_p.h \
    plugincollection.h \
    pluginsystem_global.h \
    pluginview.h

SOURCES += \
    pluginmanager.cpp \
    pluginspec.cpp \
    plugin.cpp \
    plugincollection.cpp \
    pluginview.cpp
