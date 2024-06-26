#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mks.equipment.actuator.actuateCommand.h"
#include "BlockingQueue.h"

QString used_port = nullptr;
int used_baudRate = 0;

// 定义全局变量和队列
BlockingQueue<QByteArray> rxQueue;
BlockingQueue<QByteArray> txQueue;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //_ser = new serial_port(used_port, used_baudRate);

    if ( 1 ) //_ser->start()
    {
        // 创建发送线程
        sender = new SenderThread(this);
        //sender->start();

        // 創建一個新的 QTimer 實例，並將其設置為 mainwindow 的子物件
        timer = new QTimer(this);
        timer->setSingleShot(false);
        // 將 QTimer 的 timeout 信號連接到 actuateCommand 槽函數
        QObject::connect(timer, &QTimer::timeout, this, &MainWindow::actuateCommand);
        timer->start(50); // 啟動定時器，並設置每 50 毫秒觸發一次 timeout 信號

    } else
    {
        //tx_thread->stop();
        delete this;
    }

}

MainWindow::~MainWindow()
{
    sender->quit();
    sender->wait();
    delete sender;

    timer->stop();
    delete timer;

    //delete ser;
    delete ui;
}

void MainWindow::actuateCommand()
{
    static actuateCommand_req_t req;

    uint8_t mode = ( (uint8_t) ( ui->ckb_act->isChecked() ) << 0u ) | ( (uint8_t) ( ui->ckb_pos->isChecked()  ) << 1u ) | \
                   ( (uint8_t) ( ui->ckb_cir->isChecked() ) << 2u ) | ( (uint8_t) ( ui->ckb_temp->isChecked() ) << 3u );
    mks_equipment_actuator_actuateCommand_encode(&req, 0, mode, 1500);
    QByteArray byteArray(reinterpret_cast<char*>(&req), sizeof(req));
    txQueue.enqueue(byteArray);
    //qDebug() << "Queue Size: " << txQueue.queue.count() << "\n";
    req.byte4.fcn.fsh_cnt++;
}
