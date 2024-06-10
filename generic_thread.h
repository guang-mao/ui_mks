#ifndef GENERIC_THREAD_H
#define GENERIC_THREAD_H

#include <QDebug>
#include "serial_port.h"
#include <QThread>

class TxThread : public QThread
{
    Q_OBJECT
public:
    explicit TxThread(const QString port, int baudrate, QThread *parent = nullptr);
    //TxThread(serial_port *ser, QObject *parent = nullptr);
    ~TxThread();
    void start();
    void stop();
    serial_port *_ser;
protected:
    void run() override;

private:
    bool _keepGoing;
};

// class RxTxThread : public QThread
// {
// public:
//     explicit RxTxThread();
//     virtual ~RxTxThread(){};
//     virtual void start();
//     virtual void stop();

// protected:
//     virtual void run() override;

// private:
//     bool _keepGoing;
// };

#endif // GENERIC_THREAD_H
