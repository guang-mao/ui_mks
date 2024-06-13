#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QString>
#include <QMainWindow>
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
    //SenderThread *sender = nullptr;

public slots:
    void onReadyRead();
    void actuateCommand();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
