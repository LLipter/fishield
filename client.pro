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
    ../fishield/file_transfer.pb.cc \
    ../fishield/fishield.cpp \
    ../fishield/fs_client.cpp \
    ../fishield/fs_scheduler.cpp \
    ../fishield/fs_server.cpp \
    ../fishield/fs_task.cpp \
    ../fishield/fs_task.pb.cc \
    backend.cpp \
    ../fishield/fs_dbmanager.cpp
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
    ../fishield/fishield.h \
    ../fishield/file_transfer.pb.h \
    ../fishield/fs_client.h \
    ../fishield/fs_config.h \
    ../fishield/fs_scheduler.h \
    ../fishield/fs_server.h \
    ../fishield/fs_task.h \

    ../fishield/fs_task.pb.h \
    backend.h \
    ../fishield/fs_dbmanager.h \
    ../fishield/fs_user.h

LIBS += -lboost_system -lboost_thread -lboost_filesystem
LIBS += -lpthread
LIBS += -lmysqlcppconn
LIBS += /usr/local/lib/libprotobuf.a


INCLUDEPATH += ../fishield

