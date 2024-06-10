/********************************************************************************
** Form generated from reading UI file 'setup_window.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUP_WINDOW_H
#define UI_SETUP_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Setup_window
{
public:
    QPushButton *ok_btn;
    QGroupBox *groupBox;
    QLabel *lbl_port;
    QComboBox *cbb_port;
    QLabel *lbl_baudrate;
    QComboBox *cbb_baudrate;

    void setupUi(QDialog *Setup_window)
    {
        if (Setup_window->objectName().isEmpty())
            Setup_window->setObjectName("Setup_window");
        Setup_window->resize(385, 193);
        ok_btn = new QPushButton(Setup_window);
        ok_btn->setObjectName("ok_btn");
        ok_btn->setGeometry(QRect(150, 160, 80, 23));
        groupBox = new QGroupBox(Setup_window);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(20, 10, 351, 141));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        groupBox->setFont(font);
        lbl_port = new QLabel(groupBox);
        lbl_port->setObjectName("lbl_port");
        lbl_port->setGeometry(QRect(90, 30, 162, 23));
        lbl_port->setAlignment(Qt::AlignCenter);
        cbb_port = new QComboBox(groupBox);
        cbb_port->setObjectName("cbb_port");
        cbb_port->setGeometry(QRect(29, 60, 291, 25));
        cbb_port->setEditable(false);
        lbl_baudrate = new QLabel(groupBox);
        lbl_baudrate->setObjectName("lbl_baudrate");
        lbl_baudrate->setGeometry(QRect(10, 100, 162, 23));
        lbl_baudrate->setAlignment(Qt::AlignCenter);
        cbb_baudrate = new QComboBox(groupBox);
        cbb_baudrate->setObjectName("cbb_baudrate");
        cbb_baudrate->setGeometry(QRect(180, 100, 140, 25));
        cbb_baudrate->setEditable(true);

        retranslateUi(Setup_window);

        QMetaObject::connectSlotsByName(Setup_window);
    } // setupUi

    void retranslateUi(QDialog *Setup_window)
    {
        Setup_window->setWindowTitle(QCoreApplication::translate("Setup_window", "Dialog", nullptr));
        ok_btn->setText(QCoreApplication::translate("Setup_window", "OK", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Setup_window", "COM Port interface setup", nullptr));
        lbl_port->setText(QCoreApplication::translate("Setup_window", "Select COM interface", nullptr));
        lbl_baudrate->setText(QCoreApplication::translate("Setup_window", "COM Port baudrate", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Setup_window: public Ui_Setup_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUP_WINDOW_H
