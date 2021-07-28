import QtQuick 2.0
import QtGraphicalEffects 1.0
import "../../JavaScriptFiles/simulationUtils.js" as SimUtils
import "../Components"

Item {
    property string menuTitle

    signal closeDrawer
    signal checkEngine
    signal reset

    clip: true

    Rectangle {
        anchors.fill: parent
        color: "#717171"
        opacity: 0.9

        //atrapa los clicks o toques dentro de la caja del menu para evitar que se cierre
        MouseArea {
            anchors.fill: parent

            onClicked: {}
        }
    }

    Item {
        width: 60
        height: 30
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 17
        }
        //transform: Rotation { origin.x: width / 2; origin.y: height / 2 ; angle: 180}

        Image {
            id: chevronIcon
            anchors.centerIn: parent
            width: 50
            height: 40
            source: "qrc:/images/icons/chevron.png"
            rotation: 180

            ColorOverlay {
                anchors.fill: parent
                source: parent
                color: "white"
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: closeDrawer()
        }
    }

    Text {
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 80
        }

        text: menuTitle
        font.pixelSize: 30
        horizontalAlignment: Text.AlignHCenter
        color: "white"
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 140
        spacing: 5

        Column {
            height: 200
            width: 180
            spacing: 10

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Sensor MAF"
                font.pixelSize: 18
                color: "white"
            }

            ControlSlider {
                id: mafControl
                anchors.horizontalCenter: parent.horizontalCenter
                height: 170
                width: 30

                onSliderValueChanged: {
                    canInterface.mafRatio = SimUtils.map(sliderValue, 0, 100, 0, 655.3)
                }
            }
        }

        Column {
            height: 200
            width: 180
            spacing: 10

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Temp. aire admisión"
                font.pixelSize: 18
                color: "white"
            }

            ControlSlider {
                id: intakeAirControl
                anchors.horizontalCenter: parent.horizontalCenter
                height: 170
                width: 30

                onSliderValueChanged: {
                    canInterface.intakeAirTemp = SimUtils.map(sliderValue, 0, 100, -40, 215)
                }
            }
        }

        Column {
            height: 200
            width: 180
            spacing: 10

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Sensor MAP"
                font.pixelSize: 18
                color: "white"
            }

            ControlSlider {
                id: timingControl
                anchors.horizontalCenter: parent.horizontalCenter
                height: 170
                width: 30

                onSliderValueChanged: {
                    canInterface.mapPressure = SimUtils.map(sliderValue, 0, 100, 0, 255)
                }
            }
        }

        Column {
            spacing: 10

            Button {
                width: 150
                height: 60
                opacity: 0.95
                label: "Simular Falla"

                onButtonClicked: {
                    checkEngine()
                }
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/images/check.png"
                visible: canInterface.malfunctionIndicator
            }
        }
    }

    Button {
        width: 150
        height: 60
        anchors {
            bottom: parent.bottom
            bottomMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        opacity: 0.95
        label: "Reiniciar"

        onButtonClicked: {
            reset()
        }
    }

    onReset: {
        mafControl.reset = true
        intakeAirControl.reset = true
        timingControl.reset = true
    }
}
