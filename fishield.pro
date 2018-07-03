TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    fishield.cpp \
    fs_scheduler.cpp \
    protobuf/file_transfer.pb.cc

HEADERS += \
    fishield.h \
    fs_scheduler.h \
    fs_stream_item.h \
    protobuf/file_transfer.pb.h

DISTFILES += \
    Makefile \
    protobuf/file_transfer.proto


LIBS += -lboost_system
LIBS += -lpthread
