QT += quick
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    backend.cpp \
    ../sdk/file_transfer.pb.cc \
    ../sdk/fishield.cpp \
    ../sdk/fs_client.cpp \
    ../sdk/fs_dbmanager.cpp \
    ../sdk/fs_scheduler.cpp \
    ../sdk/fs_server.cpp \
    ../sdk/fs_task.cpp \
    ../sdk/fs_task.pb.cc
RESOURCES += qml.qrc \
    icons/icons.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



# setting for papyros/qml-material
DEFINES += QPM_INIT\\(E\\)=\"E.addImportPath(QStringLiteral(\\\"qrc:/\\\"));\"
include(material/material.pri)
QML_IMPORT_PATH += material

HEADERS += \
    backend.h \
    ../sdk/file_transfer.pb.h \
    ../sdk/fishield.h \
    ../sdk/fs_client.h \
    ../sdk/fs_config.h \
    ../sdk/fs_dbmanager.h \
    ../sdk/fs_scheduler.h \
    ../sdk/fs_server.h \
    ../sdk/fs_task.h \
    ../sdk/fs_task.pb.h \
    ../sdk/fs_user.h

LIBS += -lboost_system -lboost_thread -lboost_filesystem
LIBS += -lpthread
LIBS += -lmysqlcppconn
LIBS += /usr/local/lib/libprotobuf.a


INCLUDEPATH += ../sdk
DEFINES += DEBUG

