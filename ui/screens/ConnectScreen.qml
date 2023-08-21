import QtQuick 2.12
import "../components"

Item {
    id: root
    readonly property int globalSpacing: 10
    Column {
        id: root_layout
        anchors.fill: parent
        anchors.margins: root.globalSpacing
        spacing: root.globalSpacing
        Row {
            id: btn_layout
            width: parent.width
            height: parent.height * 0.1
            spacing: root.globalSpacing
            Button {
                id: scan_btn
                height: parent.height
                width: (parent.width - root.globalSpacing) / 2
                border.color: "black"
                label.text: "SCAN"
                onClicked: {
                    BluetoothManager.startScanClicked()
                }
            }
            Button {
                id: stop_btn
                height: parent.height
                width: (parent.width - root.globalSpacing) / 2
                border.color: "black"
                label.text: "STOP"
                onClicked: {
                    BluetoothManager.stopScanClicked()
                }
            }
        }

        Rectangle {
            id: list_service_area
            border.color: "black"
            width: parent.width
            height: root_layout.height - btn_layout.height - root.globalSpacing
            ListView {
                id: list_service
                anchors.fill: list_service_area
                model: ServiceModel
                delegate: Item {
                    width: list_msg.width
                    height: list_msg.height / 10

                    Text {
                        width: parent.width - 20
                        anchors.centerIn: parent
                        font.pixelSize: 20
                        text: remoteName + " | " + address
                    }
                }
            }
        }
    }
}
