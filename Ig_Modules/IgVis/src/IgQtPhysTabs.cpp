/****************************************************************************
** Form implementation generated from reading ui file 'c:\Graphics\Projects\Iguana\Ig_Modules\IgVis\src\IgQtPhysTabs.ui'
**
** Created: Wed Oct 5 11:59:44 2005
**      by: The User Interface Compiler ($Id: IgQtPhysTabs.cpp,v 1.10 2005/10/06 02:32:47 alverson Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Ig_Modules/IgVis/interface/IgQtPhysTabs.h"

#include <qvariant.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a IgQtPhysTabs as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
IgQtPhysTabs::IgQtPhysTabs( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "IgQtPhysTabs" );
    setSizeGripEnabled( TRUE );
    IgQtPhysTabsLayout = new QVBoxLayout( this, 11, 6, "IgQtPhysTabsLayout"); 

    tabWidget = new QTabWidget( this, "tabWidget" );
    tabWidget->setTabShape( QTabWidget::Rounded );

    Widget2 = new QWidget( tabWidget, "Widget2" );
    tabWidget->insertTab( Widget2, QString::fromLatin1("") );

    Widget3 = new QWidget( tabWidget, "Widget3" );
    tabWidget->insertTab( Widget3, QString::fromLatin1("") );
    IgQtPhysTabsLayout->addWidget( tabWidget );

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 

    buttonHelp = new QPushButton( this, "buttonHelp" );
    buttonHelp->setEnabled( FALSE );
    buttonHelp->setAutoDefault( TRUE );
    Layout1->addWidget( buttonHelp );
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( FALSE );
    buttonOk->setDefault( FALSE );
    Layout1->addWidget( buttonOk );
    IgQtPhysTabsLayout->addLayout( Layout1 );
    languageChange();
    resize( QSize(314, 207).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
IgQtPhysTabs::~IgQtPhysTabs()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void IgQtPhysTabs::languageChange()
{
    setCaption( tr( "Physics Controls" ) );
    QToolTip::add( tabWidget, tr( "Each tab should control an associated set of physics cuts or options." ) );
    QWhatsThis::add( tabWidget, tr( "Physics Control widgets, stacked." ) );
    tabWidget->changeTab( Widget2, tr( "TBA" ) );
    tabWidget->changeTab( Widget3, tr( "TBA" ) );
    buttonHelp->setText( tr( "&Help" ) );
    buttonHelp->setAccel( QKeySequence( tr( "Alt+H" ) ) );
    buttonOk->setText( tr( "&Close" ) );
    buttonOk->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

