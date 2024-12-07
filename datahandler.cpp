#include "datahandler.h"
#include <qdebug.h>

dataHandler::dataHandler(QObject *parent)
    : QObject{parent}
{}

void dataHandler::setOffset(float offset)
{
    if ( offset != m_offset )
    {
        m_offset = offset;
    }
}

void dataHandler::setLoadCellReading(float loadCellReading)
{
    /* Populate list of 200 scale offset values. */
    if ( offset_values.length() < 200 ){
        offset_values.append( loadCellReading );
    }
    /* If the average offset value is 0.0, then the offset average is determined and set */
    else if ( m_offset == 0.0 ) {
        float offset_average = listAverage(offset_values);
        setOffset( offset_average );
        qDebug() << "OFFSET COMPLETE" ;
    }
    else {
        /* */
        if ( m_knownWeight != 0 && calibration_values.length() < 200 )
        {
            calibration_values.append( loadCellReading - m_offset);
        }
        else if ( calibration_values.length() == 200 && m_calibrationFactor == 0)
        {
            float average_calibration = listAverage( calibration_values ) / m_knownWeight;
            m_calibrationFactor = average_calibration;
        }
        else if (m_calibrationFactor != 0){
            qDebug() << __FUNCTION__ << (loadCellReading - m_offset)/m_calibrationFactor;
        }

        m_loadCellReading = loadCellReading - m_offset;
    }
}

void dataHandler::setKnownWeight(float knownWeight)
{
    if (knownWeight != m_knownWeight) {
        m_knownWeight = knownWeight;
        qDebug() << __FUNCTION__ << m_knownWeight;
    }
}

float dataHandler::listAverage(QList<float> inputList)
{
    float average = 0.0;
    for (uint64_t i = 0; i < inputList.length(); i++) {
        average += inputList[i] / inputList.length();
    }

    return average;
}
