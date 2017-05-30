QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = screenshoter
TEMPLATE = app


SOURCES += main.cpp\
    screenshoter.cpp \
    spysettings.cpp

HEADERS  += screenshoter.h \
    spysettings.h

RESOURCES = screenshoter.qrc
