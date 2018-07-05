TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    fishield.cpp \
    fs_scheduler.cpp \
    file_transfer.pb.cc \
    fs_task.cpp \
    fs_callback.cpp \
    fs_client.cpp \
    fs_error.cpp \
    fs_server.cpp \
    main_client.cpp

HEADERS += \
    fishield.h \
    fs_scheduler.h \
    file_transfer.pb.h \
    fs_task_info.h \
    fs_task.h \
    fs_callback.h \
    fs_client.h \
    fs_config.h \
    fs_error.h \
    fs_server.h

DISTFILES += \
    Makefile \
    protobuf/file_transfer.proto


LIBS += -lboost_system -lboost_thread
LIBS += -lpthread
LIBS += -lprotobuf
LIBS += -lssl -lcrypto
