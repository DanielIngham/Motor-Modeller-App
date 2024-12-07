#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


#include "serialreciever.h"
#include "datahandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    /* const QString &portName, int waitTimeout, const QString &response */
    serialReciever *m_thread = new serialReciever;
    dataHandler *dh = new dataHandler;

    m_thread->startReceiver("COM4", 10000, "hELLO");

    QObject::connect(m_thread, &serialReciever::sensorData, dh, &dataHandler::setLoadCellReading);


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/MotorModeller/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("dataHandler", dh);
    engine.load(url);


    return app.exec();
}
