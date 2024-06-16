#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mks.equipment.actuator.actuateCommand.h"
#include "serialportmanager.h"
#include "actuatorpanel.h"
#include "commandworker.h"

#include <QMenuBar>
#include <QMenu>

QString used_port = nullptr;
int used_baudRate = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setpoint = 0;

    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *panelMenu = menuBar->addMenu(tr("Panel"));

    QAction *addActuatorPanelAction = panelMenu->addAction(tr("Add Actuator Panel"));
    connect(addActuatorPanelAction, &QAction::triggered, this, &MainWindow::onAddActuatorPanel);

    panelMenu->addAction(tr("Firmware Upgrade"));

    QMenu *helpMenu = menuBar->addMenu(tr("Help"));
    helpMenu->addAction(tr("About us"));
    helpMenu->addAction(tr("Contact..."));

    setMenuBar(menuBar);

    // 設置進度條樣式以便文字顯示在中央
    ui->progressBar->setStyleSheet(
        "QProgressBar {"
        "    border: 1px solid grey;"
        "    border-radius: 5px;"
        "    text-align: center;" // 文字居中
        "    font-size: 16px;"    // 設置字體大小
        "    font-weight: bold;"  // 設置粗體
        "}"
        "QProgressBar::chunk {"
        "    background-color: #05B8CC;"
        "    width: 20px;"
        "}");

    PositionLabel = new QLabel("Position: N/A", this);
    CurrentLabel = new QLabel("Current: N/A", this);
    VoltageLabel = new QLabel("Voltage: N/A", this);
    TemperatureLabel = new QLabel("Temperature: N/A", this);

    PositionLabel->setGeometry(QRect(QPoint(50, 250), QSize(250, 50)));
    CurrentLabel->setGeometry(QRect(QPoint(50, 300), QSize(250, 50)));
    VoltageLabel->setGeometry(QRect(QPoint(50, 350), QSize(250, 50)));
    TemperatureLabel->setGeometry(QRect(QPoint(50, 400), QSize(250, 50)));

    // 設置字體大小和粗體屬性
    QFont labelFont;
    labelFont.setPointSize(16);  // 設置字體大小
    labelFont.setBold(true);     // 設置字體粗體

    PositionLabel->setFont(labelFont);
    CurrentLabel->setFont(labelFont);
    VoltageLabel->setFont(labelFont);
    TemperatureLabel->setFont(labelFont);

    //ser = new serial_port(used_port, used_baudRate);
    // 在构造函数或者需要的地方设置串行端口
    SerialPortManager::instance().setupSerialPort(used_port, used_baudRate);

    //if ( ser->start() )
    {
        //QObject::connect(ser->serialPort, &QSerialPort::readyRead, this, &MainWindow::onReadyRead);
        // 接收数据
        connect(&SerialPortManager::instance(), &SerialPortManager::dataReceived, this, &MainWindow::handleDataReceived);

        // // 創建一個新的 QTimer 實例，並將其設置為 mainwindow 的子物件
        // timer = new QTimer(this);
        // timer->setSingleShot(false);
        // // 將 QTimer 的 timeout 信號連接到 actuateCommand 槽函數
        // QObject::connect(timer, &QTimer::timeout, this, &MainWindow::actuateCommand);
        // timer->start(50); // 啟動定時器，並設置每 50 毫秒觸發一次 timeout 信號

        // 创建并启动工作线程
        worker = new CommandWorker();
        worker->start();
        // 当应用程序准备关闭时，停止线程
        connect(qApp, &QCoreApplication::aboutToQuit, worker, &CommandWorker::stopWorker);
        connect(worker, &CommandWorker::requestWriteData, this, &MainWindow::writeData);

        connect(ui->btn_left, &QPushButton::clicked, this, &MainWindow::onButtonLeftClicked);
        connect(ui->btn_mid, &QPushButton::clicked, this, &MainWindow::onButtonMidClicked);
        connect(ui->btn_right, &QPushButton::clicked, this, &MainWindow::onButtonRightClicked);

    }
    // else
    // {
    //     //delete ser;
    //     delete this;
    // }

}

