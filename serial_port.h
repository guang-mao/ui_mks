#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <QMutex>
#include <QMutexLocker>
#include <QSerialPort>
#include <QThread>

extern QStringList baudrates;

class serial_port
{
public:
    serial_port();
    serial_port(const QString port, int baudrate);
    ~serial_port();

    QByteArray read_message();
    int write_message(uint8_t *txf, uint16_t length);

    bool start();
    void stop();

    QSerialPort *serialPort;
private:

    void initialize_defaults();

    /*
    int _open_port(const char*port);
    int _setup_port();
    */

    int _read_port();
    int _write_port(const char *buf, unsigned length);

    QString _port;
    int _baudrate;
    bool is_open;

    QMutex lock;

};

#endif // SERIAL_PORT_H
