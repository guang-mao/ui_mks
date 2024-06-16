#include "actuatorpanel.h"
#include "mks.equipment.actuator.actuateCommand.h"

ActuatorPanel::ActuatorPanel(CommandWorker *worker, QWidget *parent) : QDialog(parent), commandWorker(worker)
{
    // 在这里设置对话框的大小、添加控件等
    setWindowTitle(tr("Actuator Panel"));
    resize(400, 300);
    // 添加其他控件和逻辑
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ActuatorPanel::sendActuatorCommand);
    timer->start(50); // 每50ms触发一次
}

ActuatorPanel::~ActuatorPanel()
{
    if ( timer )
    {
        timer->stop();
        //delete timer;
    }
}

void ActuatorPanel::sendActuatorCommand()
{

    static actuateCommand_req_t req;

    //uint8_t mode = ( (uint8_t) ( ui->ckb_act->isChecked() ) << 0u ) | ( (uint8_t) ( ui->ckb_pos->isChecked()  ) << 1u ) | \
    //               ( (uint8_t) ( ui->ckb_cir->isChecked() ) << 2u ) | ( (uint8_t) ( ui->ckb_temp->isChecked() ) << 3u );

    //ui->progressBar->setValue(setpoint);

    mks_equipment_actuator_actuateCommand_encode(&req, 0, 1, setpoint);

    QByteArray byteArray(reinterpret_cast<char*>(&req), sizeof(req));

    commandWorker->commandQueue.enqueue(byteArray);

    req.byte4.fcn.fsh_cnt++;
}
