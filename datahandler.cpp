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


/*!
 * \brief dataHandler::setLoadCellReading
 * \param loadCellReading
 */
void dataHandler::setLoadCellReading(float loadCellReading)
{
    if ( offset_values.length() < m_offset_buffer_length ){ ///< Populate list of 200 scale offset values.
        offset_values.append( loadCellReading );
    }

    else if ( m_offset == 0.0 ) { ///< If the average offset value is 0.0, then the offset average is determined and set.
        float offset_average = listAverage( offset_values );
        setOffset( offset_average );
        m_offsetComplete = true;
        emit offsetCompleteChanged();
        qDebug() << "OFFSET COMPLETE" ;
    }
    else {

        if ( m_knownWeight != 0 && calibration_values.length() < 200 )              ///< Populate calibration values.
        {
            calibration_values.append( loadCellReading - m_offset);
        }
        else if ( calibration_values.length() == 200 && m_calibrationFactor == 0)   ///< Average calibration values.
        {
            float average_calibration = listAverage( calibration_values ) / m_knownWeight;
            m_calibrationFactor = average_calibration;
        }
        else if (m_calibrationFactor != 0){ ///< Scale reading once the scale offset and calibration are calculated.
            qDebug() << __FUNCTION__ << (loadCellReading - m_offset)/m_calibrationFactor;
        }

        m_loadCellReading = (loadCellReading - m_offset)/m_calibrationFactor; ///u
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
