/****************************************************************************
** Form implementation generated from reading ui file 'c:\Graphics\Projects\IGUANA\Ig_Modules\IgVis\src\IgQtXyzScale.ui'
**
** Created: Tue Feb 18 14:15:49 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "Ig_Modules/IgVis/interface/IgQtXyzScale.h"

#include <qvariant.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a IgQtXyzScale which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
IgQtXyzScale::IgQtXyzScale( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "Scale" );
    resize( 310, 167 ); 
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setCaption( trUtf8( "Scale" ) );
    setIconText( trUtf8( "Scale" ) );
    QToolTip::add( this, trUtf8( "Adjusts overall scales of the scene" ) );
    QWhatsThis::add( this, trUtf8( "Scale scene" ) );
    ScaleLayout = new QHBoxLayout( this, 11, 6, "ScaleLayout"); 

    GroupBox8 = new QGroupBox( this, "GroupBox8" );
    GroupBox8->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)5, 0, 0, GroupBox8->sizePolicy().hasHeightForWidth() ) );
    GroupBox8->setTitle( trUtf8( "" ) );
    GroupBox8->setColumnLayout(0, Qt::Vertical );
    GroupBox8->layout()->setSpacing( 6 );
    GroupBox8->layout()->setMargin( 11 );
    GroupBox8Layout = new QVBoxLayout( GroupBox8->layout() );
    GroupBox8Layout->setAlignment( Qt::AlignTop );

    Layout11 = new QVBoxLayout( 0, 0, 6, "Layout11"); 

    xLayout = new QHBoxLayout( 0, 0, 6, "xLayout"); 

    xCheckBox = new QCheckBox( GroupBox8, "xCheckBox" );
    xCheckBox->setText( trUtf8( "x" ) );
    QToolTip::add( xCheckBox, trUtf8( "enable x scaling" ) );
    xLayout->addWidget( xCheckBox );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Preferred, QSizePolicy::Minimum );
    xLayout->addItem( spacer );

    x_slider = new QSlider( GroupBox8, "x_slider" );
    x_slider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, x_slider->sizePolicy().hasHeightForWidth() ) );
    x_slider->setOrientation( QSlider::Horizontal );
    QToolTip::add( x_slider, trUtf8( "slider to adjust x scale" ) );
    xLayout->addWidget( x_slider );
    QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Preferred, QSizePolicy::Minimum );
    xLayout->addItem( spacer_2 );

    x_text_input = new QLineEdit( GroupBox8, "x_text_input" );
    x_text_input->setFrameShape( QLineEdit::LineEditPanel );
    x_text_input->setFrameShadow( QLineEdit::Sunken );
    QToolTip::add( x_text_input, trUtf8( "indicator/setter for x scale" ) );
    xLayout->addWidget( x_text_input );
    Layout11->addLayout( xLayout );

    yLayout = new QHBoxLayout( 0, 0, 6, "yLayout"); 

    yCheckBox = new QCheckBox( GroupBox8, "yCheckBox" );
    yCheckBox->setText( trUtf8( "y" ) );
    QToolTip::add( yCheckBox, trUtf8( "enable y scaling" ) );
    yLayout->addWidget( yCheckBox );
    QSpacerItem* spacer_3 = new QSpacerItem( 0, 0, QSizePolicy::Preferred, QSizePolicy::Minimum );
    yLayout->addItem( spacer_3 );

    y_slider = new QSlider( GroupBox8, "y_slider" );
    y_slider->setOrientation( QSlider::Horizontal );
    QToolTip::add( y_slider, trUtf8( "slider to adjust y scale" ) );
    yLayout->addWidget( y_slider );
    QSpacerItem* spacer_4 = new QSpacerItem( 0, 0, QSizePolicy::Preferred, QSizePolicy::Minimum );
    yLayout->addItem( spacer_4 );

    y_text_input = new QLineEdit( GroupBox8, "y_text_input" );
    QToolTip::add( y_text_input, trUtf8( "indicator/setter for y scale" ) );
    yLayout->addWidget( y_text_input );
    Layout11->addLayout( yLayout );

    zLayout = new QHBoxLayout( 0, 0, 6, "zLayout"); 

    zCheckBox = new QCheckBox( GroupBox8, "zCheckBox" );
    zCheckBox->setText( trUtf8( "z" ) );
    QToolTip::add( zCheckBox, trUtf8( "enable z scaling" ) );
    zLayout->addWidget( zCheckBox );
    QSpacerItem* spacer_5 = new QSpacerItem( 0, 0, QSizePolicy::Preferred, QSizePolicy::Minimum );
    zLayout->addItem( spacer_5 );

    z_slider = new QSlider( GroupBox8, "z_slider" );
    z_slider->setOrientation( QSlider::Horizontal );
    QToolTip::add( z_slider, trUtf8( "slider to adjust z scale" ) );
    zLayout->addWidget( z_slider );
    QSpacerItem* spacer_6 = new QSpacerItem( 0, 0, QSizePolicy::Preferred, QSizePolicy::Minimum );
    zLayout->addItem( spacer_6 );

    z_text_input = new QLineEdit( GroupBox8, "z_text_input" );
    QToolTip::add( z_text_input, trUtf8( "indicator/setter for z scale" ) );
    zLayout->addWidget( z_text_input );
    Layout11->addLayout( zLayout );
    GroupBox8Layout->addLayout( Layout11 );
    QSpacerItem* spacer_7 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    GroupBox8Layout->addItem( spacer_7 );

    Layout13 = new QHBoxLayout( 0, 0, 6, "Layout13"); 
    QSpacerItem* spacer_8 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout13->addItem( spacer_8 );

    CloseButton = new QPushButton( GroupBox8, "CloseButton" );
    CloseButton->setText( trUtf8( "Close" ) );
    QToolTip::add( CloseButton, trUtf8( "close this popup" ) );
    Layout13->addWidget( CloseButton );
    GroupBox8Layout->addLayout( Layout13 );
    ScaleLayout->addWidget( GroupBox8 );

    // signals and slots connections
    connect( CloseButton, SIGNAL( released() ), this, SLOT( close() ) );
    connect( xCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( xboxChecked(int) ) );
    connect( y_slider, SIGNAL( valueChanged(int) ), this, SLOT( ysliderValueChanged(int) ) );
    connect( z_slider, SIGNAL( valueChanged(int) ), this, SLOT( zsliderValueChanged(int) ) );
    connect( yCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( yboxChecked(int) ) );
    connect( zCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( zboxChecked(int) ) );
    connect( x_slider, SIGNAL( valueChanged(int) ), this, SLOT( xsliderValueChanged(int) ) );
    connect( x_text_input, SIGNAL( returnPressed() ), this, SLOT( xtextValueChanged() ) );
    connect( y_text_input, SIGNAL( returnPressed() ), this, SLOT( ytextValueChanged() ) );
    connect( z_text_input, SIGNAL( returnPressed() ), this, SLOT( ztextValueChanged() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
IgQtXyzScale::~IgQtXyzScale()
{
    // no need to delete child widgets, Qt does it all for us
}

void IgQtXyzScale::xboxChecked(int)
{
    qWarning( "IgQtXyzScale::xboxChecked(int): Not implemented yet!" );
}

void IgQtXyzScale::xsliderValueChanged(int)
{
    qWarning( "IgQtXyzScale::xsliderValueChanged(int): Not implemented yet!" );
}

void IgQtXyzScale::xtextValueChanged()
{
    qWarning( "IgQtXyzScale::xtextValueChanged(): Not implemented yet!" );
}

void IgQtXyzScale::yboxChecked(int)
{
    qWarning( "IgQtXyzScale::yboxChecked(int): Not implemented yet!" );
}

void IgQtXyzScale::ysliderValueChanged(int)
{
    qWarning( "IgQtXyzScale::ysliderValueChanged(int): Not implemented yet!" );
}

void IgQtXyzScale::ytextValueChanged()
{
    qWarning( "IgQtXyzScale::ytextValueChanged(): Not implemented yet!" );
}

void IgQtXyzScale::zboxChecked(int)
{
    qWarning( "IgQtXyzScale::zboxChecked(int): Not implemented yet!" );
}

void IgQtXyzScale::zsliderValueChanged(int)
{
    qWarning( "IgQtXyzScale::zsliderValueChanged(int): Not implemented yet!" );
}

void IgQtXyzScale::ztextValueChanged()
{
    qWarning( "IgQtXyzScale::ztextValueChanged(): Not implemented yet!" );
}

