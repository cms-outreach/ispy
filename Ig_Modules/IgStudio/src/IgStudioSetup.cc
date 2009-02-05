/****************************************************************************
** Form implementation generated from reading ui file 'Ig_Modules/IgStudio/src/IgStudioSetup.ui'
**
** Created: Tue Aug 19 09:43:50 2003
**      by: The User Interface Compiler ($Id: IgStudioSetup.cc,v 1.6 2007/03/07 14:31:50 yana Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Ig_Modules/IgStudio/interface/IgStudioSetup.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a IgStudioSetup as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
IgStudioSetup::IgStudioSetup( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "IgStudioSetup" );
    IgStudioSetupLayout = new QGridLayout( this, 1, 1, 11, 6, "IgStudioSetupLayout"); 

    m_label = new QLabel( this, "m_label" );
    m_label->setAlignment( int( QLabel::AlignVCenter ) );

    IgStudioSetupLayout->addWidget( m_label, 0, 0 );

    m_layout2 = new QGridLayout( 0, 1, 1, 0, 6, "m_layout2"); 

    m_layout1 = new QGridLayout( 0, 1, 1, 0, 6, "m_layout1"); 
    QSpacerItem* spacer = new QSpacerItem( 20, 60, QSizePolicy::Minimum, QSizePolicy::Expanding );
    m_layout1->addItem( spacer, 2, 0 );

    m_cancel = new QPushButton( this, "m_cancel" );

    m_layout1->addWidget( m_cancel, 1, 0 );

    m_help = new QPushButton( this, "m_help" );

    m_layout1->addWidget( m_help, 3, 0 );

    m_ok = new QPushButton( this, "m_ok" );
    m_ok->setDefault( TRUE );

    m_layout1->addWidget( m_ok, 0, 0 );

    m_layout2->addLayout( m_layout1, 0, 1 );

    m_configs = new QListBox( this, "m_configs" );

    m_layout2->addWidget( m_configs, 0, 0 );

    IgStudioSetupLayout->addLayout( m_layout2, 1, 0 );
    languageChange();
    resize( QSize(350, 200).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( m_configs, SIGNAL( returnPressed(QListBoxItem*) ), this, SLOT( accept() ) );
    connect( m_configs, SIGNAL( doubleClicked(QListBoxItem*) ), this, SLOT( accept() ) );
    connect( m_cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( m_ok, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( m_help, SIGNAL( clicked() ), this, SLOT( helpSlot() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
IgStudioSetup::~IgStudioSetup()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void IgStudioSetup::languageChange()
{
    setCaption( tr( "IGUANA Setup" ) );
    m_label->setText( tr( "Please select configuration" ) );
    m_cancel->setText( tr( "&Cancel" ) );
    QToolTip::add( m_cancel, tr( "Terminate setup" ) );
    QWhatsThis::add( m_cancel, tr( "<p><b>Cancel Button:</b></p><p>Canceling setup will cause termination of the IGUANA application.</p>" ) );
    m_help->setText( tr( "&Help" ) );
    QToolTip::add( m_help, tr( "Enter \"What's This\" mode" ) );
    QWhatsThis::add( m_help, tr( "<p><b>Help Button:</b></p><p>Clicking on the button will change cursor in the <b><i>what's this</i></b> mode.</p>" ) );
    m_ok->setText( tr( "&OK" ) );
    QToolTip::add( m_ok, tr( "Load selected extension" ) );
    QWhatsThis::add( m_ok, tr( "<p><b>OK Button:</b></p><p>Accept <i><b>selected</b></i> extension from the configuration list to be loaded.<p></p>The extension can be loaded by left mouse button <b>double-click</b> on the selected item.</p>" ) );
    QToolTip::add( m_configs, tr( "List of all available extensions" ) );
    QWhatsThis::add( m_configs, tr( "<p>The list of currently available extensions is collected from plugin databases defined by <b>$IGUANA_PLUGINS</b> environment variable.</p>" ) );
}

void IgStudioSetup::helpSlot()
{
    QWhatsThis::enterWhatsThisMode ();
}

