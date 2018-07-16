#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "QQmlContext"
#include "backend.h"


int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QPM_INIT(engine)

    // register backend object
    backend* _backend = new backend;
    engine.rootContext()->setContextProperty("backend", _backend);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // register cleaning up function
    QObject::connect(&app, SIGNAL(aboutToQuit()), _backend, SLOT(cleaning_up()));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
