QT = core
QT += serialport
QT += network

CONFIG += console
CONFIG -= app_bundle

TARGET = serais
TEMPLATE = app

HEADERS += \
    serialportreader.h \
    log.h

SOURCES += \
    main.cpp \
    serialportreader.cpp \
    log.cpp

OTHER_FILES += \
    setting.ini
