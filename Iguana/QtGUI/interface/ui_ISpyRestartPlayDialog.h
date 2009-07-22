/********************************************************************************
** Form generated from reading ui file 'ISpyRestartPlayDialog.ui'
**
** Created: Wed Jul 22 09:46:23 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef ISPYRESTARTPLAYDIALOG_H
#define ISPYRESTARTPLAYDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ISpyRestartPlayDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLCDNumber *lcdNumber;
    QLabel *label_3;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ISpyRestartPlayDialog)
    {
        if (ISpyRestartPlayDialog->objectName().isEmpty())
            ISpyRestartPlayDialog->setObjectName(QString::fromUtf8("ISpyRestartPlayDialog"));
        ISpyRestartPlayDialog->resize(452, 337);
        QIcon icon;
        icon.addPixmap(QPixmap(QString::fromUtf8(":/images/iguana-small.xpm")), QIcon::Normal, QIcon::Off);
        ISpyRestartPlayDialog->setWindowIcon(icon);
        ISpyRestartPlayDialog->setLayoutDirection(Qt::LeftToRight);
        ISpyRestartPlayDialog->setModal(true);
        gridLayout = new QGridLayout(ISpyRestartPlayDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(ISpyRestartPlayDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/images/IGUANA_SPLASH.xpm")));

        verticalLayout->addWidget(label_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(ISpyRestartPlayDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout->addWidget(label);

        lcdNumber = new QLCDNumber(ISpyRestartPlayDialog);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setFrameShape(QFrame::NoFrame);
        lcdNumber->setFrameShadow(QFrame::Plain);
        lcdNumber->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout->addWidget(lcdNumber);

        label_3 = new QLabel(ISpyRestartPlayDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);


        horizontalLayout_2->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(ISpyRestartPlayDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel);

        horizontalLayout_2->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(ISpyRestartPlayDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ISpyRestartPlayDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ISpyRestartPlayDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ISpyRestartPlayDialog);
    } // setupUi

    void retranslateUi(QDialog *ISpyRestartPlayDialog)
    {
        ISpyRestartPlayDialog->setWindowTitle(QApplication::translate("ISpyRestartPlayDialog", "iSpy is Idle", 0, QApplication::UnicodeUTF8));
        label_2->setText(QString());
        label->setText(QApplication::translate("ISpyRestartPlayDialog", "Auto-play mode will restart in", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ISpyRestartPlayDialog", "seconds.", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ISpyRestartPlayDialog);
    } // retranslateUi

};

namespace Ui {
    class ISpyRestartPlayDialog: public Ui_ISpyRestartPlayDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ISPYRESTARTPLAYDIALOG_H
