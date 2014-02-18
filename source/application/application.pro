#-------------------------------------------------
#
# OrbitsWriter - an Offline Blog Writer
#
# Copyright (C) 2013 devbean@galaxyworld.org
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

include(../../orbitswriter.pri)

TEMPLATE = app
TARGET   = $$APPLICATION_TARGET
DESTDIR  = $$APPLICATION_BIN_PATH

include(../rpath.pri)
include(../libs/extern/qtsingleapplication/qtsingleapplication.pri)

QT      *= core gui

LIBS    *= -l$$libraryName(Commons) -l$$libraryName(PluginSystem)

SOURCES += \
    main.cpp

win32 {
    RC_FILE      = application.rc
    target.path  = /bin
    INSTALLS    += target
} else:macx {
    #TODO
} else {
    target.path  = /bin
    INSTALLS    += target
}

OTHER_FILES += application.rc \
    version.h.in

version.input          = $$PWD/version.h.in
version.output         = $$OUT_PWD/version.h
QMAKE_SUBSTITUTES     += version
