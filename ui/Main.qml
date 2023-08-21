import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: root
    width: 1280
    height: 720
    maximumHeight: height
    minimumHeight: height
    maximumWidth: width
    minimumWidth: width
    visible: true
    title: qsTr("Hello World")

    Loader {
        id: screen_loader
        anchors.fill: parent
        source: "qrc:/ui/screens/ChatScreen.qml"
    }
}
