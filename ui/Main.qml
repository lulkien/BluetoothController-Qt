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
                id: name
                width: (parent.width - 2 * parent.height) / 2
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 20
                text: model.deviceName
            }

            Text {
                id: address
                width: name.width / 2
                anchors.left: name.right
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 20
                text: model.deviceAddress
            }

            Rectangle {
                id: unpairButton
                anchors.right: parent.right
                anchors.rightMargin: parent.height * 0.05
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height * 0.9
                width: height
                border.color: "black"
                Text {
                    anchors.centerIn: parent
                    text: "Unpair"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        BluetoothManager.requestUnpair(index)
                    }
                }
            }

            Rectangle {
                id: pairButton
                anchors.right: unpairButton.left
                anchors.rightMargin: parent.height * 0.05
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height * 0.9
                width: height
                border.color: "black"
                Text {
                    anchors.centerIn: parent
                    text: "Pair"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        BluetoothManager.requestPairing(index)
                    }
                }
            }

            Rectangle {
                anchors.bottom: delegateItem.bottom
                anchors.horizontalCenter: delegateItem.horizontalCenter
                color: "black"
                height: 1
                width: delegateItem.width * 0.8
            }
        }
    }
}
