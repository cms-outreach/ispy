/****************************************************************************
** Form implementation generated from reading ui file 'IgWizardBase.ui'
**
** Created: Fri Mar 1 02:14:40 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "Ig_Modules/IgStudio/interface/IgWizardBase.h"

#include <qvariant.h>
#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

static const char* const image0_data[] = { 
"98 64 39 1",
"g c #313031",
"b c #316531",
"h c #316562",
"v c #319931",
"d c #319962",
"G c #31ce31",
"H c #623062",
"e c #626531",
"o c #626562",
"K c #62659c",
"q c #629931",
"# c #629962",
"p c #62999c",
"I c #62ce31",
"x c #62ce62",
"E c #62ce9c",
"J c #9c6562",
"F c #9c659c",
"f c #9c9962",
"n c #9c999c",
"C c #9c99cd",
"A c #9cce62",
"c c #9cce9c",
"u c #9ccecd",
"B c #9cff62",
"y c #9cff9c",
"D c #9cffcd",
"t c #cd999c",
"s c #cd99cd",
"w c #cdce9c",
"i c #cdcecd",
"m c #cdceff",
"z c #cdff9c",
"a c #cdffcd",
"k c #cdffff",
"l c #ffcecd",
"j c #ffceff",
"r c #ffffcd",
". c #ffffff",
"..................................................................................................",
"..................................................................................................",
"..................................................................................................",
"...................................................#..............................................",
"..................................................ab..............................................",
"..........cdebbdf.................................ce..............................................",
"........#bgehbebbbbi..............................#bcjkl..........................................",
"......ibg#ci..mn#obbbi............................bpqmrm..........................................",
".....ibbi.jaiiiiscnehboa..........................bi#iilk.........................................",
"....rheiajiiitiiisut#bbocj........................ec#iiil.........................................",
"....bbi.jiiiiiiiiiiiifbvbcj.......................bn#iuim.........................................",
"...wh#.aiiiiiiijajiiiinovbni......................##ni##i.........................................",
"...#b.jiiiii........mlinhqbni..................n..fbnne#ja........................................",
"...e#.iisiii...........iibdbnij................cc.ubqbniiij.......................................",
"..ihi.iiii.............iiibebnaj................b.ibhfu#ii........................................",
"..nb.iiiiaj..............iivhb#i................iqivx#botiij......................................",
"..fb.iiim................jii#eb#ii...............pevc##nniik......................................",
"..pb.liii..................ii#bbbcj...............ovyxnnnsil......................................",
"..cb.iii....................ii#ovv#i..............cbcyonnni.......................................",
"..ibmiiii....................ii#v#vqnj.............vqcy##nim......................................",
"..rbiiij......................jinbvxx#ij...........mqvyzyxfi......................................",
"...bfaiii.......................itdvAxx#i...........ib#y.kcfij..................wc................",
"...chii.i......................jaisvdxcA##cj.........nbAr.yhiir...............iv#.................",
"....bniji.........................is#b#Bycxq#ci.....#b#y.zvonCij............cbedbb#nnc............",
"....nbiiaj........................liinbvcyzDwE#q#ii#bca.xheonnia..........iv#v#vehq#ni............",
".....#bima..........................iin#bvcyaraacxdqy..avoooonsi.........ib#vbbhvnb#ncii..........",
".....rbeii...........................iisnobqxaa..raa...ivbehvvovni.......#v##.f##o##euilkj........",
"......i#bni............................iinn#bbxy........aczaazDzx##n.....qvqminen#onnniii.........",
".......i#ehcj..........................ijiCnnebvAa...............ay#qna..chvninnnnnnnsiirk........",
"........lueb#i...........................riiCnFhbqa................aix#nj.#qbCnnCnnwCwiml.........",
".......k.iiueb#i..........................j.iitn#hA....r..............ax#fnbdfnniiiiiimr..........",
"...........iiu#b#i............................minbx.axxxy...............Dcx#c#nniijmr.m...........",
".........lklmlii#b#j...........................lchx.Abbbbxz...............ar.c#tii................",
"...........r.iiiiibbi.......................in#niex.cooobbvGxyaar.............c#Cij...............",
"..............jajii#bii....................#bxxx#vy.aooHoobobbvvxxyar..........c#ci...............",
".................i.icbii.................c#bvA#Icz..aooooFnoooehbebvxyar........z#qni.............",
"..................i.icbcjk.............ib#.nbx#bvxa.rdoFnnnnnnooooobbbvya........kzx#fk...........",
"....................jjibnaj...........fbc..#bb#ndevyAbonniiiusnnnFnFoobqaazyyxxyr...aAnij.........",
"....................a.iibcja..........u....bob#onodbboonniljiiiiiCcobvhvvvdebebdxa...kAnia........",
"........................ienkj.............cbb#n#nJnonoonii.....i.libqvehoeoooHoehAa...kxnii.......",
".......................i.uqnrm...........jqp#n#FnKnnnnnnii........#vhcsnoKoooooobvca...a#sij......",
"........................j.ibci...........n#iei#fnntCtCnsi.........he#iinnFpFnnnnnovAar.r#nci......",
".........................rkienkl.........vj.din#Ciiiaiiajk.......rbx##fnnntcsiisin#bdBDaxonii.....",
"...........................kae#m........cc.l#ninwiij...r.........ivxqopnniiiiki.iiinbebvbonni.....",
"...........................j.i#qi......i#..k#iiiii.a.............ibdvevonim.......iinnobboonii....",
"..............................m#bi...iq#i..ibiiiii.j.............#fhbon##il........miCnnoFnni.....",
".............................r.i#bi.iEi...jcniji.j..............i#.f##bnnii.........riinnnnsii....",
".................................necj....rklkrmaia.................ib#ooniim..........liiciii.....",
"................................mrubcl.m.i.i.jaj....................cep#ntir...........klmrm......",
"..................................jiv#i.i.jr...rm..................jm#nnCiii...............r......",
"..................................aji#ei.i.k.j.....................r.iiiiii.......................",
".....................................knbiaj...........................riiimi......................",
"....................................lklchir..........................jk.lkr.......................",
"......................................a.cbi.i............................j........................",
"......................................j.invmr.....................................................",
"..........................................nimrm...................................................",
".........................................i.irmr...................................................",
"............................................mrm...................................................",
"..........................................rmr..r..................................................",
"............................................jkj...................................................",
"..................................................................................................",
"..................................................................................................",
"..................................................................................................",
".................................................................................................."};


/* 
 *  Constructs a IgWizardBase which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
IgWizardBase::IgWizardBase( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    QPixmap image0( ( const char** ) image0_data );
    if ( !name )
	setName( "IgWizardBase" );
    resize( 464, 330 ); 
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setCaption( trUtf8( "IGUANA Wizard" ) );
    IgWizardBaseLayout = new QVBoxLayout( this, 0, 0, "IgWizardBaseLayout"); 

    m_titleAreaLayout = new QVBoxLayout( 0, 0, 0, "m_titleAreaLayout"); 

    m_titleLayout = new QHBoxLayout( 0, 0, 0, "m_titleLayout"); 

    m_titlePixmap = new QLabel( this, "m_titlePixmap" );
    m_titlePixmap->setPixmap( image0 );
    m_titlePixmap->setScaledContents( FALSE );
    m_titleLayout->addWidget( m_titlePixmap );

    m_titleTextLayout = new QVBoxLayout( 0, 0, 0, "m_titleTextLayout"); 

    m_title = new QLabel( this, "m_title" );
    m_title->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, 0, 0, m_title->sizePolicy().hasHeightForWidth() ) );
    m_title->setMinimumSize( QSize( 220, 20 ) );
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 223, 223, 223) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, white );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, black );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, white );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, black );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, white );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, black );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setDisabled( cg );
    m_title->setPalette( pal );
    QFont m_title_font(  m_title->font() );
    m_title_font.setFamily( "tahoma" );
    m_title_font.setPointSize( 18 );
    m_title_font.setBold( TRUE );
    m_title->setFont( m_title_font ); 
    m_title->setText( trUtf8( "IGUANA Wizard" ) );
    m_title->setTextFormat( QLabel::PlainText );
    m_title->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );
    m_title->setIndent( -1 );
    m_titleTextLayout->addWidget( m_title );

    m_category = new QLabel( this, "m_category" );
    m_category->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, 0, 0, m_category->sizePolicy().hasHeightForWidth() ) );
    m_category->setMinimumSize( QSize( 220, 40 ) );
    cg.setColor( QColorGroup::Foreground, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 223, 223, 223) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, white );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, white );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, white );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setDisabled( cg );
    m_category->setPalette( pal );
    QFont m_category_font(  m_category->font() );
    m_category_font.setFamily( "tahoma" );
    m_category_font.setPointSize( 14 );
    m_category->setFont( m_category_font ); 
    m_category->setText( trUtf8( "Geometry" ) );
    m_category->setTextFormat( QLabel::PlainText );
    m_category->setAlignment( int( QLabel::AlignTop | QLabel::AlignLeft ) );
    m_category->setIndent( 2 );
    m_titleTextLayout->addWidget( m_category );
    m_titleLayout->addLayout( m_titleTextLayout );
    m_titleAreaLayout->addLayout( m_titleLayout );

    m_titleSeparator = new QFrame( this, "m_titleSeparator" );
    m_titleSeparator->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, m_titleSeparator->sizePolicy().hasHeightForWidth() ) );
    m_titleSeparator->setMinimumSize( QSize( 460, 2 ) );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, black );
    cg.setColor( QColorGroup::Midlight, QColor( 223, 223, 223) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, white );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, black );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, white );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, black );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, white );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, black );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setDisabled( cg );
    m_titleSeparator->setPalette( pal );
    m_titleSeparator->setProperty( "frameShape", (int)QFrame::HLine );
    m_titleSeparator->setFrameShadow( QFrame::Sunken );
    m_titleSeparator->setFrameShape( QFrame::HLine );
    m_titleAreaLayout->addWidget( m_titleSeparator );
    IgWizardBaseLayout->addLayout( m_titleAreaLayout );

    m_content = new QFrame( this, "m_content" );
    m_content->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)3, 0, 0, m_content->sizePolicy().hasHeightForWidth() ) );
    m_content->setMinimumSize( QSize( 0, 220 ) );
    m_content->setFrameShape( QFrame::StyledPanel );
    m_content->setFrameShadow( QFrame::Raised );
    m_content->setLineWidth( 0 );
    IgWizardBaseLayout->addWidget( m_content );

    m_bottomLayout = new QVBoxLayout( 0, 0, 0, "m_bottomLayout"); 

    m_bottomSeparator = new QFrame( this, "m_bottomSeparator" );
    m_bottomSeparator->setProperty( "frameShape", (int)QFrame::HLine );
    m_bottomSeparator->setFrameShadow( QFrame::Sunken );
    m_bottomSeparator->setFrameShape( QFrame::HLine );
    m_bottomLayout->addWidget( m_bottomSeparator );

    m_buttonLayout = new QHBoxLayout( 0, 0, 0, "m_buttonLayout"); 

    m_cancelButton = new QPushButton( this, "m_cancelButton" );
    m_cancelButton->setText( trUtf8( "Cancel" ) );
    m_buttonLayout->addWidget( m_cancelButton );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    m_buttonLayout->addItem( spacer );

    m_backButton = new QPushButton( this, "m_backButton" );
    m_backButton->setEnabled( FALSE );
    m_backButton->setText( trUtf8( "< Back" ) );
    m_buttonLayout->addWidget( m_backButton );

    m_nextButton = new QPushButton( this, "m_nextButton" );
    m_nextButton->setEnabled( FALSE );
    m_nextButton->setText( trUtf8( "Next >" ) );
    m_buttonLayout->addWidget( m_nextButton );

    m_finishButton = new QPushButton( this, "m_finishButton" );
    m_finishButton->setEnabled( FALSE );
    m_finishButton->setText( trUtf8( "Finish" ) );
    m_buttonLayout->addWidget( m_finishButton );

    m_helpButton = new QPushButton( this, "m_helpButton" );
    m_helpButton->setEnabled( FALSE );
    m_helpButton->setText( trUtf8( "Help" ) );
    m_buttonLayout->addWidget( m_helpButton );
    m_bottomLayout->addLayout( m_buttonLayout );
    IgWizardBaseLayout->addLayout( m_bottomLayout );

    // signals and slots connections
    connect( m_cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( m_backButton, SIGNAL( clicked() ), this, SLOT( back() ) );
    connect( m_nextButton, SIGNAL( clicked() ), this, SLOT( next() ) );
    connect( m_helpButton, SIGNAL( clicked() ), this, SLOT( help() ) );
    connect( m_finishButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
IgWizardBase::~IgWizardBase()
{
    // no need to delete child widgets, Qt does it all for us
}

void IgWizardBase::back()
{
    qWarning( "IgWizardBase::back(): Not implemented yet!" );
}

void IgWizardBase::help()
{
    qWarning( "IgWizardBase::help(): Not implemented yet!" );
}

void IgWizardBase::next()
{
    qWarning( "IgWizardBase::next(): Not implemented yet!" );
}

