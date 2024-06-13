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

    ser = new serial_port(used_port, used_baudRate);

    if ( ser->start() )
    {
        QObject::connect(ser->serialPort, &QSerialPort::readyRead, this, &MainWindow::onReadyRead);
        // 創建一個新的 QTimer 實例，並將其設置為 mainwindow 的子物件
        timer = new QTimer(this);
        timer->setSingleShot(false);
        // 將 QTimer 的 timeout 信號連接到 actuateCommand 槽函數
        QObject::connect(timer, &QTimer::timeout, this, &MainWindow::actuateCommand);
        timer->start(50); // 啟動定時器，並設置每 50 毫秒觸發一次 timeout 信號
    } else
    {
        //delete ser;
        delete this;
    }

}

MainWindow::~MainWindow()
{
    delete ser;
    timer->stop();
    delete timer;
    delete ui;
}

typedef enum {
    NONE = 0,
    HEADER1,
    HEADER2,
    COMMAND,
    MODE,
} state;
state st = NONE;
uint8_t length = 0;
void MainWindow::onReadyRead()
{
    rxBuf = ser->read_message();
    QByteArray src;

    // if ( rxBuf.size() > 0 )

    while (  ( rxBuf.size() > 0 ) )
    {
        src = rxBuf.left(1);
        switch ( st )
        {
        case NONE:
            if ( *( (uint8_t *) src.data() ) == 0xbc )
            {
                st = HEADER1;
                packet.append(src);
            } else
            {
                st = NONE;
                packet.clear();
            }
            break;
        case HEADER1:
            if ( *( (uint8_t *) src.data() ) == 0x73 )
            {
                st = HEADER2;
                packet.append(src);
            } else
            {
                st = NONE;
                packet.clear();
            }
            break;
        case HEADER2:
            st = COMMAND;
            packet.append(src);
            break;
        case COMMAND:
            st = MODE;
            if ( (uint8_t) packet.at(3) == 0x02 )
            {

            } else
            {

            }
            break;
        case MODE:
            break;
    }
    //qDebug() << "Received data:" << packet << "Size:" << packet.size() << "\n";
}

void MainWindow::actuateCommand()
{
    static actuateCommand_req_t req;
    uint8_t mode = ( (uint8_t) ( ui->ckb_act->isChecked() ) << 0u ) | ( (uint8_t) ( ui->ckb_pos->isChecked()  ) << 1u ) | \
                   ( (uint8_t) ( ui->ckb_cir->isChecked() ) << 2u ) | ( (uint8_t) ( ui->ckb_temp->isChecked() ) << 3u );
    mks_equipment_actuator_actuateCommand_encode(&req, 0, mode, 1500);
    QByteArray byteArray(reinterpret_cast<char*>(&req), sizeof(req));
    ser->write_message(reinterpret_cast<uint8_t *>(byteArray.data()), byteArray.size());
    //txQueue.enqueue(byteArray);
    //qDebug() << "Queue Size: " << txQueue.queue.count() << "\n";
    req.byte4.fcn.fsh_cnt++;
}
