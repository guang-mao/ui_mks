/********************************************************************************
** Form generated from reading UI file 'setup_window_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUP_WINDOW_DIALOG_H
#define UI_SETUP_WINDOW_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Setup_window_Dialog
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *Setup_window_Dialog)
    {
        if (Setup_window_Dialog->objectName().isEmpty())
            Setup_window_Dialog->setObjectName("Setup_window_Dialog");
        Setup_window_Dialog->resize(400, 300);
        pushButton = new QPushButton(Setup_window_Dialog);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(160, 250, 80, 23));

        retranslateUi(Setup_window_Dialog);

        QMetaObject::connectSlotsByName(Setup_window_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Setup_window_Dialog)
    {
        Setup_window_Dialog->setWindowTitle(QCoreApplication::translate("Setup_window_Dialog", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("Setup_window_Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Setup_window_Dialog: public Ui_Setup_window_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUP_WINDOW_DIALOG_H
