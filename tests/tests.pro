include(../defaults.pri)

QT -= qt core gui

CONFIG   -= app_bundle
CONFIG += c++14 console

LIBS += -L../../libKitsuneCommon/src -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/debug -lKitsuneCommon
LIBS += -L../../libKitsuneCommon/src/release -lKitsuneCommon
INCLUDEPATH += ../../libKitsuneCommon/include/libKitsuneCommon

INCLUDEPATH += $$PWD

LIBS += -L../src -lKitsuneIni

SOURCES += \
        main.cpp \
    libKitsuneIni/iniItem_test.cpp

HEADERS += \
    libKitsuneIni/iniItem_test.hpp

