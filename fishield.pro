TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    fishield.cpp \
    file_transfer.pb.cc \
    fs_client.cpp \
    main_client.cpp \
    fs_server.cpp

HEADERS += \
    fishield.h \
    file_transfer.pb.h \
    fs_client.h \
    fs_error.h \
    fs_server.h

DISTFILES += \
    Makefile \
    file_transfer.proto


LIBS += -lboost_system -lboost_thread
LIBS += -lpthread
LIBS += -lprotobuf

