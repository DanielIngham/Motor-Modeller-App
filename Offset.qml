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
        opacity: 0.5
        antialiasing: true
        legend.visible: false
        ValuesAxis {
            id: counter_axis
            min: 0;
            max: dataHandler.offsetBufferLength;
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
                /* Update the counter axis (x-axis) to always be one larger than the counter. */
                if ( counter == counter_axis.max ) {
                    counter_axis.max = counter + 1;
                }

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
        text: "Measuring Offset"

    }
}
