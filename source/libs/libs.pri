include(../library.pri)

win32-msvc* {
    DEFINES    += -D_CRT_SECURE_NO_DEPRECATE
    CONFIG     += embed_manifest_exe warn_on dll
} else {
    DEFINES    += _CRT_SECURE_NO_DEPRECATE
    CONFIG     += warn_off
}
