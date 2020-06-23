# Tells build system that project uses Qt Core and Qt GUI modules.
QT       += core gui

# Prior to Qt 5 widgets were part of Qt GUI module. In Qt 5 we need to add Qt Widgets module.
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Specifies name of the binary.
TARGET = myapp

# Denotes that project is an application.
TEMPLATE = app

# List of source files (note: Qt Creator will take care about this list, you don't need to update is manually).
SOURCES += main.cpp\
        mainwindow.cpp

# List of header files (note: Qt Creator will take care about this list).
HEADERS  += mainwindow.h

# List of "ui" files for a tool called Qt Designer, which is embedded into Qt Creator in newer versions of IDE (note: Qt Creator will take care about this list).
FORMS    += mainwindow.ui
