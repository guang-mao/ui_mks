#ifndef COMMANDWORKER_H
#define COMMANDWORKER_H

#include <QThread>
#include "ThreadSafeQueue.h"

class CommandWorker : public QThread
{
    Q_OBJECT
public:
    explicit CommandWorker(QObject *parent = nullptr);
    void run() override;

    ThreadSafeQueue<QByteArray> commandQueue;

signals:
    void requestWriteData(const QByteArray &data, bool expectReply);
    void errorOccurred(const QString &error);

public slots:
    void stopWorker(); // 用於停止線程的槽
private:
    bool running = true;
};

#endif // COMMANDWORKER_H
