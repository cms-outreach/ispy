/********************************************************************************
** Form generated from reading ui file 'IgTreeViewForm.ui'
**
** Created: Tue Jun 10 14:27:14 2008
**      by: Qt User Interface Compiler version 4.4.0-beta1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IGTREEVIEWFORM_H
#define UI_IGTREEVIEWFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IgTreeViewForm
{
public:
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QTreeView *treeView;

    void setupUi(QDockWidget *IgTreeViewForm)
    {
    if (IgTreeViewForm->objectName().isEmpty())
        IgTreeViewForm->setObjectName(QString::fromUtf8("IgTreeViewForm"));
    IgTreeViewForm->resize(315, 628);
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8("../../Studio/interface/iguana-small.xpm")), QIcon::Normal, QIcon::Off);
    IgTreeViewForm->setWindowIcon(icon);
    dockWidgetContents = new QWidget(IgTreeViewForm);
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    dockWidgetContents->setGeometry(QRect(0, 20, 315, 608));
    gridLayout = new QGridLayout(dockWidgetContents);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    treeView = new QTreeView(dockWidgetContents);
    treeView->setObjectName(QString::fromUtf8("treeView"));
    treeView->setAlternatingRowColors(true);

    gridLayout->addWidget(treeView, 0, 0, 1, 1);

    IgTreeViewForm->setWidget(dockWidgetContents);

    retranslateUi(IgTreeViewForm);

    QMetaObject::connectSlotsByName(IgTreeViewForm);
    } // setupUi

    void retranslateUi(QDockWidget *IgTreeViewForm)
    {
    IgTreeViewForm->setWindowTitle(QApplication::translate("IgTreeViewForm", "Twig Explorer", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(IgTreeViewForm);
    } // retranslateUi

};

namespace Ui {
    class IgTreeViewForm: public Ui_IgTreeViewForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IGTREEVIEWFORM_H
