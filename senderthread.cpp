// Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#include "serial_port.h"
#include "senderthread.h"
#include "BlockingQueue.h"

#include <QSerialPort>
#include <QTime>

extern QString used_port;
extern int used_baudRate;
extern BlockingQueue<QByteArray> txQueue;

SenderThread::SenderThread(serial_port *ser, QObject *parent) :
    QThread(parent), ser(ser), is_open(false)
{
    //Start();
}

//! [0]
SenderThread::~SenderThread()
{
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}
//! [0]

//! [1] //! [2]
void SenderThread::Start()
{
    //! [1]
    const QMutexLocker locker(&m_mutex);
    if ( !isRunning() )
    {
        start();
    }
    else
    {
        m_cond.wakeOne();
    }
}
//! [2] //! [3]

//! [4]
void SenderThread::run()
{
    if ( ser->serialPort->isOpen() )
    {
        is_open = true;
        m_quit = false;
    } else
    {
        is_open = false;
        m_quit = true;
    }

    while ( !m_quit )
    {
        QByteArray data = txQueue.dequeue();
        ser->write_message(reinterpret_cast<uint8_t *>(data.data()), data.size());
        //QThread::msleep(50);
    }
}
