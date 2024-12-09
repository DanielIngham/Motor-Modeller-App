#include "serialreciever.h"

serialReciever::serialReciever(QObject *parent)
    : QThread{parent}
{}

serialReciever::~serialReciever()
{
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();
    wait();
}

/*!
 * \brief serialReciever::startReceiver initiates the communication link between the application and the embedded device.
 * \param portName  The COMM port name (e.g COM5).
 * \param waitTimeout   Connection timeout period.
 */
void serialReciever::startReceiver(const QString &portName, int waitTimeout)
{
    const QMutexLocker locker(&m_mutex);
    m_portName = portName;
    m_waitTimeout = waitTimeout;

    if (!isRunning())
        start();
}

/*!
 * \brief serialReciever::run
 */
void serialReciever::run()
{
    bool currentPortNameChanged = false;

    /* Check if the port name has been set */
    m_mutex.lock();

    QString currentPortName;
    if (currentPortName != m_portName) {
        currentPortName = m_portName;
        currentPortNameChanged = true;
    }

    /* Set wait time and data response */
    int currentWaitTimeout = m_waitTimeout;

    m_mutex.unlock();

    /* Serial Setup */
    QSerialPort serial;

    serial.setPortName(currentPortName);                    /* Set COM port name    */
    serial.setBaudRate( QSerialPort::Baud57600 );           /* Baud Rate 57600 BPS  */
    serial.setDataBits( QSerialPort::Data8 );               /* 8N1: 8 Data bits     */
    serial.setParity( QSerialPort::NoParity );              /* 8N1: No-parity bit   */
    serial.setStopBits( QSerialPort::OneStop );             /* 8N1: 1 Stop bit      */

    /* Clear the serial read buffer before commencing with communication to aviod intial garbage */
    serial.open(QIODevice::ReadWrite);
    currentPortNameChanged = false;
    serial.clear();

    /* Write state character ('a') and state numeic (0) to embedded device to start logging  */
    const char a[2] = {'a', 0};
    serial.write(a, 2);

    /* Read logging data from serial interface */
    while ( !m_quit ) {

        /* If the port name changed, close and open with the new port name */
        if (currentPortNameChanged) {
            serial.close();
            serial.setPortName(currentPortName);

            if (!serial.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                               .arg(m_portName).arg(serial.error()));
                return;
            }
        }

        /* If there is data in to be read from the serial port */
        if (serial.waitForReadyRead( currentWaitTimeout )) {

            /* 4 Byte float*/
            union {                
                char buffer[4U];
                float data;
            } reading;

            /* Read serial data into buffer and emit the it's corresponding   */
            if ( serial.bytesAvailable() >= 4U ) {
                /* Read 4 Bytes from butter and emit corresponding floating point values (bytesRead not used at the momemnt)*/
                qint64 bytesRead = serial.read( reading.buffer, 4U );
                emit sensorData(reading.data);
            }
        }

        /* If not serial data received within time-out, emit serial timeout) */
        else {
            emit timeout(tr("Wait read request timeout %1")
                             .arg(QTime::currentTime().toString()));
        }

        /* Lock the mutex and check if serial port name has changed and raise appropriate flags */
        m_mutex.lock();

        if (currentPortName != m_portName) {
            currentPortName = m_portName;
            currentPortNameChanged = true;
        }
        else {
            currentPortNameChanged = false;
        }

        currentWaitTimeout = m_waitTimeout;
        m_mutex.unlock();
    }
}
