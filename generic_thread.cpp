#include "generic_thread.h"

TxThread::TxThread(const QString port, int baudrate, QThread *parent) : _keepGoing(false)
{
    _ser = new serial_port(port, baudrate);
    _ser->start();
    //start();
}

TxThread::~TxThread()
{
    stop();
    qDebug() << "txThread deleted\n";
}

void TxThread::start()
{
    if ( !isRunning() )
    {
        _keepGoing = true;
        QThread::start();
    }
}

void TxThread::stop()
{
    _keepGoing = false;
    if ( isRunning() )
    {
        quit();
        wait();
    }
}


void TxThread::run()
{
    while ( _keepGoing )
    {
        uint8_t string[] = "hello world!\n";
        //QByteArray data = txQueue.dequeue();
        _ser->write_message( string, sizeof(string) );
        //_ser->write_message(reinterpret_cast<uint8_t *>(data.data()), data.size());
        //qDebug() << "send thread\n";
        QThread::msleep(50);
    }
}
