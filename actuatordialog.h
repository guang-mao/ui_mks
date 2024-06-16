#ifndef ACTUATORDIALOG_H
#define ACTUATORDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QComboBox>
#include <QWidget>
#include <QCheckbox>
#include <QProgressBar>
#include <QLineEdit>
#include <QSlider>
#include <QKeyEvent>
#include "commandworker.h"

namespace Ui {
class ActuatorDialog;
}

enum ModeType
{
    STEP = 0,
    RAMP,
    MANUAL
};

class ActuatorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ActuatorDialog(CommandWorker *worker, QWidget *parent = nullptr);
    ~ActuatorDialog();

public slots:
    void sendActuatorCommand();
    void onTabChanged(int index);
    void onButtonLeftClicked();
    void onButtonMidClicked();
    void onButtonRightClicked();

private:
    Ui::ActuatorDialog *ui;

    QTimer *timer;
    QWidget *mode1Widget = nullptr, *mode2Widget = nullptr, *mode3Widget = nullptr;
    CommandWorker *commandWorker; // 保存CommandWorker指针的成員變量
    float setpoint, velocity = 0.0f;
    int8_t ramp_direction = 0;
    ModeType mode = STEP;

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // ACTUATORDIALOG_H
