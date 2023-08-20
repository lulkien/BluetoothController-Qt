#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "bluetoothmanager.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("BluetoothManager", &BluetoothManager::instance());

    // load main module
    engine.load("qrc:/ui/Main.qml");

    return app.exec();
}
