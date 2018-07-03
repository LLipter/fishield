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
    protobuf/file_transfer.pb.h \
    fs_task_info.h

DISTFILES += \
    Makefile \
    protobuf/file_transfer.proto


LIBS += -lboost_system
LIBS += -lpthread
LIBS += -lprotobuf
