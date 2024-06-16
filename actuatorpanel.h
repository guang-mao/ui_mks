#ifndef ACTUATORPANEL_H
#define ACTUATORPANEL_H

#include <QDialog>
#include <QTimer>
#include "commandworker.h"

class ActuatorPanel : public QDialog
{
    Q_OBJECT
public:
    ActuatorPanel(CommandWorker *worker, QWidget *parent = nullptr);
    ~ActuatorPanel();

public slots:
    void sendActuatorCommand();

private:
    QTimer *timer;
    CommandWorker *commandWorker; // 保存CommandWorker指针的成員變量
    uint16_t setpoint;
};

#endif // ACTUATORPANEL_H
