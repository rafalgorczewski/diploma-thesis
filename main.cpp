#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "calibrationbackend.hpp"
#include "recorderbackend.hpp"
#include "configurator.hpp"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<CalibrationBackend>("Backend.Calibration",1,0,"Calibration");
    qmlRegisterType<RecorderBackend>("Backend.Recorder",1,0,"Recorder");
    qmlRegisterType<Configurator>("Backend.Configurator",1,0,"Configurator");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
