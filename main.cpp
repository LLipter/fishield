#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "QQmlContext"
#include "login_backend.h"

void init(QQmlContext* context){
    login_backend* lbackend = new login_backend;
    context->setContextProperty("login_backend", lbackend);
}

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QPM_INIT(engine)
    init(engine.rootContext());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
