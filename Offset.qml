import QtQuick
import QtCharts 2.16

Item {
    id: root
    anchors.fill: parent
    width: 100
    height: 100
    property int counter: 0
    property bool firstReading: true;


    ChartView {
        anchors.fill: root
        antialiasing: true

        backgroundColor: "#00FFFFFF"
        legend.visible: false
        opacity: 0.5

        ValuesAxis {
            id: counter_axis
            min: -10;
            max: dataHandler.offsetBufferLength + 10;
            visible: false
        }

        ValuesAxis {
            id: value_axis
            min: 0
            max: 1
            visible: false

            // Behavior on min { PropertyAnimation { duration: 10 }}
            // Behavior on max { PropertyAnimation { duration: 10 }}
        }

        ScatterSeries {
            id: offset_scatter
            axisX: counter_axis
            axisY: value_axis
        }

        Connections {
            target: serial

            function onSensorData( loadCellReading ) {
                if ( counter > dataHandler.offsetBufferLength )
                    return;

                /* Check if the min and max of the value axis (y-axis) has been set */
                if ( firstReading ) {
                    firstReading = false;
                    value_axis.max = loadCellReading + 200;
                    value_axis.min = loadCellReading - 200;
                }
                /* Perform range check and update value axis (y-axis) accordingly */
                else {
                    if ( loadCellReading < value_axis.min )
                        value_axis.min = loadCellReading - 10;
                    else if ( loadCellReading > value_axis.max )
                        value_axis.max = loadCellReading + 10;
                }

                offset_scatter.append(counter, loadCellReading);
                counter++;
            }
        }
    }
    Text {
        id: heading
        anchors {
            verticalCenter: root.verticalCenter
            horizontalCenter: root.horizontalCenter

        }
        font.pixelSize: 50
        font.letterSpacing: 10

        text: "Measuring Offset"

        Connections {
            target: dataHandler

            function onOffsetCompleteChanged() {
                heading.text = "Complete"
            }
        }
    }

}
