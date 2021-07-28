import QtQuick 2.0

Rectangle {

    property string label
    property string icon

    signal buttonClicked

    radius: 5
    border.color: "#b5b6bb"
    border.width: 2
    color: "#152852"

    Text {
        anchors.fill: parent
        text: label
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideMiddle
        color: "white"
        visible: label != ""
    }

    Image {
        height: 20
        width: height
        anchors.centerIn: parent
        source: icon
        visible: label == ""
    }

    MouseArea {
        anchors.fill: parent

        onPressed: {
            parent.color = "#5b6885"
        }

        onReleased: {
            parent.color = "#152852"
            buttonClicked()
        }
    }
}
