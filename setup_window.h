#ifndef SETUP_WINDOW_H
#define SETUP_WINDOW_H

#include "ifacelistupdater.h"
#include <QDialog>

namespace Ui {
class Setup_window;
}

class Setup_window : public QDialog
{
    Q_OBJECT

public:
    explicit Setup_window(QWidget *parent = nullptr);
    ~Setup_window();

public slots:
    void update_iface_list();

private slots:
    void on_ok_btn_clicked();

private:
    Ui::Setup_window *ui;
    BackgrondIfaceListUpdater iface_lister;
    QTimer *timer;
};

#endif // SETUP_WINDOW_H
