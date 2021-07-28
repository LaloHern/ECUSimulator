import QtQuick 2.0
import QtGraphicalEffects 1.0
import "../../JavaScriptFiles/simulationUtils.js" as SimUtils

Item {
    id: sliderContainer

    property double sliderValue
    property bool reset

    onResetChanged: {
        if (reset) {
            resetAnimation.running = true
            reset = false
        }
    }

    Rectangle {
        id: sliderBackground
        width: 30
        height: parent.height
        radius: 15
        color: "#abdbe3"
        anchors.centerIn: parent
    }

    DropShadow {
        anchors.fill: sliderBackground
        horizontalOffset: 1
        verticalOffset: 1
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: sliderBackground
    }

    Rectangle {
        width: 30
        radius: 15
        color: "#e28743"

        anchors {
            top: sliderIndicator.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }

    Rectangle {
        id: sliderIndicator

        y: sliderContainer.height - sliderIndicator.height
        width: 30
        height: 30
        radius: 15
        color: "#eeeee4"

        anchors.horizontalCenter: parent.horizontalCenter

        Drag.active: dragArea.drag.active
        Drag.hotSpot.x: 5
        Drag.hotSpot.y: 5

        onYChanged: sliderValue = SimUtils.map(sliderIndicator.y, sliderContainer.height - sliderIndicator.height, 0, 0, 100)

        Rectangle {
            id: innerShape
            anchors.centerIn: parent
            width: 25
            radius: width / 2
            height: width
        }

        InnerShadow {
            anchors.fill: innerShape
            radius: 2.0
            samples: 4
            horizontalOffset: 0.3
            verticalOffset: 0.3
            color: "#b0000000"
            source: innerShape
        }

        MouseArea {
            id: dragArea
            anchors.fill: parent
            drag.target: parent
            drag.minimumY: 0
            drag.maximumY: sliderContainer.height - sliderIndicator.height
            drag.axis: Drag.YAxis
        }

        PropertyAnimation {
            id: resetAnimation
            target: sliderIndicator
            property: "y"
            to: sliderContainer.height - sliderIndicator.height
            duration: 100
            easing.type: Easing.InOutQuad
        }
    }
}
