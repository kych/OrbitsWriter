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

TARGET   = core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../library.pri)
include(core_dependencies.pri)

HEADERS += \
    global.h \
    mainwindow.h \
    visualeditor.h \
    sourceeditor.h \
    previewer.h \
    fontchooser.h \
    fontsizechooser.h \
    colorbutton.h \
    dpointer.h

SOURCES += \
    mainwindow.cpp \
    visualeditor.cpp \
    sourceeditor.cpp \
    previewer.cpp \
    fontchooser.cpp \
    fontsizechooser.cpp \
    colorbutton.cpp \
    dpointer.cpp

RESOURCES += \
    resources.qrc
