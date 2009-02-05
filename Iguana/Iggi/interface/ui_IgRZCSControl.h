/********************************************************************************
** Form generated from reading ui file 'IgRZCSControl.ui'
**
** Created: Thu Jun 12 11:35:02 2008
**      by: Qt User Interface Compiler version 4.4.0-beta1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IGRZCSCONTROL_H
#define UI_IGRZCSCONTROL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_IgRZCSControl
{
public:
    QGridLayout *gridLayout;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QDoubleSpinBox *doubleSpinBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QDoubleSpinBox *doubleSpinBox_2;
    QCheckBox *checkBox_5;

    void setupUi(QDialog *IgRZCSControl)
    {
    if (IgRZCSControl->objectName().isEmpty())
        IgRZCSControl->setObjectName(QString::fromUtf8("IgRZCSControl"));
    IgRZCSControl->resize(187, 231);
    gridLayout = new QGridLayout(IgRZCSControl);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    checkBox = new QCheckBox(IgRZCSControl);
    checkBox->setObjectName(QString::fromUtf8("checkBox"));
    checkBox->setChecked(true);

    gridLayout->addWidget(checkBox, 0, 0, 1, 2);

    checkBox_2 = new QCheckBox(IgRZCSControl);
    checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

    gridLayout->addWidget(checkBox_2, 1, 0, 1, 2);

    checkBox_3 = new QCheckBox(IgRZCSControl);
    checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

    gridLayout->addWidget(checkBox_3, 2, 0, 1, 2);

    checkBox_4 = new QCheckBox(IgRZCSControl);
    checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
    checkBox_4->setChecked(true);

    gridLayout->addWidget(checkBox_4, 3, 0, 1, 2);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label = new QLabel(IgRZCSControl);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout_2->addWidget(label);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(horizontalSpacer);

    doubleSpinBox = new QDoubleSpinBox(IgRZCSControl);
    doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
    doubleSpinBox->setDecimals(1);
    doubleSpinBox->setMinimum(0.1);
    doubleSpinBox->setMaximum(5);
    doubleSpinBox->setValue(5);

    horizontalLayout_2->addWidget(doubleSpinBox);


    gridLayout->addLayout(horizontalLayout_2, 5, 0, 1, 2);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label_2 = new QLabel(IgRZCSControl);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout->addWidget(label_2);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer_2);

    doubleSpinBox_2 = new QDoubleSpinBox(IgRZCSControl);
    doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
    doubleSpinBox_2->setDecimals(1);
    doubleSpinBox_2->setMinimum(0.1);
    doubleSpinBox_2->setMaximum(1);
    doubleSpinBox_2->setSingleStep(0.1);
    doubleSpinBox_2->setValue(0.3);

    horizontalLayout->addWidget(doubleSpinBox_2);


    gridLayout->addLayout(horizontalLayout, 6, 0, 1, 2);

    checkBox_5 = new QCheckBox(IgRZCSControl);
    checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));
    checkBox_5->setChecked(true);

    gridLayout->addWidget(checkBox_5, 4, 0, 1, 1);


    retranslateUi(IgRZCSControl);
    QObject::connect(checkBox, SIGNAL(stateChanged(int)), IgRZCSControl, SLOT(setShowMeter(int)));
    QObject::connect(checkBox_2, SIGNAL(stateChanged(int)), IgRZCSControl, SLOT(setShowCentimeter(int)));
    QObject::connect(checkBox_3, SIGNAL(stateChanged(int)), IgRZCSControl, SLOT(setShowMillimeter(int)));
    QObject::connect(checkBox_4, SIGNAL(stateChanged(int)), IgRZCSControl, SLOT(setShowLabels(int)));
    QObject::connect(checkBox_5, SIGNAL(stateChanged(int)), IgRZCSControl, SLOT(setShowEtaLines(int)));
    QObject::connect(doubleSpinBox, SIGNAL(valueChanged(double)), IgRZCSControl, SLOT(setMaxEta(double)));
    QObject::connect(doubleSpinBox_2, SIGNAL(valueChanged(double)), IgRZCSControl, SLOT(setDeltaEta(double)));

    QMetaObject::connectSlotsByName(IgRZCSControl);
    } // setupUi

    void retranslateUi(QDialog *IgRZCSControl)
    {
    IgRZCSControl->setWindowTitle(QApplication::translate("IgRZCSControl", "Dialog", 0, QApplication::UnicodeUTF8));
    checkBox->setText(QApplication::translate("IgRZCSControl", "m  -----------", 0, QApplication::UnicodeUTF8));
    checkBox_2->setText(QApplication::translate("IgRZCSControl", "cm - - - - - -", 0, QApplication::UnicodeUTF8));
    checkBox_3->setText(QApplication::translate("IgRZCSControl", "mm . . . . . .", 0, QApplication::UnicodeUTF8));
    checkBox_4->setText(QApplication::translate("IgRZCSControl", "Labels", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("IgRZCSControl", "Max. Eta", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("IgRZCSControl", "Delta Eta", 0, QApplication::UnicodeUTF8));
    checkBox_5->setText(QApplication::translate("IgRZCSControl", "Eta Lines", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(IgRZCSControl);
    } // retranslateUi

};

namespace Ui {
    class IgRZCSControl: public Ui_IgRZCSControl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IGRZCSCONTROL_H
