QT       += core
QT       += network
QT       += gui
QT       +=xml

TARGET = http-image-server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    httpprotocol.cpp \
    startimagewebserver.cpp \
    imagewebserver.cpp \
    connection.cpp \
    iwsconfig.cpp \
    htmlpage.cpp \
    gethandler.cpp \
    pagemodel.cpp \
    item.cpp

HEADERS += \
    httpprotocol.h \
    imagewebserver.h \
    connection.h \
    iwsconfig.h \
    htmlpage.h \
    gethandler.h \
    pagemodel.h \
    item.h
