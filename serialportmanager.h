#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QSerialPort>
#include <QObject>
#include <QMutex>
#include <QTimer>

extern QStringList baudrates;

enum CommunicationState
{
    Idle,
    SendingCommand,
    WaitingForReply
};

class SerialPortManager : public QObject
{
    Q_OBJECT

public:
    static SerialPortManager& instance();
    void setupSerialPort(const QString &portName, int baudRate);
    void writeData(const QByteArray &data, bool expectReply);

signals:
    void dataReceived(const QByteArray &data);
    void errorOccurred(const QString &error);

private slots:
    void readData();
    void onTimeout();

private:
    QSerialPort m_serialPort;
    QMutex mutex; // 添加互斥锁
    QTimer timeoutTimer;
    CommunicationState currentState = Idle;
    QByteArray buffer;
    bool expectReply;
    SerialPortManager(); // Private constructor for singleton
    ~SerialPortManager(); // Destructor
    Q_DISABLE_COPY(SerialPortManager)
};

#endif // SERIALPORTMANAGER_H
