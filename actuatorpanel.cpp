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

    QComboBox *cbb_mode = new QComboBox();
    cbb_mode->addItem("Step Mode");
    cbb_mode->addItem("Ramp Mode");
    cbb_mode->addItem("Manual Mode");

    // 模式容器
    // QWidget *mode1Widget = new QWidget();
    // QWidget *mode2Widget = new QWidget();
    // QWidget *mode3Widget = new QWidget();

    // 连接信号和槽
    connect(cbb_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(onModeChanged(int)));

    TopLayout = new QHBoxLayout();
}

ActuatorPanel::~ActuatorPanel()
{
    if ( timer )
    {
        timer->stop();
        delete timer;
    }
}

void ActuatorPanel::sendActuatorCommand()
{

    static actuateCommand_req_t req;

    mks_equipment_actuator_actuateCommand_encode(&req, 0, 1, setpoint);

    QByteArray byteArray(reinterpret_cast<char*>(&req), sizeof(req));

    commandWorker->commandQueue.enqueue(byteArray);

    req.byte4.fcn.fsh_cnt++;
}

void ActuatorPanel::onModeChanged(int index)
{

}
