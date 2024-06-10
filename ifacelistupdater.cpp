#include "ifacelistupdater.h"
#include <QDebug>

BackgrondIfaceListUpdater::BackgrondIfaceListUpdater(QObject *parent)
    : QThread(parent), _keepGoing(false)
{
    _ifaces = list_ifaces();
    _keepGoing = true;
}

BackgrondIfaceListUpdater::~BackgrondIfaceListUpdater()
{
    stop();
    qDebug() << "Thread deleted\n";
}

void BackgrondIfaceListUpdater::start()
{
    if ( !isRunning() )
    {
        _keepGoing = true;
        QThread::start();
    }
}

void BackgrondIfaceListUpdater::stop()
{
    _keepGoing = false;
    if ( isRunning() )
    {
        quit();
        wait();
    }
}

void BackgrondIfaceListUpdater::run()
{
    while ( _keepGoing )
    {
        QThread::msleep(500);
        QMutexLocker locker(&_mutex);

        _ifaces = list_ifaces();
        qDebug() << "Updated interfaces list.\n";
    }
}

QMap<QString, QString> BackgrondIfaceListUpdater::list_ifaces()
{
    //創建 QMap 對象
    QMap<QString, QString> out;
    QList<QSerialPortInfo> serailPorts = QSerialPortInfo::availablePorts();

    for ( const QSerialPortInfo &port : serailPorts )
    {
        out[port.description()] = port.systemLocation();
    }
    return out;
}

QMap<QString, QString> BackgrondIfaceListUpdater::get_list()
{
    QMutexLocker locker(&_mutex);
    return _ifaces;
}
