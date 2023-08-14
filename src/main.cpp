#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "bluetoothmanager.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//        &app, [](QObject *obj, const QUrl &objUrl) {
//            if (nullptr == obj)
//                QCoreApplication::exit(-1);
//        }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("BluetoothManager", &BluetoothManager::instance());
    engine.rootContext()->setContextProperty("DevicesList", BluetoothManager::instance().devicesList());

    // load main module
    engine.load("qrc:/ui/Main.qml");

    return app.exec();
}
