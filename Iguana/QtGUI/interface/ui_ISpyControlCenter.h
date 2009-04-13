/********************************************************************************
** Form generated from reading ui file 'ISpyControlCenter.ui'
**
** Created: Thu Mar 19 20:39:41 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IGSPYCONTROLCENTER_H
#define UI_IGSPYCONTROLCENTER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ISpyControlCenter
{
public:
    QWidget *tab;
    QWidget *tab1;

    void setupUi(QTabWidget *ISpyControlCenter)
    {
        if (ISpyControlCenter->objectName().isEmpty())
            ISpyControlCenter->setObjectName(QString::fromUtf8("ISpyControlCenter"));
        ISpyControlCenter->resize(321, 380);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setGeometry(QRect(0, 0, 317, 353));
        ISpyControlCenter->addTab(tab, QString());
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        tab1->setGeometry(QRect(0, 0, 455, 460));
        ISpyControlCenter->addTab(tab1, QString());

        retranslateUi(ISpyControlCenter);

        ISpyControlCenter->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ISpyControlCenter);
    } // setupUi

    void retranslateUi(QTabWidget *ISpyControlCenter)
    {
        ISpyControlCenter->setWindowTitle(QApplication::translate("ISpyControlCenter", "ControlCenter", 0, QApplication::UnicodeUTF8));
        ISpyControlCenter->setTabText(ISpyControlCenter->indexOf(tab), QApplication::translate("ISpyControlCenter", "RPhi Grid", 0, QApplication::UnicodeUTF8));
        ISpyControlCenter->setTabText(ISpyControlCenter->indexOf(tab1), QApplication::translate("ISpyControlCenter", "RZ Grid", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ISpyControlCenter);
    } // retranslateUi

};

namespace Ui {
    class ISpyControlCenter: public Ui_ISpyControlCenter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IGSPYCONTROLCENTER_H
