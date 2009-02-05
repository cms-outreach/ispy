/********************************************************************************
** Form generated from reading ui file 'IgOpenFileDialog.ui'
**
** Created: Tue Jun 10 14:27:14 2008
**      by: Qt User Interface Compiler version 4.4.0-beta1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IGOPENFILEDIALOG_H
#define UI_IGOPENFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>

QT_BEGIN_NAMESPACE

class Ui_IgOpenFileDialog
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *IgOpenFileDialog)
    {
    if (IgOpenFileDialog->objectName().isEmpty())
        IgOpenFileDialog->setObjectName(QString::fromUtf8("IgOpenFileDialog"));
    IgOpenFileDialog->resize(400, 300);
    buttonBox = new QDialogButtonBox(IgOpenFileDialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(30, 240, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    retranslateUi(IgOpenFileDialog);
    QObject::connect(buttonBox, SIGNAL(accepted()), IgOpenFileDialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), IgOpenFileDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(IgOpenFileDialog);
    } // setupUi

    void retranslateUi(QDialog *IgOpenFileDialog)
    {
    IgOpenFileDialog->setWindowTitle(QApplication::translate("IgOpenFileDialog", "Open File", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(IgOpenFileDialog);
    } // retranslateUi

};

namespace Ui {
    class IgOpenFileDialog: public Ui_IgOpenFileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IGOPENFILEDIALOG_H
