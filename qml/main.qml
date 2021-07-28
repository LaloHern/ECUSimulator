import QtQuick 2.9
import QtQuick.Window 2.2
import "Widgets"
import "../JavaScriptFiles/simulationUtils.js" as SimUtils

Item {
    id: root
    height: 480
    width: 800
    anchors.centerIn: parent

    property int milDistance

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 30
        spacing: 5

        Tacometro {
            id: tachometerIndicator
            width: 180
            height: width
            minOutput: -130
            maxOutput: 130
            maxInput: 100
            minInput: 0
        }

        Velocimetro {
            id: speedometer
            width: 180
            height: width
            minOutput: -130
            maxOutput: 130
            maxInput: 100
            minInput: 0
        }

        FuelIndicator {
            id: fuelIndicator
            width: 180
            height: width
            minOutput: -131
            maxOutput: 131
            maxInput: 100
            minInput: 0
        }

        TemperatureIndicator {
            id: temperatureIndicator
            width: 180
            height: width
            minOutput: -65
            maxOutput: 65
            maxInput: 100
            minInput: 0
        }
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 250
        spacing: 5

        Item {
            height: 170
            width: 180

            ControlSlider {
                id: tachometerControl
                anchors.horizontalCenter: parent.horizontalCenter
                height: 170
                width: 30

                onSliderValueChanged: {
                    tachometerIndicator.input = sliderValue
                    canInterface.engineRpm = SimUtils.map(sliderValue, 0, 100, 0, 8000)
                }
            }
        }

        Item {
            height: 170
            width: 180

            ControlSlider {
                id: speedometerControl
                anchors.horizontalCenter: parent.horizontalCenter
                height: 170
                width: 30

                onSliderValueChanged: {
                    speedometer.input = sliderValue
                    canInterface.speed = SimUtils.map(sliderValue, 0, 100, 0, 240)
                }
            }
        }

        Item {
            height: 170
            width: 180

            ControlSlider {
                id: fuelControl
                anchors.horizontalCenter: parent.horizontalCenter
                height: 170
                width: 30

                onSliderValueChanged: {
                    fuelIndicator.input = sliderValue
                    canInterface.fuelLevel = SimUtils.map(sliderValue, 0, 100, 0, 100)
                }
            }
        }

        Item {
            height: 170
            width: 180

            ControlSlider {
                id: temperatureControl
                anchors.horizontalCenter: parent.horizontalCenter
                height: 170
                width: 30

                onSliderValueChanged: {
                    temperatureIndicator.input = sliderValue
                    canInterface.engineTemp = SimUtils.map(sliderValue, 0, 100, 0, 200)
                }
            }
        }
    }

    Item {
        width: 60
        height: 30
        visible: menu.height === 0
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 15
        }

        Image {
            anchors.centerIn: parent
            width: 50
            height: 40
            source: "qrc:/images/icons/chevron.png"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: menu.height = root.height
        }
    }

    MouseArea {
        anchors.fill: parent
        enabled: menu.height !== 0

        onClicked: menu.closeDrawer()
    }

    Drawer {
        id: menu
        width: parent.width
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
        }
        menuTitle: "Controles Adicionales"

        Behavior on height { NumberAnimation { duration: 200 } }

        onCheckEngine: {
            canInterface.malfunctionIndicator = !canInterface.malfunctionIndicator
        }

        onCloseDrawer: height = 0

        onReset: {
            canInterface.malfunctionIndicator = false
            tachometerControl.reset = true
            speedometerControl.reset = true
            fuelControl.reset = true
            temperatureControl.reset = true
            milDistanceTimer.stop()
            milDistance = 0
        }
    }

    Connections {
        target: canInterface

        onMalfunctionIndicatorChanged: {
            if (canInterface.malfunctionIndicator) {
                milDistanceTimer.start()
            } else {
                milDistanceTimer.stop()
            }
        }
    }

    Timer {
        id: milDistanceTimer
        interval: 3000
        running: false
        repeat: true

        onTriggered: {
            if (milDistance < 10000)
                milDistance ++

            canInterface.malfunctionDistance = milDistance
        }
    }
}
