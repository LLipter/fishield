TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    fishield.cpp \
    fs_scheduler.cpp \
    file_transfer.pb.cc \
    fs_client.cpp \
    fs_error.cpp \
    main_client.cpp

HEADERS += \
    fishield.h \
    fs_scheduler.h \
    file_transfer.pb.h \
    fs_client.h \
    fs_error.h

DISTFILES += \
    Makefile \
    file_transfer.proto


LIBS += -lboost_system -lboost_thread
LIBS += -lpthread
LIBS += -lprotobuf

