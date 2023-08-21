import QtQuick 2.15

Rectangle {
    id: root_btn
    property alias label: btn_label
    property bool enabled: true

    width: 50
    height: 100

    signal clicked()
    signal pressed()

    Text {
        id: btn_label
        anchors.centerIn: parent
    }

    MouseArea {
        id: ma
        anchors.fill: parent
        enabled: root_btn.enabled
        onClicked: root_btn.clicked()
        onPressed: root_btn.pressed()
    }
}
