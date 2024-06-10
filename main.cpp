#include "mainwindow.h"
#include "setup_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Setup_window *setupDlg = new Setup_window;

    int result = setupDlg->exec();

    delete setupDlg;    // 確保 setupDlg 被刪除，觸發析構函數停止 QThread

    if ( result == QDialog::Accepted )
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    else
    {
        return 0;
    }
}
