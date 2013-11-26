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

include(../../OrbitsWriter.pri)

TEMPLATE = app
TARGET   = $$APPLICATION_TARGET
DESTDIR  = $$APPLICATION_BIN_PATH

include(../rpath.pri)
include(../libs/extern/QtSingleApplication/qtsingleapplication.pri)

QT      *= core gui

LIBS    *= -l$$libraryName(core)

INCLUDEPATH += $$PWD/../libs/core

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

app_info.input          = $$PWD/version.h.in
app_info.output         = $$OUT_PWD/version.h
app_info_include.input  = $$PWD/version.in
app_info_include.output = $$OUT_PWD/Version
QMAKE_SUBSTITUTES      += app_info app_info_include
