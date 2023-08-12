#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "common.h"
#include "bluetoothmanager.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);
    qSetMessagePattern(LOGGING_PATTERN);

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [](QObject *obj, const QUrl &objUrl) {
            if (nullptr == obj)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("DevicesList", BluetoothManager::instance().devicesList());

    // load main module
    engine.loadFromModule("Bluetooth", "Main");

    return app.exec();
}
