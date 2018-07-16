TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    fishield.cpp \
    file_transfer.pb.cc \
    fs_client.cpp \
    fs_server.cpp \
    fs_scheduler.cpp \
    fs_task.cpp \
    fs_task.pb.cc \
    main_client.cpp \
    fs_dbmanager.cpp

HEADERS += \
    fishield.h \
    file_transfer.pb.h \
    fs_client.h \
    fs_server.h \
    fs_task.h \
    fs_scheduler.h \
    fs_config.h \
    fs_task.pb.h \
    fs_dbmanager.h \
    fs_user.h

DISTFILES += \
    Makefile \
    file_transfer.proto


LIBS += -lboost_system -lboost_thread -lboost_filesystem
LIBS += -lpthread
LIBS += -lprotobuf
LIBS += -lmysqlcppconn