MainWindow::~MainWindow()
{
    if ( worker->isRunning() )
    {
        worker->stopWorker();
        worker->wait();
    }
    delete worker;
    //delete ser;
    //timer->stop();
    //delete timer;
    delete ui;
}

// 計算封包長度的函數
int calculatePacketLength(const QByteArray &buffer)
{
    if ( buffer.size() < 4 )
    {
        return -1; // 不足以檢查第四個字節
    }

    unsigned char byte4 = static_cast<unsigned char>( buffer[4] );
    int length = 8; // 最小長度

    // 根據 byte4 的位來增加封包長度
    if ( byte4 & 0x02 ) length += 2; // bit 1
    if ( byte4 & 0x04 ) length += 4; // bit 2
    if ( byte4 & 0x08 ) length += 2; // bit 3

    return length;
}

// 計算封包的校驗和的函數
bool verifyChecksum(const QByteArray &packet)
{
    if ( packet.size() < 2 )
    {
        return false; // 封包不足以進行校驗和檢查
    }

    // 計算封包數據（不包括最後兩個字節）的校驗和
    quint16 checksum = 0;
    for ( int i = 0; i < packet.size() - 2; i++ )
    {
        checksum += static_cast<unsigned char>(packet[i]);
    }

    // 獲取封包中的校驗和
    quint16 packetChecksum = static_cast<unsigned char>(packet[packet.size() - 2]) << 8 |
                             static_cast<unsigned char>(packet[packet.size() - 1]);

    // 比較計算出的校驗和和封包中的校驗和
    return checksum == packetChecksum;
}

// 將 QByteArray 轉換為大寫的十六進制字符串，並在每個字節之間插入空格
QString byteArrayToHexString(const QByteArray &byteArray)
{
    QString hexString = byteArray.toHex().toUpper();
    QString spacedHexString;
    for ( int i = 0; i < hexString.size(); i += 2 )
    {
        if ( i > 0 )
        {
            spacedHexString.append(' ');
        }
        spacedHexString.append(hexString.mid(i, 2));
    }
    return spacedHexString;
}

