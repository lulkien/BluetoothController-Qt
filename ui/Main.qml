import QtQuick
import QtQuick.Window
import Bluetooth

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

    Column {
        id: layout
        anchors.fill: parent
        anchors.margins: 10
        spacing: 5
        Rectangle {
            id: scanButton
            width: parent.width
            height: root.height / 15
            border.color: "black"
            color: BluetoothManager.isScanning ? "gray" : "white"
            Text {
                anchors.centerIn: parent
                font.pixelSize: 30
                font.bold: true
                text: "Scan on"
                color: BluetoothManager.isScanning ? "white" : "black"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.warn("Clicked: scanButton")
                    BluetoothManager.startScan()
                }
                onPressAndHold: {
                    console.warn("onPressAndHold: scanButton")
                    BluetoothManager.stopScan()
                }
            }
        }

        Rectangle {
            id: listArea
            width: scanButton.width
            height: root.height - layout.anchors.margins * 2 - scanButton.height - layout.spacing
            color: "lightgray"
            border.color: "black"
            clip: true
            ListView {
                id: listDevices
                anchors.fill: parent
                model: DevicesList
                snapMode: ListView.SnapToItem
                delegate: deviceDelegateItem
            }
        }
    }

    Component {
        id: deviceDelegateItem
        Item {
            id: delegateItem
            width: listDevices.width
            height: listDevices.height / 6
            Text {
                anchors.centerIn: parent
                font.pixelSize: 30
                text: "Name: " + model.deviceName + " | Address: " + model.deviceAddress
            }
            Rectangle {
                anchors.bottom: delegateItem.bottom
                anchors.horizontalCenter: delegateItem.horizontalCenter
                color: "black"
                height: 1
                width: delegateItem.width * 0.8
            }
            MouseArea {
                anchors.fill: parent
                onPressAndHold: {
                    BluetoothManager.requestPairing(index)
                }
            }
        }
    }

    Component.onCompleted: {
        BluetoothManager.showHostInfo()
    }
}
