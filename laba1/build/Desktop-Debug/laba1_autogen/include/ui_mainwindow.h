/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QWidget *gridWidget;
    QGridLayout *gridLayout;
    QTextEdit *inputTextField;
    QRadioButton *radioButton1Ipt;
    QRadioButton *radioButton3Ipt;
    QPlainTextEdit *outputTextField;
    QRadioButton *radioButton2Out;
    QPushButton *swapButton;
    QRadioButton *radioButton1Out;
    QRadioButton *radioButton3Out;
    QRadioButton *radioButton2Ipt;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *convertButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(591, 327);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridWidget = new QWidget(centralwidget);
        gridWidget->setObjectName("gridWidget");
        gridLayout = new QGridLayout(gridWidget);
        gridLayout->setObjectName("gridLayout");
        inputTextField = new QTextEdit(gridWidget);
        inputTextField->setObjectName("inputTextField");

        gridLayout->addWidget(inputTextField, 1, 1, 1, 1, Qt::AlignmentFlag::AlignHCenter);

        radioButton1Ipt = new QRadioButton(gridWidget);
        radioButton1Ipt->setObjectName("radioButton1Ipt");
        radioButton1Ipt->setChecked(true);

        gridLayout->addWidget(radioButton1Ipt, 3, 1, 1, 1, Qt::AlignmentFlag::AlignHCenter);

        radioButton3Ipt = new QRadioButton(gridWidget);
        radioButton3Ipt->setObjectName("radioButton3Ipt");

        gridLayout->addWidget(radioButton3Ipt, 5, 1, 1, 1, Qt::AlignmentFlag::AlignHCenter);

        outputTextField = new QPlainTextEdit(gridWidget);
        outputTextField->setObjectName("outputTextField");
        outputTextField->setReadOnly(true);

        gridLayout->addWidget(outputTextField, 1, 3, 1, 1);

        radioButton2Out = new QRadioButton(gridWidget);
        radioButton2Out->setObjectName("radioButton2Out");
        radioButton2Out->setChecked(false);

        gridLayout->addWidget(radioButton2Out, 4, 3, 1, 1, Qt::AlignmentFlag::AlignHCenter);

        swapButton = new QPushButton(gridWidget);
        swapButton->setObjectName("swapButton");

        gridLayout->addWidget(swapButton, 1, 2, 1, 1);

        radioButton1Out = new QRadioButton(gridWidget);
        radioButton1Out->setObjectName("radioButton1Out");

        gridLayout->addWidget(radioButton1Out, 3, 3, 1, 1, Qt::AlignmentFlag::AlignHCenter);

        radioButton3Out = new QRadioButton(gridWidget);
        radioButton3Out->setObjectName("radioButton3Out");

        gridLayout->addWidget(radioButton3Out, 5, 3, 1, 1, Qt::AlignmentFlag::AlignHCenter);

        radioButton2Ipt = new QRadioButton(gridWidget);
        radioButton2Ipt->setObjectName("radioButton2Ipt");

        gridLayout->addWidget(radioButton2Ipt, 4, 1, 1, 1, Qt::AlignmentFlag::AlignHCenter);


        gridLayout_2->addWidget(gridWidget, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(150, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 75, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 75, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 3, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(150, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        convertButton = new QPushButton(centralwidget);
        convertButton->setObjectName("convertButton");

        gridLayout_2->addWidget(convertButton, 2, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        radioButton1Ipt->setText(QCoreApplication::translate("MainWindow", "TEN", nullptr));
        radioButton3Ipt->setText(QCoreApplication::translate("MainWindow", "EIGHT", nullptr));
        radioButton2Out->setText(QCoreApplication::translate("MainWindow", "TWO", nullptr));
        swapButton->setText(QCoreApplication::translate("MainWindow", "<-->", nullptr));
        radioButton1Out->setText(QCoreApplication::translate("MainWindow", "TEN", nullptr));
        radioButton3Out->setText(QCoreApplication::translate("MainWindow", "EIGHT", nullptr));
        radioButton2Ipt->setText(QCoreApplication::translate("MainWindow", "TWO", nullptr));
        convertButton->setText(QCoreApplication::translate("MainWindow", "Convert", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