void MainWindow::processPacket(const QByteArray &packet)
{
    if ( static_cast<unsigned char>(packet[3]) == 0x02 )
    {
        static unsigned char past_cnt = 0;
        unsigned char byte4 = ( ( static_cast<unsigned char>( packet[4] ) & 0x0f ) >> 1u ) ;
        unsigned char servo_freshcnt = ( ( static_cast<unsigned char>( packet[4] ) & 0xf0 ) >> 4u ) ;
        unsigned char diff = ( servo_freshcnt - past_cnt ) & 0x0f;
        past_cnt = servo_freshcnt;

        qDebug() << "Servo Freshness Count Increase value:" << diff << "\n";
        switch ( byte4 )
        {
            default: {
            case 0:

                break;
            }
            case 1: {
                if ( packet.size() == 10 )
                {
                    const unsigned char *packetPtr = reinterpret_cast<const unsigned char *> ( packet.data() + 6 );
                    uint16_t pos = 0;
                    memcpy(&pos, packetPtr, sizeof(uint16_t));

                    PositionLabel->setText(QString("Position: ( %1 ) us").arg(pos));
                    CurrentLabel->setText(QString("Current: N/A"));
                    VoltageLabel->setText(QString("Voltage: N/A"));
                    TemperatureLabel->setText(QString("Temperature: N/A"));
                }
                break;
            }
            case 2: {
                if ( packet.size() == 12 )
                {
                    const unsigned char *packetPtr = reinterpret_cast<const unsigned char *> ( packet.data() + 6 );
                    uint16_t curr = 0, volt = 0;
                    float current = 0.0f, voltage = 0.0f;

                    memcpy(&curr, packetPtr + 0, sizeof(uint16_t));
                    memcpy(&volt, packetPtr + 2, sizeof(uint16_t));

                    current = curr / 100.0f;
                    voltage = volt / 100.0f;

                    PositionLabel->setText(QString("Position: N/A"));
                    CurrentLabel->setText(QString("Current: ( %1 ) A").arg(QString::number(current, 'f', 2)));
                    VoltageLabel->setText(QString("Voltage: ( %1 ) V").arg(QString::number(voltage, 'f', 2)));
                    TemperatureLabel->setText(QString("Temperature: N/A"));
                }
                break;
            }
            case 3: {
                if ( packet.size() == 14 )
                {
                    const unsigned char *packetPtr = reinterpret_cast<const unsigned char *> ( packet.data() + 6 );
                    uint16_t pos = 0, curr = 0, volt = 0;
                    float current = 0.0f, voltage = 0.0f;

                    memcpy(&pos, packetPtr + 0, sizeof(uint16_t));
                    memcpy(&curr, packetPtr + 2, sizeof(uint16_t));
                    memcpy(&volt, packetPtr + 4, sizeof(uint16_t));

                    current = curr / 100.0f;
                    voltage = volt / 100.0f;

                    PositionLabel->setText(QString("Position: ( %1 ) us").arg(pos));
                    CurrentLabel->setText(QString("Current: ( %1 ) A").arg(QString::number(current, 'f', 2)));
                    VoltageLabel->setText(QString("Voltage: ( %1 ) V").arg(QString::number(voltage, 'f', 2)));
                    TemperatureLabel->setText(QString("Temperature: N/A"));
                }
                break;
            }
            case 4: {
                if ( packet.size() == 10 )
                {
                    const unsigned char *packetPtr = reinterpret_cast<const unsigned char *> ( packet.data() + 6 );
                    uint16_t temp = 0;

                    memcpy(&temp, packetPtr + 0, sizeof(uint16_t));

                    PositionLabel->setText(QString("Position: N/A"));
                    CurrentLabel->setText(QString("Current: N/A"));
                    VoltageLabel->setText(QString("Voltage: N/A"));
                    TemperatureLabel->setText(QString("Temperature: ( %1 ) °C").arg(temp));
                }
                break;
            }
            case 5: {
                if ( packet.size() == 12 )
                {
                    const unsigned char *packetPtr = reinterpret_cast<const unsigned char *> ( packet.data() + 6 );
                    uint16_t pos = 0, temp = 0;

                    memcpy(&pos, packetPtr + 0, sizeof(uint16_t));
                    memcpy(&temp, packetPtr + 2, sizeof(uint16_t));

                    PositionLabel->setText(QString("Position: ( %1 ) us").arg(pos));
                    CurrentLabel->setText(QString("Current: N/A"));
                    VoltageLabel->setText(QString("Voltage: N/A"));
                    TemperatureLabel->setText(QString("Temperature: ( %1 ) °C").arg(temp));
                }
                break;
            }
            case 6: {
                if ( packet.size() == 14 )
                {
                    const unsigned char *packetPtr = reinterpret_cast<const unsigned char *> ( packet.data() + 6 );
                    uint16_t curr = 0, volt = 0, temp = 0;
                    float current = 0.0f, voltage = 0.0f;

                    memcpy(&curr, packetPtr + 0, sizeof(uint16_t));
                    memcpy(&volt, packetPtr + 2, sizeof(uint16_t));
                    memcpy(&temp, packetPtr + 4, sizeof(uint16_t));

                    current = curr / 100.0f;
                    voltage = volt / 100.0f;

                    PositionLabel->setText(QString("Position: N/A"));
                    CurrentLabel->setText(QString("Current: ( %1 ) A").arg(QString::number(current, 'f', 2)));
                    VoltageLabel->setText(QString("Voltage: ( %1 ) V").arg(QString::number(voltage, 'f', 2)));
                    TemperatureLabel->setText(QString("Temperature: ( %1 ) °C").arg(temp));
                }
                break;
            }
            case 7: {
                if ( packet.size() == 16 )
                {
                    const unsigned char *packetPtr = reinterpret_cast<const unsigned char *> ( packet.data() + 6 );
                    uint16_t pos = 0, curr = 0, volt = 0, temp = 0;
                    float current = 0.0f, voltage = 0.0f;

                    memcpy(&pos, packetPtr + 0, sizeof(uint16_t));
                    memcpy(&curr, packetPtr + 2, sizeof(uint16_t));
                    memcpy(&volt, packetPtr + 4, sizeof(uint16_t));
                    memcpy(&temp, packetPtr + 6, sizeof(uint16_t));

                    current = curr / 100.0f;
                    voltage = volt / 100.0f;

                    PositionLabel->setText(QString("Position: ( %1 ) us").arg(pos));
                    CurrentLabel->setText(QString("Current: ( %1 ) A").arg(QString::number(current, 'f', 2)));
                    VoltageLabel->setText(QString("Voltage: ( %1 ) V").arg(QString::number(voltage, 'f', 2)));
                    TemperatureLabel->setText(QString("Temperature: ( %1 ) °C").arg(temp));
                }
                break;
            }
        }
    } else
    {

    }
    /*
    float position = *reinterpret_cast<const float*>(packet.data() + 4);
    float current = *reinterpret_cast<const float*>(packet.data() + 8);
    float voltage = *reinterpret_cast<const float*>(packet.data() + 12);
    float temperature = *reinterpret_cast<const float*>(packet.data() + 16);
    */

}

