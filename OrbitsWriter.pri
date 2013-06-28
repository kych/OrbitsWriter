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

!isEmpty(ORBITSWRITER_PRI_INCLUDED): error("orbitswriter.pri already included")
ORBITSWRITER_PRI_INCLUDED = 1

APPLICATION_NAME    = OrbitsWriter
APPLICATION_VERSION = 0.0.1

isEqual(QT_MAJOR_VERSION, 5) {

    defineReplace(cleanPath) {
        return($$clean_path($$1))
    }

    defineReplace(targetPath) {
        return($$shell_path($$1))
    }

} else { # if not Qt5

    defineReplace(cleanPath) {
        win32:1 ~= s|\\\\|/|g
        contains(1, ^/.*):pfx = /
        else:pfx =
        segs = $$split(1, /)
        out =
        for(seg, segs) {
            equals(seg, ..):out = $$member(out, 0, -2)
            else:!equals(seg, .):out += $$seg
        }
        return($$join(out, /, $$pfx))
    }

    defineReplace(targetPath) {
        return($$replace(1, /, $$QMAKE_DIR_SEP))
    }

}

defineReplace(libraryName) {
   unset(LIBRARY_NAME)
   LIBRARY_NAME = $$1
   CONFIG(debug, debug|release) {
      !debug_and_release|build_pass {
          mac:RET = $$member(LIBRARY_NAME, 0)_debug
              else:win32:RET = $$member(LIBRARY_NAME, 0)d
      }
   }
   isEmpty(RET):RET = $$LIBRARY_NAME
   return($$RET)
}

isEmpty(LIBRARY_BASENAME) {
    LIBRARY_BASENAME = libs
}

SOURCE_TREE = $$PWD
isEmpty(BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    BUILD_TREE = $$cleanPath($$OUT_PWD)
    BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}
OUTPUT_PATH = $$BUILD_TREE/output

!macx {
    contains(TEMPLATE, vc.*): vcproj = 1
    APPLICATION_TARGET           = OrbitsWriter
    APPLICATION_LIBRARY_PATH     = $$BUILD_TREE/$$LIBRARY_BASENAME
    APPLICATION_COMPONENTS_PATH  = $$OUTPUT_PATH/plugins
    #APP_LIBEXEC_PATH            = $$APP_PATH # FIXME
    #APP_DATA_PATH               = $$APP_BUILD_TREE/share/orbitswriter
    #APP_DOC_PATH                = $$APP_BUILD_TREE/share/doc/orbitswriter
    APPLICATION_BIN_PATH         = $$OUTPUT_PATH
    APPLICATION_GUIXML_PATH      = $$APPLICATION_BIN_PATH
    !isEqual(SOURCE_TREE, $$BUILD_TREE):copydata = 1
} else {
    #APP_TARGET       = "Orbits Writer"
    #APP_LIBRARY_PATH = $$APP_PATH/$${APP_TARGET}.app/Contents/PlugIns
    #APP_PLUGIN_PATH  = $$APP_LIBRARY_PATH
    #APP_LIBEXEC_PATH = $$APP_PATH/$${APP_TARGET}.app/Contents/Resources
    #APP_DATA_PATH    = $$APP_PATH/$${APP_TARGET}.app/Contents/Resources
    #APP_DOC_PATH     = $$APP_DATA_PATH/doc
    #APP_BIN_PATH     = $$APP_PATH/$${APP_TARGET}.app/Contents/MacOS
    #copydata = 1
#    isEmpty(TIGER_COMPAT_MODE):TIGER_COMPAT_MODE=$$(QTC_TIGER_COMPAT)
    #isEmpty(TIGER_COMPAT_MODE) {
    #    QMAKE_CXXFLAGS *= -mmacosx-version-min=10.5
    #    QMAKE_LFLAGS *= -mmacosx-version-min=10.5
    #}
}

INCLUDEPATH += \
    $$SOURCE_TREE/source/components \
    $$BUILD_TREE/source/components \ # for <Application/Version>
    $$SOURCE_TREE/source/libs/GCF/include

CONFIG += depend_includepath c++11

LIBS += -L$$APPLICATION_LIBRARY_PATH

!isEmpty(vcproj) {
    DEFINES += LIBRARY_BASENAME=\"$$LIBRARY_BASENAME\"
} else {
    DEFINES += LIBRARY_BASENAME=\\\"$$LIBRARY_BASENAME\\\"
}

unix {
    CONFIG(debug, debug|release):OBJECTS_DIR = $${OUT_PWD}/.obj/debug-shared
    CONFIG(release, debug|release):OBJECTS_DIR = $${OUT_PWD}/.obj/release-shared

    CONFIG(debug, debug|release):MOC_DIR = $${OUT_PWD}/.moc/debug-shared
    CONFIG(release, debug|release):MOC_DIR = $${OUT_PWD}/.moc/release-shared

    RCC_DIR = $${OUT_PWD}/.rcc
    UI_DIR = $${OUT_PWD}/.uic
}

win32-msvc* {
    #Don't warn about sprintf, fopen etc being 'unsafe'
    DEFINES += _CRT_SECURE_NO_WARNINGS
} else {
    QMAKE_CXXFLAGS += -std=c++0x
}

QMAKEFEATURES += $$SOURCE_TREE/libs/GCF/qmake_prf

CONFIG(debug, debug|release):DEFINES += _DEBUG_
