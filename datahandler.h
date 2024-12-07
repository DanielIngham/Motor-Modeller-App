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
    float m_knownWeight = 0.0;
    float m_calibrationFactor = 0.0;
    float m_offset = 0.0;
    float m_loadCellReading;

    QList<float> offset_values;
    QList<float> calibration_values;

    float listAverage( QList<float> inputList);

};

#endif // DATAHANDLER_H
