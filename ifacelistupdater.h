#ifndef IFACELISTUPDATER_H
#define IFACELISTUPDATER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QSerialPortInfo>
#include <QMap>

//#include <thread>  // std::this_thread::sleep_for
//#include <chrono>  // std::chrono::seconds


class BackgrondIfaceListUpdater : public QThread
{
    Q_OBJECT
public:
    explicit BackgrondIfaceListUpdater(QObject *parent = nullptr);
    ~BackgrondIfaceListUpdater();
    void start();
    void stop();
    QMap<QString, QString> get_list();
    static constexpr float UPDATE_INTERVAL = 0.5f;
    QMap<QString, QString> _ifaces;

protected:
    void run() override;

private:
    QMap<QString, QString> list_ifaces();
    bool _keepGoing;
    // 使用 mutable 允許在 const 成員函數中修改
    mutable QMutex _mutex;
};

#endif // IFACELISTUPDATER_H
