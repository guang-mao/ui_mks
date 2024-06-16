#include "setup_window.h"
#include "ui_setup_window.h"
#include "serialportmanager.h"

#include <QCompleter>
#include <QSerialPortInfo>
#include <QMap>
#include <QTimer>
#include <QDebug>

extern QString used_port;
extern int used_baudRate;

Setup_window::Setup_window(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Setup_window)
{
    ui->setupUi(this);
    // 設置對話框無標題欄
    //setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowTitle("Setup");
    //setAttribute(Qt::WA_DeleteOnClose);

    setWindowIcon(QIcon(":/icons/usb.png"));

    iface_lister.start();

    timer = new QTimer(this);   // 創建一個新的 QTimer 實例，並將其設置為 Setup_window 的子物件
    timer->setSingleShot(false);
    QObject::connect(timer, &QTimer::timeout, this, &Setup_window::update_iface_list); // 將 QTimer 的 timeout 信號連接到 update_iface_list 槽函數
    timer->start(int(BackgrondIfaceListUpdater::UPDATE_INTERVAL / 2 * 1000)); // 啟動定時器，並設置每 250 毫秒觸發一次 timeout 信號

    // baudrates from serial_port.cpp...
    ui->cbb_baudrate->addItems(baudrates);
    ui->cbb_baudrate->setCurrentText("115200");

    // 創建 QCompleter 並設置自動完成
    QCompleter *completer = new QCompleter(baudrates, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->cbb_baudrate->setCompleter(completer);

    // 不知道為何ui->ok_btn自動連接到Setup_window::on_ok_btn_clicked，所以我把它註解
    // connect(ui->ok_btn, &QPushButton::clicked, this, &Setup_window::on_ok_btn_clicked);
}

Setup_window::~Setup_window()
{
    iface_lister.stop();
    delete ui;
}

void Setup_window::update_iface_list()
{
    // Clear known keys
    QSet<QString> known_keys;
    QList<int> remove_indices;
    QMap<QString, QString> ifaces;

    ifaces = iface_lister.get_list();

    bool was_empty = ( ui->cbb_port->count() == 0 );

    // Marking known and scheduling for removal
    for ( int idx = 0; idx < ui->cbb_port->count(); ++idx )
    {
        QString tx = ui->cbb_port->itemText(idx);
        if ( tx.isEmpty() )
            break;

        known_keys.insert(tx);

        // 如果comboBox中的keys，在ifaces中沒有包含的話，準備刪除
        if ( !ifaces.contains(tx) )
        {
            qDebug() << "Removing iface" << tx;
            remove_indices.append(idx);
        }
    }

    // Removing items
    // 遍歷remove_indces，並將賦予給'idx'
    for ( int idx : remove_indices )
         ui->cbb_port->removeItem(idx);

    // Adding new items
    QStringList keys = ifaces.keys();
    for ( int i = keys.size() - 1; i >= 0; --i )
    {
        QString key = keys.at(i);
        if (!key.isEmpty() && !known_keys.contains(key))
        {
            qDebug() << "Adding iface" << key;
             ui->cbb_port->insertItem(0, key);
        }
    }

    // Updating selection
    if ( was_empty )
    {
         ui->cbb_port->setCurrentIndex(0);
    }
}

void Setup_window::on_ok_btn_clicked()
{
    iface_lister.stop();
    timer->stop();

    // 得到選擇的comboBox文本，並且刪除頭尾空格
    QString key = ui->cbb_port->currentText().trimmed();
    used_port = iface_lister._ifaces[key];

    // 得到選擇的comboBox文本，並且刪除頭尾空格
    used_baudRate = ui->cbb_baudrate->currentText().trimmed().toInt();
    // 發送信號，將設置的串口和鮑率傳遞給 MainWindow
    qDebug() << used_port << " baudRate:" << used_baudRate;

    accept();
}

