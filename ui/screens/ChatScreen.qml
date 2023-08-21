import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    readonly property int globalSpacing: 10

    Column {
        id: col_layout
        anchors.fill: parent
        anchors.margins: 10
        spacing: root.globalSpacing
        Rectangle {
            id: list_msg_area
            width: col_layout.width
            height: col_layout.height * 0.8
            border.color: "black"
            ListView {
                id: list_msg
                anchors.fill: list_msg_area
                model: Conversation
                delegate: Item {
                    width: list_msg.width
                    height: list_msg.height / 10

                    Rectangle {
                        id: chat_bubble
                        anchors.fill: parent
                        anchors.margins: 5
                        radius: 10
                        border.color: "black"
                    }

                    Text {
                        width: parent.width - 20
                        anchors.centerIn: parent
                        horizontalAlignment: owner ? Text.AlignRight : Text.AlignLeft
                        font.pixelSize: 20
                        text: owner ? message : (sender + ": " + message)
                    }
                }
            }
        }

        Row {
            width: parent.width
            height: parent.height - list_msg_area.height - parent.spacing
            spacing: root.globalSpacing
            Rectangle {
                id: text_box_area
                width: parent.width * 0.8
                height: parent.height
                border.color: "black"
                clip: true
                TextEdit {
                    id: text_box
                    anchors.fill: parent
                    anchors.margins: 5
                    wrapMode: TextEdit.WrapAnywhere
                    font.pixelSize: 20
                }
            }

            Rectangle {
                id: send_btn
                width: parent.width - text_box_area.width - parent.spacing
                height: parent.height
                border.color: "black"
                Text {
                    anchors.centerIn: parent
                    text: "SEND"
                    font.pixelSize: 25
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        ChatManager.sendMessage(text_box.text)
                        text_box.text = ""
                    }
                }
            }
        }
    }
}
