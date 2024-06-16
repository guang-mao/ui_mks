#include "commandworker.h"
//#include "serialportmanager.h"

CommandWorker::CommandWorker(QObject *parent) : QThread(parent)
{

}

void CommandWorker::stopWorker()
{
    running = false;
}

void CommandWorker::run()
{
    QByteArray command;
    while ( running )
    {
        if ( commandQueue.dequeue(command) )
        {
            // 在这里发送命令，例如通过串口
            emit requestWriteData(command, 0);
            QThread::msleep(1); // 讓傳送封包間有個idle時間
            //SerialPortManager::instance().writeData(command, 0 );

        } else
        {
            // 如果佇列為空，暫停一段時間
            QThread::msleep(10); // 防止CPU過度使用
        }
    }
}
