#include <QIcon>
#include "actuatordialog.h"
#include "ui_actuatordialog.h"
#include "mks.equipment.actuator.actuateCommand.h"

ActuatorDialog::ActuatorDialog(CommandWorker *worker, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ActuatorDialog)
    , commandWorker(worker)
    , setpoint(1500)
{
    ui->setupUi(this);

    setWindowTitle(tr("Actuator Panel"));

    setWindowIcon(QIcon(":/icons/servo.png"));

    // 設置進度條樣式以便文字顯示在中央
    ui->progressBar->setStyleSheet(
        "QProgressBar {"
        "    border: 1px solid grey;"
        "    border-radius: 5px;"
        "    text-align: center;" // 文字居中
        "    font-size: 16px;"    // 設置字體大小
        "    font-weight: bold;"  // 設置粗體
        "}"
        "QProgressBar::chunk {"
        //"    background-color: #05B8CC;"
        "    width: 20px;"
        "}");

    // 添加其他控件和逻辑
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ActuatorDialog::sendActuatorCommand);
    timer->start(50); // 每50ms触发一次

    // Set QIntValidator，只允許整數輸入
    QIntValidator *intValidator = new QIntValidator(0, 3000, this); // 限制输入范围为 0 到 3000
    ui->le_velocity->setValidator(intValidator);
    ui->le_velocity->setPlaceholderText("0~3000");

    // 預設Command Command在Step模式
    ui->tabWidget->setCurrentIndex(0);

    // 连接信号和槽
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &ActuatorDialog::onTabChanged);

    connect(ui->btn_tab0_left, &QPushButton::clicked, this, &ActuatorDialog::onButtonLeftClicked);
    connect(ui->btn_tab0_mid, &QPushButton::clicked, this, &ActuatorDialog::onButtonMidClicked);
    connect(ui->btn_tab0_right, &QPushButton::clicked, this, &ActuatorDialog::onButtonRightClicked);

    connect(ui->btn_tab1_left, &QPushButton::clicked, this, &ActuatorDialog::onButtonLeftClicked);
    connect(ui->btn_tab1_mid, &QPushButton::clicked, this, &ActuatorDialog::onButtonMidClicked);
    connect(ui->btn_tab1_right, &QPushButton::clicked, this, &ActuatorDialog::onButtonRightClicked);

    // 为所有子部件安装事件过滤器
    qApp->installEventFilter(this);
}

ActuatorDialog::~ActuatorDialog()
{
    if ( timer )
    {
        timer->stop();
        delete timer;
    }
    delete ui;
}

void ActuatorDialog::sendActuatorCommand()
{
    static actuateCommand_req_t req;

    uint8_t fcn = ( (uint8_t) ( ui->ckb_act->isChecked() ) << 0u ) | ( (uint8_t) ( ui->ckb_pos->isChecked() ) << 1u ) | \
                   ( (uint8_t) ( ui->ckb_cir->isChecked() ) << 2u ) | ( (uint8_t) ( ui->ckb_tmp->isChecked() ) << 3u );



    switch ( mode )
    {
        case STEP: {
            break;
        }
        case RAMP: {
            bool ok;
            QString text = ui->le_velocity->text();
            velocity = text.toInt(&ok) / 20.0f;
            if ( ok )
            {
                setpoint += ( ramp_direction * velocity );
                if ( setpoint >= 2100 )
                {
                    setpoint = 2100;
                    ramp_direction = -1;
                }
                else if ( setpoint <= 900 )
                {
                    setpoint = 900;
                    ramp_direction = 1;
                }
            }
            break;
        }
        case MANUAL: {
            setpoint = ui->horizontalSlider->value();
            break;
        }
    }

    ui->progressBar->setValue( (uint16_t) setpoint);

    mks_equipment_actuator_actuateCommand_encode(&req, 0, fcn, (uint16_t) setpoint);

    QByteArray byteArray(reinterpret_cast<char*>(&req), sizeof(req));

    commandWorker->commandQueue.enqueue(byteArray);

    req.byte4.fcn.fsh_cnt++;
}

void ActuatorDialog::onTabChanged(int index)
{
    // 重置
    ramp_direction = 0;

    setpoint = 1500;

    switch ( index )
    {
        default: {
        case STEP:
            mode = STEP;
            break;
        }
        case RAMP: {
            mode = RAMP;
            break;
        }
        case MANUAL: {
            mode = MANUAL;
            break;
        }
    }
}

void ActuatorDialog::onButtonLeftClicked()
{
    switch ( mode )
    {
        case STEP: {
            setpoint = 900;
            break;
        }
        case RAMP: {
            ramp_direction = -1;
            break;
        }
        case MANUAL: {
            break;
        }
    }
}

void ActuatorDialog::onButtonMidClicked()
{
    switch ( mode )
    {
        case STEP: {
            setpoint = 1500;
            break;
        }
        case RAMP: {
            setpoint = 1500;
            ramp_direction = 0;
            break;
        }
        case MANUAL: {
            break;
        }
    }
}

void ActuatorDialog::onButtonRightClicked()
{
    switch ( mode )
    {
        case STEP: {
            setpoint = 2100;
            break;
        }
        case RAMP: {
            ramp_direction = 1;
            break;
        }
        case MANUAL: {
            break;
        }
    }
}

void ActuatorDialog::keyPressEvent(QKeyEvent *event)
{
    switch ( event->key() )
    {
        case Qt::Key_A: {
            switch ( mode )
            {
                case STEP: {
                    ui->btn_tab0_left->click();
                    break;
                }
                case RAMP: {
                    ui->btn_tab1_left->click();
                    break;
                }
                case MANUAL: {
                    break;
                }
            }
            break;
        }
        case Qt::Key_S: {
            switch ( mode )
            {
                case STEP: {
                    ui->btn_tab0_mid->click();
                    break;
                }
                case RAMP: {
                    ui->btn_tab1_mid->click();
                    break;
                }
                case MANUAL: {
                    break;
                }
            }
            break;
        }
        case Qt::Key_D: {
            switch ( mode )
            {
                case STEP: {
                    ui->btn_tab0_right->click();
                    break;
                }
                case RAMP: {
                    ui->btn_tab1_right->click();
                    break;
                }
                case MANUAL: {
                    break;
                }
            }
            break;
        }
        default: {
            QWidget::keyPressEvent(event);
            break;
        }
    }
}
