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
TARGET   = HtmlEdit
DEFINES += HTMLEDIT_LIBRARY

QT      += core gui webkit
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

include(../../plugin.pri)
include(htmledit_dependencies.pri)

HEADERS += \
    htmledit_global.h \
    htmleditplugin.h \
    htmleditwidget.h \
    htmleditconstants.h \
    sourceedit.h \
    htmleditor.h \
    htmleditorfactory.h

SOURCES += \
    htmleditplugin.cpp \
    htmleditwidget.cpp \
    sourceedit.cpp \
    htmleditor.cpp \
    htmleditorfactory.cpp

OTHER_FILES += \
    HtmlEdit.spec.in