void MainWindow::handleDataReceived(const QByteArray &data)
{
    if ( data.isEmpty() ) return;

    rxBuf.append(data);

    // 處理緩衝區中的數據
    while ( rxBuf.size() >= 8 ) // 確保有至少8個字節可以檢查 header1 和 header2 以及最小封包長度
    {
        if ( static_cast<unsigned char>(rxBuf[0]) == 0xbc &&
             static_cast<unsigned char>(rxBuf[1]) == 0x73 )
        {
            int packetLength = 16; // 預設封包長度為 16
            if ( static_cast<unsigned char>(rxBuf[3]) == 0x02 )
            {
                // 如果第三個字節為 0x02，計算封包長度
                packetLength = calculatePacketLength(rxBuf);
                if ( packetLength == -1 )
                {
                    // 不足以檢查第四個字節，等待更多數據
                    break;
                }

            }
            // 檢查是否有足夠的數據形成完整封包
            if ( rxBuf.size() >= packetLength )
            {
                // 獲取完整封包數據
                QByteArray packet = rxBuf.left(packetLength);

                // 處理封包
                if ( verifyChecksum(packet) )
                {
                    //qDebug() << "Received data:" << byteArrayToHexString(packet) << "\n";
                    processPacket(packet);
                } else
                {

                }
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
            // 非預期的頭部標記，移除第一個字節並繼續檢查
            rxBuf.remove(0, 1);
        }
    }
    //qDebug() << "Remained data:" << byteArrayToHexString(rxBuf) << "\n";
}

void MainWindow::actuateCommand()
{
    static actuateCommand_req_t req;

    uint8_t mode = ( (uint8_t) ( ui->ckb_act->isChecked() ) << 0u ) | ( (uint8_t) ( ui->ckb_pos->isChecked()  ) << 1u ) | \
                   ( (uint8_t) ( ui->ckb_cir->isChecked() ) << 2u ) | ( (uint8_t) ( ui->ckb_temp->isChecked() ) << 3u );

    ui->progressBar->setValue(setpoint);

    mks_equipment_actuator_actuateCommand_encode(&req, 0, mode, setpoint);

    QByteArray byteArray(reinterpret_cast<char*>(&req), sizeof(req));

    SerialPortManager::instance().writeData(byteArray, ( mode & 0x0e ) );

    req.byte4.fcn.fsh_cnt++;
}

void MainWindow::writeData(const QByteArray &data, bool expectReply)
{
    SerialPortManager::instance().writeData(data, expectReply);
}

void MainWindow::onAddActuatorPanel()
{
    ActuatorPanel *dialog = new ActuatorPanel(worker ,this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void MainWindow::onButtonLeftClicked()
{
    setpoint = 900;
}

void MainWindow::onButtonMidClicked()
{
    setpoint = 1500;
}

void MainWindow::onButtonRightClicked()
{
    setpoint = 2100;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_A:
            setpoint = 900;
            break;
        case Qt::Key_S:
            setpoint = 1500;
            break;
        case Qt::Key_D:
            setpoint = 2100;
            break;
        default:
            QMainWindow::keyPressEvent(event);
            break;
    }

    // 在這裡你可以執行其他操作，例如更新UI等

    event->accept(); // 標記事件已經被處理
}
