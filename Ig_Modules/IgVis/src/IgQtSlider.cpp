/****************************************************************************
** Form implementation generated from reading ui file 'c:\Graphics\Projects\IGUANA\Ig_Modules\IgVis\src\IgQtSlider.ui'
**
** Created: Tue Feb 18 14:15:50 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "Ig_Modules/IgVis/interface/IgQtSlider.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a IgQtSlider which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
IgQtSlider::IgQtSlider( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "Threshold_Panel" );
    resize( 261, 64 ); 
    setCaption( trUtf8( "Threshold Slider" ) );
    Threshold_PanelLayout = new QVBoxLayout( this, 6, 6, "Threshold_PanelLayout"); 

    GroupBox1 = new QGroupBox( this, "GroupBox1" );
    GroupBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, GroupBox1->sizePolicy().hasHeightForWidth() ) );
    GroupBox1->setTitle( trUtf8( "Threshold" ) );
    GroupBox1->setColumnLayout(0, Qt::Vertical );
    GroupBox1->layout()->setSpacing( 6 );
    GroupBox1->layout()->setMargin( 11 );
    GroupBox1Layout = new QVBoxLayout( GroupBox1->layout() );
    GroupBox1Layout->setAlignment( Qt::AlignTop );

    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 

    Slider = new QSlider( GroupBox1, "Slider" );
    Slider->setOrientation( QSlider::Horizontal );
    Layout1->addWidget( Slider );

    Units = new QLabel( GroupBox1, "Units" );
    Units->setText( trUtf8( "Arb Units" ) );
    Layout1->addWidget( Units );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    Layout1->addItem( spacer );

    SliderTextValue = new QLineEdit( GroupBox1, "SliderTextValue" );
    SliderTextValue->setMaximumSize( QSize( 60, 32767 ) );
    Layout1->addWidget( SliderTextValue );
    GroupBox1Layout->addLayout( Layout1 );
    Threshold_PanelLayout->addWidget( GroupBox1 );

    // signals and slots connections
    connect( SliderTextValue, SIGNAL( returnPressed() ), this, SLOT( textValueChangedSlot() ) );
    connect( Slider, SIGNAL( valueChanged(int) ), this, SLOT( sliderValueChangedSlot(int) ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
IgQtSlider::~IgQtSlider()
{
    // no need to delete child widgets, Qt does it all for us
}

void IgQtSlider::sliderValueChangedSlot(int)
{
    qWarning( "IgQtSlider::sliderValueChangedSlot(int): Not implemented yet!" );
}

void IgQtSlider::textValueChangedSlot()
{
    qWarning( "IgQtSlider::textValueChangedSlot(): Not implemented yet!" );
}

