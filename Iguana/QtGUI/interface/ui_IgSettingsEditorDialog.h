/********************************************************************************
** Form generated from reading ui file 'IgSettingsEditorDialog.ui'
**
** Created: Tue Mar 31 10:37:15 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IGSETTINGSEDITORDIALOG_H
#define UI_IGSETTINGSEDITORDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollArea>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IgSettingsEditorDialog
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QDialog *IgSettingsEditorDialog)
    {
        if (IgSettingsEditorDialog->objectName().isEmpty())
            IgSettingsEditorDialog->setObjectName(QString::fromUtf8("IgSettingsEditorDialog"));
        IgSettingsEditorDialog->resize(464, 338);
        QIcon icon;
        icon.addPixmap(QPixmap(QString::fromUtf8(":/images/iguana-small.xpm")), QIcon::Normal, QIcon::Off);
        IgSettingsEditorDialog->setWindowIcon(icon);
        gridLayout = new QGridLayout(IgSettingsEditorDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        buttonBox = new QDialogButtonBox(IgSettingsEditorDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);

        scrollArea = new QScrollArea(IgSettingsEditorDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 442, 282));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);


        retranslateUi(IgSettingsEditorDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), IgSettingsEditorDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), IgSettingsEditorDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(IgSettingsEditorDialog);
    } // setupUi

    void retranslateUi(QDialog *IgSettingsEditorDialog)
    {
        IgSettingsEditorDialog->setWindowTitle(QApplication::translate("IgSettingsEditorDialog", "iSpy Settings Editor", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(IgSettingsEditorDialog);
    } // retranslateUi

};

namespace Ui {
    class IgSettingsEditorDialog: public Ui_IgSettingsEditorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IGSETTINGSEDITORDIALOG_H
