//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DBaseWindow/interface/IgSceneManagerCategory.h"
#include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlFloat.h"
#include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlUInt.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"
#include "Ig_Modules/Ig3DControls/interface/IgQtAxisAlignment.h"
#include "Ig_Modules/Ig3DControls/interface/Ig3DRotationControl.h"
#include "Ig_Modules/IgControlCentre/interface/IgStudioCentre.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCentre.h"
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <gl2ps.h>
#include <qhbox.h>
#include <qwidget.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qwidget.h>
#include <qcolordialog.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qtoolbox.h>
#include <classlib/utils/DebugAids.h>
#include <qapplication.h>
#include <qtoolbutton.h>
#include <qstyle.h>
#include <qstylefactory.h>
#include <qpushbutton.h>
#include <qwhatsthis.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgSceneManagerCategory, "/ControlCentre/Categories/View Properties/Scene Manager");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSceneManagerCategory::IgSceneManagerCategory (IgState *state)
    : IgCommonViewCategory (state, "Scene Manager"),
      m_background	(0),
      m_rgbMode		(0),
      m_realTimeUpdate	(0)
{
    addSceneManagerTab ();
    state->put (s_key, this);    
}

IgSceneManagerCategory::~IgSceneManagerCategory (void)
{ state()->detach (s_key); }

void
IgSceneManagerCategory::addSceneManagerTab (void)
{
    QWidget	*sceneManagerOptions = new QWidget (topArea ());
    topLayout ()->addWidget (sceneManagerOptions);
    
    QVBoxLayout	*sceneManagerVLayout  = new QVBoxLayout (sceneManagerOptions);
    QGridLayout	*sceneManagerLayout   = new QGridLayout (sceneManagerVLayout);

    QVBoxLayout *bodyLayout = new QVBoxLayout (0, 0, 6, "bodyLayout"); 

    QHBoxLayout *topLayout = new QHBoxLayout (0, 0, 6, "topLayout"); 

    //Scene Manager background color
    QLabel *bgColLabel = new QLabel (sceneManagerOptions, "bgColLabel");
    bgColLabel->setText (tr ("Background Color"));
    topLayout->addWidget (bgColLabel);

    m_background = new QFrame (sceneManagerOptions, "m_background");
    m_background->setMinimumSize (QSize (77, 0 ));
    m_background->setFrameShape (QFrame::StyledPanel);
    m_background->setFrameShadow (QFrame::Sunken);
    QSpacerItem* spacer1 = new QSpacerItem (21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    topLayout->addItem (spacer1);
    topLayout->addWidget (m_background);
    QSpacerItem* spacer2 = new QSpacerItem (21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    topLayout->addItem (spacer2);

    QPushButton *selectColor = new QPushButton (sceneManagerOptions, "selectColor" );
    selectColor->setText (tr ("&Select"));
    selectColor->setAccel (QKeySequence (tr ("Alt+S")));
    topLayout->addWidget (selectColor);
    bodyLayout->addLayout (topLayout);

    QVBoxLayout *bottomLayout = new QVBoxLayout (0, 0, 6, "bottomLayout"); 

    // Scene Manager RGB Mode
    m_rgbMode = new QCheckBox (sceneManagerOptions, "m_rgbMode");
    m_rgbMode->setText (tr ("RGB Mode"));
    bottomLayout->addWidget (m_rgbMode);

    // Real Time Update while animating
    m_realTimeUpdate = new QCheckBox (sceneManagerOptions, "m_realTimeUpdate");
    m_realTimeUpdate->setText (tr ("Real Time Update"));
    bottomLayout->addWidget (m_realTimeUpdate);
    bodyLayout->addLayout (bottomLayout);

    sceneManagerLayout->addLayout (bodyLayout, 0, 0);    
    QSpacerItem *spacer = new QSpacerItem (20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);
    sceneManagerVLayout->addItem (spacer);

    connect (selectColor,      	SIGNAL (clicked (void)),
	     this,		SLOT   (setBackgroundColor(void)));
    connect (m_rgbMode,		SIGNAL (toggled (bool)),
	     this,		SLOT   (setRGBMode(bool)));
    connect (m_realTimeUpdate,	SIGNAL (toggled (bool)),
	     this,		SLOT   (setRealTimeUpdate(bool)));

    // Add tooltips and what's this information
    QWhatsThis::add (bgColLabel, 
		     "Change the back ground color.");
    QWhatsThis::add (selectColor, 
		     "Change the back ground color.");
    QWhatsThis::add (m_background, 
		     "Change the back ground color.");
    QWhatsThis::add (m_rgbMode, 
		     "Turn RGB truecolor mode on or off. If you turn truecolor mode off, "
		     "colorindex mode will be used instead.");

    QWhatsThis::add (m_realTimeUpdate,
		     "Set whether or not for SoSceneManager instances to \"touch\" the "
		     "global realTime field after a redraw. If this is not done, redrawing "
		     "when animating the scene will only happen as fast as the realTime "
		     "interval goes (which defaults to 12 times a second).");
    QWhatsThis::add (panel (), "Edit 3D scene manager properties");
    QWhatsThis::add (sceneManagerOptions, "Edit 3D scene manager properties");
}

void 
IgSceneManagerCategory::browserChanged (Ig3DBaseBrowser *source)
{
    if (source)
    {
	IgCommonViewCategory::browserChanged (source);
	SbColor color (m_source->getSceneManager ()->getBackgroundColor ());
	QColor color1;
	color1.setRgb (int (color [0] * 255),
		       int (color [1] * 255),
		       int (color [2] * 255));
	m_background->setPaletteBackgroundColor (color1);
	m_rgbMode->setChecked (m_source->getSceneManager ()->isRGBMode ());
	m_realTimeUpdate->setChecked (m_source->getSceneManager ()->isRealTimeUpdateEnabled ());    
    }
}

//////////////////////////////////////////////////////////////////////////
void
IgSceneManagerCategory::setBackgroundColor (void)
{
    QColor color = 
	QColorDialog::getColor (m_background->paletteBackgroundColor (),
				m_source->getShellWidget (),
				"Select color");
    if (color.isValid ())
    {
	m_background->setPaletteBackgroundColor (color);
	setBackgroundColor (SbColor (color.red   () / 255.,
				     color.green () / 255.,
				     color.blue  () / 255.));
    }
}

//////////////////////////////////////////////////////////////////////////
void
IgSceneManagerCategory::setBackgroundColor (const SbColor& color)
{ m_source->getSceneManager ()->setBackgroundColor (color); }

void
IgSceneManagerCategory::setRGBMode (bool option)
{
    bool old = m_source->getSceneManager ()->isRGBMode ();
    if (old != option)
	m_source->getSceneManager ()->setRGBMode (option);
}

void
IgSceneManagerCategory::setRealTimeUpdate (bool option)
{
    bool old = m_source->getSceneManager ()->isRealTimeUpdateEnabled ();
    if (old != option)
	m_source->getSceneManager ()->enableRealTimeUpdate (option);
}
