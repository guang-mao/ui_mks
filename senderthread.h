// Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef SENDERTHREAD_H
#define SENDERTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include "serial_port.h"

//! [0]
class SenderThread : public QThread
{
    Q_OBJECT

public:
    explicit SenderThread(serial_port *ser, QObject *parent = nullptr);
    ~SenderThread();

    void Start();

private:
    void run() override;

    serial_port *ser;
    int m_waitTimeout = 0;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bool is_open = false;
    bool m_quit = false;
};
//! [0]

#endif // SENDERTHREAD_H
