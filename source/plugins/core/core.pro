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

TEMPLATE = lib
TARGET   = Core
DEFINES += CORE_LIBRARY

QT      *= core gui

include(../../plugin.pri)
include(core_dependencies.pri)

PRECOMPILED_HEADER = $$PWD/core_pch.h

HEADERS += \
    mainwindow.h \
    core_pch.h \
    coreplugin.h \
    id.h \
    actionsystem/command.h \
    actionsystem/command_p.h \
    actionsystem/proxyaction.h \
    actionsystem/actioncontainer.h \
    actionsystem/actioncontainer_p.h \
    actionsystem/actionmanager.h \
    actionsystem/actionmanager_p.h \
    context.h \
    appcore.h \
    appcore_p.h \
    corelistener.h \
    editorsystem/editor.h \
    editorsystem/editormanager.h \
    editorsystem/editorfactory.h \
    editorsystem/editormanager_p.h \
    plugindialog.h \
    coreconstants.h \
    documentsystem/documentmanager.h \
    documentsystem/documentmanager_p.h \
    documentsystem/document.h \
    documentsystem/document_p.h \
    core_export.h \
    documentsystem/documentcharstyle.h \
    documentsystem/documentparagraphstyle.h

SOURCES += \
    mainwindow.cpp \
    coreplugin.cpp \
    id.cpp \
    actionsystem/command.cpp \
    actionsystem/proxyaction.cpp \
    actionsystem/actioncontainer.cpp \
    actionsystem/actionmanager.cpp \
    context.cpp \
    appcore.cpp \
    corelistener.cpp \
    editorsystem/editor.cpp \
    editorsystem/editormanager.cpp \
    editorsystem/editorfactory.cpp \
    plugindialog.cpp \
    documentsystem/documentmanager.cpp \
    documentsystem/document.cpp \
    documentsystem/documentcharstyle.cpp \
    documentsystem/documentparagraphstyle.cpp

RESOURCES += \
    resources/resources.qrc

OTHER_FILES += \
    Core.spec.in

