#ifndef ACTUATORPANEL_H
#define ACTUATORPANEL_H

#include <QDialog>
#include <QTimer>
#include <QComboBox>
#include <QWidget>
#include <QCheckbox>
#include <QProgressBar>
#include <QLineEdit>
#include <QSlider>
#include <QHBoxLayout>
#include "commandworker.h"

class ActuatorPanel : public QDialog
{
    Q_OBJECT
public:
    ActuatorPanel(CommandWorker *worker, QWidget *parent = nullptr);
    ~ActuatorPanel();

public slots:
    void sendActuatorCommand();
    void onModeChanged(int index);

private:
    QTimer *timer;

    QHBoxLayout *TopLayout = nullptr;


    QComboBox *cbb_mode = nullptr;
    QWidget *mode1Widget = nullptr, *mode2Widget = nullptr, *mode3Widget = nullptr;
    QCheckBox *ckb_act = nullptr, *ckb_pos = nullptr, *ckb_cir = nullptr, *ckb_tmp = nullptr;
    QProgressBar *pgb_setpoint = nullptr;
    QLineEdit *le_left = nullptr, *le_mid = nullptr, *le_right = nullptr, *le_servo_id = nullptr;
    QSlider *slider_setpoint = nullptr;

    CommandWorker *commandWorker; // 保存CommandWorker指针的成員變量
    uint16_t setpoint;
};

#endif // ACTUATORPANEL_H
