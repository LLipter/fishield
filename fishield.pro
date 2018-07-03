TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    fishield.cpp \
    fs_scheduler.cpp

HEADERS += \
    fishield.h \
    fs_scheduler.h \
    fs_stream_item.h

DISTFILES += \
    Makefile


LIBS += -lboost_system
LIBS += -lpthread
