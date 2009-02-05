/****************************************************************************
** Form interface generated from reading ui file 'c:\Graphics\Projects\Iguana\Ig_Modules\IgVis\src\IgQtPhysTabs.ui'
**
** Created: Wed Oct 5 11:59:43 2005
**      by: The User Interface Compiler ($Id: IgQtPhysTabs.h,v 1.9 2007/08/22 18:18:41 alverson Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef IGQTPHYSTABS_H
#define IGQTPHYSTABS_H

#include "Ig_Modules/IgVis/interface/config.h"
#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QSpacerItem;
class QTabWidget;
class QWidget;
class QPushButton;

class IG_VIS_API IgQtPhysTabs : public QDialog
{ 
    Q_OBJECT

public:
    IgQtPhysTabs( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~IgQtPhysTabs();

    QTabWidget* tabWidget;
    QWidget* Widget2;
    QWidget* Widget3;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;

protected:
    QVBoxLayout* IgQtPhysTabsLayout;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // IGQTPHYSTABS_H
