QT = core
QT += serialport
QT += network

CONFIG += console
CONFIG -= app_bundle

TARGET = serais
TEMPLATE = app

HEADERS += \
    serialportreader.h

SOURCES += \
    main.cpp \
    serialportreader.cpp

OTHER_FILES += \
    setting.ini
