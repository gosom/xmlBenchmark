TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

INCLUDEPATH += "/home/giorgos/include"
LIBS += -lcrypto

SOURCES += main.cpp

HEADERS += \
    myhash.h

