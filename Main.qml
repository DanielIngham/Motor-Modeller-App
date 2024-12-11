import QtQuick
import QtQuick.Controls
import QtCharts
import QtCore

 ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Motor Modeller")

    // StateGroup {
    //     state : "Login"
    //     states : [
    //         State {
    //             name: "calibration"

    //             // PropertyChanges {
    //                 // target: object

    //             // }
    //         }
    //     ]
    // }

    Offset {
        id: offset
        width : 100
        height: 100


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
}
