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

// 計算封包長度的函數
int calculatePacketLength(const QByteArray &buffer) {
    if (buffer.size() < 4) {
        return -1; // 不足以檢查第四個字節
    }

    unsigned char byte4 = static_cast<unsigned char>(buffer[3]);
    int length = 8; // 最小長度

    // 根據 byte4 的位來增加封包長度
    if (byte4 & 0x01) length += 2; // bit 1
    if (byte4 & 0x02) length += 4; // bit 2
    if (byte4 & 0x04) length += 2; // bit 3

    return length;
}

void MainWindow::onReadyRead()
{
    rxBuf.append(ser->read_message());

    // 確保有至少8個字節可以檢查 header1 和 header2 以及計算最小封包長度
    while ( rxBuf.size() >= 8 )
    {
        if (static_cast<unsigned char>(rxBuf[0]) == 0xbc &&
            static_cast<unsigned char>(rxBuf[1]) == 0x73 &&
            static_cast<unsigned char>(rxBuf[3]) == 0x02 )
        {
            // 預期的封包頭部標記和第三個字節為0x02

            // 計算封包長度
            int packetLength = calculatePacketLength(rxBuf);
            if ( packetLength == -1 )
            {
                // 不足以檢查第四個字節，等待更多數據
                break;
            }

            // 檢查是否有足夠的數據形成完整封包
            if ( rxBuf.size() >= packetLength )
            {
                // 獲取完整封包數據
                QByteArray packet = rxBuf.left(packetLength);
                qDebug() << "Received data:" << packet << "\n";
                // 處理封包
                //processPacket(packet);

                // 從緩衝區中移除已處理的封包
                rxBuf.remove(0, packetLength);
            }
            else
            {
                // 數據不足以形成一個完整封包，等待更多數據
                break;
            }
        }
        else
        {
            // 非預期的頭部標記或第三個字節不為0x02，移除第一個字節並繼續檢查
            rxBuf.remove(0, 1);
        }
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
