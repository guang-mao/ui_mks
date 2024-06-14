#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QString>
#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>
//#include "generic_thread.h"
//#include "senderthread.h"
#include "serial_port.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    serial_port *ser;
    QTimer *timer;
    QByteArray rxBuf;
    QByteArray packet;
    uint16_t setpoint;
    //SenderThread *sender = nullptr;

public slots:
    void onReadyRead();
    void actuateCommand();
    void onButtonLeftClicked();
    void onButtonMidClicked();
    void onButtonRightClicked();

private:
    Ui::MainWindow *ui;

    void processPacket(const QByteArray &packet);
    QLabel *PositionLabel;
    QLabel *CurrentLabel;
    QLabel *VoltageLabel;
    QLabel *TemperatureLabel;
protected:
    void keyPressEvent(QKeyEvent *event) override;

};

#endif // MAINWINDOW_H
