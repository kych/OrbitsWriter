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

INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

SOURCES += \
    $$PWD/qtsingleapplication.cpp \
    $$PWD/qtlocalpeer.cpp \
    $$PWD/qtlockedfile.cpp

HEADERS += \
    $$PWD/qtsingleapplication.h \
    $$PWD/qtlocalpeer.h \
    $$PWD/qtlockedfile.h

unix:SOURCES  += $$PWD/qtlockedfile_unix.cpp
win32:SOURCES += $$PWD/qtlockedfile_win.cpp

QT *= network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
win32:contains(TEMPLATE, lib):contains(CONFIG, shared) {
    DEFINES += QT_QTSINGLEAPPLICATION_EXPORT=__declspec(dllexport)
    DEFINES += QT_QTLOCKEDFILE_EXPORT=__declspec(dllexport)
}
