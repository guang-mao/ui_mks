/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QCheckBox *ckb_temp;
    QCheckBox *ckb_cir;
    QCheckBox *ckb_pos;
    QCheckBox *ckb_act;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        ckb_temp = new QCheckBox(centralwidget);
        ckb_temp->setObjectName("ckb_temp");
        ckb_temp->setGeometry(QRect(30, 20, 151, 21));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        ckb_temp->setFont(font);
        ckb_cir = new QCheckBox(centralwidget);
        ckb_cir->setObjectName("ckb_cir");
        ckb_cir->setGeometry(QRect(210, 20, 131, 21));
        ckb_cir->setFont(font);
        ckb_pos = new QCheckBox(centralwidget);
        ckb_pos->setObjectName("ckb_pos");
        ckb_pos->setGeometry(QRect(360, 20, 131, 21));
        ckb_pos->setFont(font);
        ckb_act = new QCheckBox(centralwidget);
        ckb_act->setObjectName("ckb_act");
        ckb_act->setGeometry(QRect(510, 20, 161, 21));
        ckb_act->setFont(font);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        ckb_temp->setText(QCoreApplication::translate("MainWindow", "Request Temperature", nullptr));
        ckb_cir->setText(QCoreApplication::translate("MainWindow", "Request Circuit", nullptr));
        ckb_pos->setText(QCoreApplication::translate("MainWindow", "Request Position", nullptr));
        ckb_act->setText(QCoreApplication::translate("MainWindow", "Activate Servo action", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
