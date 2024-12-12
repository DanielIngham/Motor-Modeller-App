#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


#include "serialreciever.h"
#include "datahandler.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    serialReciever *m_thread = new serialReciever;  ///< Create new instance of serial reception class
    dataHandler *dh = new dataHandler;              ///< Create new instance of datahandler class

    m_thread->startReceiver("COM4", 10000);         ///< Start Serial Communication on COM4 with a timeout of 10 seconds.

    QObject::connect(m_thread, &serialReciever::sensorData, dh, &dataHandler::setLoadCellReading); ///< Connect sensorData signal from the serial receiver to the dataHanldlers slot

    /* QT GUI setup */
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/MotorModeller/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    /* Connect dh class to qml with name dataHandler */
    engine.rootContext()->setContextProperty("dataHandler", dh);
    engine.rootContext()->setContextProperty("serial", m_thread);
    engine.load(url);


    return app.exec();
}
