#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "chatmanager.h"
#include "bluetoothmanager.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("ChatManager",     &ChatManager::instance());
    engine.rootContext()->setContextProperty("Conversation",    ChatManager::instance().getConversation());

    engine.rootContext()->setContextProperty("BluetoothM",      &BluetoothManager::instance());
    engine.rootContext()->setContextProperty("ServiceModel",    BluetoothManager::instance().getServiceModel());


    // load main module
    engine.load("qrc:/ui/Main.qml");

    return app.exec();
}
