#include "serial_port.h"
#include <QDebug>

QStringList baudrates = {"9600", "38400", "56000", "115200", "460800", "921600"};

serial_port::serial_port()
{

}

serial_port::serial_port(QString port, int baudrate)
{
    serialPort = new QSerialPort();
    initialize_defaults();
    _port = port;
    _baudrate = baudrate;
}

serial_port::~serial_port()
{
    stop();
    delete serialPort;
}

void serial_port::initialize_defaults()
{
    QMutexLocker locker(&lock);
    is_open = false;
    _port = nullptr;
    _baudrate = 115200;
    return;
}

QByteArray serial_port::read_message()
{
    //QMutexLocker locker(&lock);
    return serialPort->readAll();
}

int serial_port::write_message(uint8_t *txf, uint16_t length)
{
    // Write buffer to serial port, locks port while writing
    int bytesWritten = _write_port( (char *)txf, length);

    return bytesWritten;
}

bool serial_port::start()
{
    serialPort->setPortName(_port); // 根據您的實際串口名稱設置
    serialPort->setBaudRate(_baudrate);

    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if ( serialPort->open(QIODevice::ReadWrite) )
    {
        qDebug() << "串口已打開！";
        is_open = true;
    }
    else
    {
        qDebug() << "無法打開串口！";
        is_open = false;
    }
    return is_open;
}

void serial_port::stop()
{
    if ( serialPort->isOpen() )
    {
        qDebug() << "串口已關閉！";
        serialPort->close();
        is_open = false;
    }
    return;
}

int serial_port::_read_port()
{
    //QMutexLocker locker(&lock);

    //rxBuf = serialPort->readAll();

    return 0;//rxBuf.size();
}

int serial_port::_write_port(const char *buf, unsigned length)
{
    //QMutexLocker locker(&lock);
    int bytesWritten = serialPort->write(buf, length);
    serialPort->waitForBytesWritten(5);
    return bytesWritten;
}
