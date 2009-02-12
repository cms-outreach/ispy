/********************************************************************************
** Form generated from reading ui file 'IggiControlCenter.ui'
**
** Created: Thu Jun 12 13:14:01 2008
**      by: Qt User Interface Compiler version 4.4.0-beta1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IGGICONTROLCENTER_H
#define UI_IGGICONTROLCENTER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IggiControlCenter
{
public:
    QWidget *tab;
    QWidget *tab1;

    void setupUi(QTabWidget *IggiControlCenter)
    {
    if (IggiControlCenter->objectName().isEmpty())
        IggiControlCenter->setObjectName(QString::fromUtf8("IggiControlCenter"));
    IggiControlCenter->resize(321, 380);
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    tab->setGeometry(QRect(0, 0, 317, 353));
    IggiControlCenter->addTab(tab, QString());
    tab1 = new QWidget();
    tab1->setObjectName(QString::fromUtf8("tab1"));
    tab1->setGeometry(QRect(0, 0, 455, 460));
    IggiControlCenter->addTab(tab1, QString());

    retranslateUi(IggiControlCenter);

    IggiControlCenter->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(IggiControlCenter);
    } // setupUi

    void retranslateUi(QTabWidget *IggiControlCenter)
    {
    IggiControlCenter->setWindowTitle(QApplication::translate("IggiControlCenter", "ControlCenter", 0, QApplication::UnicodeUTF8));
    IggiControlCenter->setTabText(IggiControlCenter->indexOf(tab), QApplication::translate("IggiControlCenter", "RPhi Grid", 0, QApplication::UnicodeUTF8));
    IggiControlCenter->setTabText(IggiControlCenter->indexOf(tab1), QApplication::translate("IggiControlCenter", "RZ Grid", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(IggiControlCenter);
    } // retranslateUi

};

namespace Ui {
    class IggiControlCenter: public Ui_IggiControlCenter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IGGICONTROLCENTER_H
