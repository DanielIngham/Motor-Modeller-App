#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>

class dataHandler : public QObject
{
    Q_OBJECT
public:
    explicit dataHandler(QObject *parent = nullptr);

    void setOffset(float offset);
public slots:
    void setLoadCellReading(float loadCellReading);
    void setKnownWeight(float knownWeight);

signals:

private:
    float m_knownWeight = 0.0;          ///< Known Weight used for calibration of raw loadcell data.
    float m_calibrationFactor = 0.0;    ///< Linear Calibration factor calculated during calibration.
    float m_offset = 0.0;               ///< Linear Offset for raw loadcell data.
    float m_loadCellReading;            ///< Raw loadcell reading.

    QList<float> offset_values;         ///< Vector of offset values such used for averaging the offset.
    QList<float> calibration_values;    ///< Vector of calibration values used for averaging calibration factor.

    float listAverage( QList<float> inputList);

};

#endif // DATAHANDLER_H
