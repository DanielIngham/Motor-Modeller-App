#ifndef SERIALRECIEVER_H
#define SERIALRECIEVER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSerialPort>
#include <QTime>


class serialReciever : public QThread
{
    Q_OBJECT
public:
    explicit serialReciever(QObject *parent = nullptr);
    ~serialReciever();

    void startReceiver(const QString &portName, int waitTimeout, const QString &response);

signals:
    void sensorData(const float &loadCellReading);
    void request(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    void run() override;

    QString m_portName;
    QString m_response;
    int m_waitTimeout = 0;
    QMutex m_mutex;
    bool m_quit = false;


};

#endif // SERIALRECIEVER_H
