#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "commandworker.h"

#include <QTimer>
#include <QString>
#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>

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
    QTimer *timer;
    QByteArray rxBuf;
    QByteArray packet;
    uint16_t setpoint;

public slots:
    void handleDataReceived(const QByteArray &data);
    void actuateCommand();
    void writeData(const QByteArray &data, bool expectReply = false);
    void onAddActuatorPanel();
    void onButtonLeftClicked();
    void onButtonMidClicked();
    void onButtonRightClicked();

private:
    Ui::MainWindow *ui;
    CommandWorker *worker;
    void processPacket(const QByteArray &packet);
    QLabel *PositionLabel;
    QLabel *CurrentLabel;
    QLabel *VoltageLabel;
    QLabel *TemperatureLabel;
protected:
    void keyPressEvent(QKeyEvent *event) override;

};

#endif // MAINWINDOW_H
