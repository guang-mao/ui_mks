#include "serialportmanager.h"

QStringList baudrates = {"9600", "38400", "56000", "115200", "460800", "921600"};

SerialPortManager& SerialPortManager::instance()
{
    static SerialPortManager instance;
    return instance;
}

SerialPortManager::SerialPortManager()
{
    // Private constructor
    connect(&timeoutTimer, &QTimer::timeout, this, &SerialPortManager::onTimeout);
    timeoutTimer.setInterval(3); // 设置超时时间为3ms
    timeoutTimer.setSingleShot(true); // 设置为单次触发
}

SerialPortManager::~SerialPortManager()
{
    if ( m_serialPort.isOpen() )
    {
        m_serialPort.close();
    }
}

void SerialPortManager::setupSerialPort(const QString &portName, qint32 baudRate)
{
    if ( !m_serialPort.isOpen() )
    {
        m_serialPort.setPortName(portName);
        m_serialPort.setBaudRate(baudRate);
        if ( !m_serialPort.open(QIODevice::ReadWrite) )
        {
            emit errorOccurred( m_serialPort.errorString() );
            return;
        }
        connect(&m_serialPort, &QSerialPort::readyRead, this, &SerialPortManager::readData);
    }
}

void SerialPortManager::writeData(const QByteArray &data, bool expectReply = false)
{
    mutex.lock(); // 在寫操作前鎖定
    if ( m_serialPort.isOpen() )
    {
        this->expectReply = expectReply;
        m_serialPort.write(data);
        m_serialPort.waitForBytesWritten(-1); // 等待直到所有数据被写入
        if ( expectReply )
        {
            timeoutTimer.start();// 如果期待回饋，啟動逾時定時器
        }
    }
    mutex.unlock(); // 完成寫操作後解鎖
}

void SerialPortManager::readData()
{
    QByteArray data = m_serialPort.readAll();
    //buffer.append(data); // 將接收到的數據累積到緩衝區
    emit dataReceived(data);
}

void SerialPortManager::onTimeout()
{
    if ( expectReply )
    {
        // 逾時處理邏輯，僅在期待回饋封包時執行
        timeoutTimer.stop(); // 確保停止當前的逾時定時器
    }
}
