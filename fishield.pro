TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    fishield.cpp \
    fs_scheduler.cpp \
    protobuf/file_transfer.pb.cc \
    fs_task.cpp \
    fs_callback.cpp \
    fs_client.cpp \
    fs_error.cpp

HEADERS += \
    fishield.h \
    fs_scheduler.h \
    protobuf/file_transfer.pb.h \
    fs_task_info.h \
    fs_task.h \
    fs_callback.h \
    fs_client.h \
    fs_config.h \
    fs_error.h

DISTFILES += \
    Makefile \
    protobuf/file_transfer.proto


LIBS += -lboost_system
LIBS += -lpthread
LIBS += -lprotobuf
LIBS += -lssl -lcrypto
