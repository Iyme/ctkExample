TEMPLATE = lib
TARGET = MainWindow
DESTDIR = $$PWD/../bin/plugins/$$TARGET
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#加载ctk库
LIBS += -L$$PWD/../libs/ -lCTKCore
LIBS += -L$$PWD/../libs/ -lCTKPluginFramework

INCLUDEPATH += $$PWD/../inclues \
        += $$PWD/../includes/core \
        += $$PWD/../includes/pluginFramework

INCLUDEPATH += $$PWD/../libs
DEPENDPATH += $$PWD/../libs

HEADERS += \
    mainwindowactivator.h \
    mainwindowplugin.h \
    ../includes/imainwindow.h \
    mainwindowdlg.h

SOURCES += \
    mainwindowactivator.cpp \
    mainwindowplugin.cpp \
    mainwindowdlg.cpp

RESOURCES += \
    mainwindow.qrc

file.path = $$DESTDIR
file.files = MANIFEST.MF

INSTALLS += file

FORMS += \
    mainwindowdlg.ui
