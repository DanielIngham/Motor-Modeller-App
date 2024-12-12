import QtQuick
import QtQuick.Controls
import QtCharts
import QtCore

 ApplicationWindow {
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Motor Modeller")

    StateGroup {
        id: appStates
        state : "calibration"
        states : [
            State {
                name: "home"
                // PropertyChanges {
                //     target: object

                // }
            },

            State {
                name: "calibration"

                PropertyChanges {
                    target: offset.visible = true;
                }
            }
        ]
    }

    Offset {
        id: offset
        visible: true

    }

    TextField {
        id: knownWeightInput

        placeholderText: qsTr("Enter known weight (g)")

        validator: DoubleValidator{bottom: 0; top: 1000; decimals: 5}

        anchors {
            top: parent.top
            left: parent.left

            topMargin: 10
            leftMargin: 10
        }

    }
    Button {
        id: enterKnownWeight
        text: "Enter"
        anchors {
            top: knownWeightInput.top
            left: knownWeightInput.right
            bottom: knownWeightInput.bottom

            leftMargin: 10
        }

        onClicked: {
            if (knownWeightInput.text !== "")
            {
                dataHandler.setKnownWeight(knownWeightInput.text)
            }
        }
    }

    Connections {
        target: dataHandler

        function onOffsetCompleteChanged() {

        }
    }
}
