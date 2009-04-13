/********************************************************************************
** Form generated from reading ui file 'IgPolarCSControl.ui'
**
** Created: Thu Jun 12 11:38:06 2008
**      by: Qt User Interface Compiler version 4.4.0-beta1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IGPOLARCSCONTROL_H
#define UI_IGPOLARCSCONTROL_H

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
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_IgPolarCSControl
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QCheckBox *unitLabel;
    QSpacerItem *horizontalSpacer;
    QLineEdit *unitLabelText;
    QCheckBox *angleLabels;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *minorAngles;
    QSpacerItem *horizontalSpacer_2;
    QDoubleSpinBox *doubleSpinBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *unitDistLabel;
    QSpacerItem *horizontalSpacer_3;
    QDoubleSpinBox *doubleSpinBox_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QSpacerItem *horizontalSpacer_4;
    QDoubleSpinBox *doubleSpinBox_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_5;
    QDoubleSpinBox *doubleSpinBox_4;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer_5;

    void setupUi(QDialog *IgPolarCSControl)
    {
    if (IgPolarCSControl->objectName().isEmpty())
        IgPolarCSControl->setObjectName(QString::fromUtf8("IgPolarCSControl"));
    IgPolarCSControl->resize(252, 256);
    gridLayout = new QGridLayout(IgPolarCSControl);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    unitLabel = new QCheckBox(IgPolarCSControl);
    unitLabel->setObjectName(QString::fromUtf8("unitLabel"));
    unitLabel->setChecked(true);

    horizontalLayout->addWidget(unitLabel);

    horizontalSpacer = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    unitLabelText = new QLineEdit(IgPolarCSControl);
    unitLabelText->setObjectName(QString::fromUtf8("unitLabelText"));

    horizontalLayout->addWidget(unitLabelText);


    gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

    angleLabels = new QCheckBox(IgPolarCSControl);
    angleLabels->setObjectName(QString::fromUtf8("angleLabels"));
    angleLabels->setChecked(true);

    gridLayout->addWidget(angleLabels, 2, 0, 1, 1);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    minorAngles = new QCheckBox(IgPolarCSControl);
    minorAngles->setObjectName(QString::fromUtf8("minorAngles"));
    minorAngles->setChecked(true);

    horizontalLayout_2->addWidget(minorAngles);

    horizontalSpacer_2 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_2->addItem(horizontalSpacer_2);

    doubleSpinBox = new QDoubleSpinBox(IgPolarCSControl);
    doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
    doubleSpinBox->setMinimum(5);
    doubleSpinBox->setMaximum(360);
    doubleSpinBox->setValue(22.5);

    horizontalLayout_2->addWidget(doubleSpinBox);


    gridLayout->addLayout(horizontalLayout_2, 4, 0, 1, 1);

    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    unitDistLabel = new QLabel(IgPolarCSControl);
    unitDistLabel->setObjectName(QString::fromUtf8("unitDistLabel"));

    horizontalLayout_3->addWidget(unitDistLabel);

    horizontalSpacer_3 = new QSpacerItem(78, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_3->addItem(horizontalSpacer_3);

    doubleSpinBox_2 = new QDoubleSpinBox(IgPolarCSControl);
    doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
    doubleSpinBox_2->setDecimals(2);
    doubleSpinBox_2->setMinimum(0.01);
    doubleSpinBox_2->setMaximum(1000);
    doubleSpinBox_2->setValue(10);

    horizontalLayout_3->addWidget(doubleSpinBox_2);


    gridLayout->addLayout(horizontalLayout_3, 6, 0, 1, 1);

    horizontalLayout_4 = new QHBoxLayout();
    horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
    label = new QLabel(IgPolarCSControl);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout_4->addWidget(label);

    horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_4->addItem(horizontalSpacer_4);

    doubleSpinBox_3 = new QDoubleSpinBox(IgPolarCSControl);
    doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
    doubleSpinBox_3->setDecimals(1);
    doubleSpinBox_3->setMaximum(1000);

    horizontalLayout_4->addWidget(doubleSpinBox_3);


    gridLayout->addLayout(horizontalLayout_4, 8, 0, 1, 1);

    horizontalLayout_5 = new QHBoxLayout();
    horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
    label_2 = new QLabel(IgPolarCSControl);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout_5->addWidget(label_2);

    horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_5->addItem(horizontalSpacer_5);

    doubleSpinBox_4 = new QDoubleSpinBox(IgPolarCSControl);
    doubleSpinBox_4->setObjectName(QString::fromUtf8("doubleSpinBox_4"));
    doubleSpinBox_4->setDecimals(1);
    doubleSpinBox_4->setMaximum(1000);
    doubleSpinBox_4->setValue(170);

    horizontalLayout_5->addWidget(doubleSpinBox_4);


    gridLayout->addLayout(horizontalLayout_5, 10, 0, 1, 1);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);

    verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(verticalSpacer_2, 3, 0, 1, 1);

    verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(verticalSpacer_3, 5, 0, 1, 1);

    verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(verticalSpacer_4, 7, 0, 1, 1);

    verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout->addItem(verticalSpacer_5, 9, 0, 1, 1);


    retranslateUi(IgPolarCSControl);
    QObject::connect(unitLabel, SIGNAL(stateChanged(int)), IgPolarCSControl, SLOT(setShowUnitLabel(int)));
    QObject::connect(angleLabels, SIGNAL(stateChanged(int)), IgPolarCSControl, SLOT(setShowAngleLabels(int)));
    QObject::connect(minorAngles, SIGNAL(stateChanged(int)), IgPolarCSControl, SLOT(setShowMinorAngles(int)));
    QObject::connect(unitLabelText, SIGNAL(textChanged(QString)), IgPolarCSControl, SLOT(setUnitLabel(QString)));
    QObject::connect(doubleSpinBox, SIGNAL(valueChanged(double)), IgPolarCSControl, SLOT(setDeltaPhi(double)));
    QObject::connect(doubleSpinBox_2, SIGNAL(valueChanged(double)), IgPolarCSControl, SLOT(setEnergyUnit(double)));
    QObject::connect(doubleSpinBox_3, SIGNAL(valueChanged(double)), IgPolarCSControl, SLOT(setStartRadius(double)));
    QObject::connect(doubleSpinBox_4, SIGNAL(valueChanged(double)), IgPolarCSControl, SLOT(setEndRadius(double)));

    QMetaObject::connectSlotsByName(IgPolarCSControl);
    } // setupUi

    void retranslateUi(QDialog *IgPolarCSControl)
    {
    IgPolarCSControl->setWindowTitle(QApplication::translate("IgPolarCSControl", "Dialog", 0, QApplication::UnicodeUTF8));
    unitLabel->setText(QApplication::translate("IgPolarCSControl", "Unit label", 0, QApplication::UnicodeUTF8));
    unitLabelText->setText(QApplication::translate("IgPolarCSControl", "GeV", 0, QApplication::UnicodeUTF8));
    angleLabels->setText(QApplication::translate("IgPolarCSControl", "Angle labels", 0, QApplication::UnicodeUTF8));
    minorAngles->setText(QApplication::translate("IgPolarCSControl", "Minor angles", 0, QApplication::UnicodeUTF8));
    unitDistLabel->setText(QApplication::translate("IgPolarCSControl", "Unit distance", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("IgPolarCSControl", "Inner radius", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("IgPolarCSControl", "Outer radius", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(IgPolarCSControl);
    } // retranslateUi

};

namespace Ui {
    class IgPolarCSControl: public Ui_IgPolarCSControl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IGPOLARCSCONTROL_H
