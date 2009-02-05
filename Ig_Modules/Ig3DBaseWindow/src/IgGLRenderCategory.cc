//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DBaseWindow/interface/IgGLRenderCategory.h"
#include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlUInt.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <classlib/utils/DebugAids.h>
#include <qwhatsthis.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgGLRenderCategory, "/ControlCentre/Categories/View Properties/GL Render");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgGLRenderCategory::IgGLRenderCategory (IgState *state)
    : IgCommonViewCategory (state, "GL Render"),
      m_transparencyType(0),
      m_redrawPriority	(0),
      m_antialiasingPasses(0),
      m_antialiasing	(0),
      m_autoRedraw	(0),
      m_passUpdate	(0)
{
    addGLRenderTab ();
    state->put (s_key, this);    
}

IgGLRenderCategory::~IgGLRenderCategory (void)
{ state()->detach (s_key); }

void
IgGLRenderCategory::addGLRenderTab (void)
{
    QWidget	*renderOptions = new QWidget (topArea ());
    topLayout ()->addWidget (renderOptions);

    QVBoxLayout	*renderOptionsVLayout  = new QVBoxLayout (renderOptions);
    QGridLayout	*renderOptionsLayout   = new QGridLayout (renderOptionsVLayout);

    QLabel *transpTypeLabel = new QLabel (renderOptions, "transpTypeLabel");
    transpTypeLabel->setText (tr ("Transparency Type"));
    renderOptionsLayout->addWidget (transpTypeLabel, 0, 0);

    QLabel *redrawPriorityLabel = new QLabel (renderOptions, "redrawPriorityLabel");
    redrawPriorityLabel->setText (tr ("Redraw Priority"));
    renderOptionsLayout->addWidget (redrawPriorityLabel, 1, 0);

    m_antialiasing = new QCheckBox (renderOptions, "m_antialiasing");
    m_antialiasing->setText (tr ("Antialiasing Passes"));
    QWhatsThis::add (m_antialiasing, tr ("Enable / disable antialiasing."));
    renderOptionsLayout->addWidget (m_antialiasing, 2, 0);

    m_transparencyType = new QComboBox (FALSE, renderOptions, "m_transparencyType" );
    m_transparencyType->clear ();
    m_transparencyType->insertItem (tr ("Screen door"));
    m_transparencyType->insertItem (tr ("Add"));
    m_transparencyType->insertItem (tr ("Delayed add"));
    m_transparencyType->insertItem (tr ("Sorted object add"));
    m_transparencyType->insertItem (tr ("Blend"));
    m_transparencyType->insertItem (tr ("Delayed blend"));
    m_transparencyType->insertItem (tr ("Sorted object blend"));
    QWhatsThis::add (m_transparencyType, tr ("Select GL render transparency type."));
    renderOptionsLayout->addWidget (m_transparencyType, 0, 1);

    m_redrawPriority = new IgQtRangeControlUInt (renderOptions,
 					     IgQtRangeControl::StepperPart
 					     | IgQtRangeControl::ValuePart);
					 
    m_redrawPriority->setMinValue (1);
    m_redrawPriority->setLongStep (5);
    m_redrawPriority->setSmallStep (1);
    QWhatsThis::add (m_redrawPriority->widget (), tr ("Set redraw priority."));
    renderOptionsLayout->addWidget (m_redrawPriority->widget (), 1, 1);

    m_antialiasingPasses = new IgQtRangeControlUInt (renderOptions,
						 IgQtRangeControl::StepperPart
						 | IgQtRangeControl::ValuePart);
					 
    m_antialiasingPasses->setMinValue (1);
    m_antialiasingPasses->setLongStep (5);
    m_antialiasingPasses->setSmallStep (1);
    m_antialiasingPasses->widget ()->setEnabled (false);
    QWhatsThis::add (m_antialiasingPasses->widget (), tr ("Edit antialiasing passes."));
    renderOptionsLayout->addWidget (m_antialiasingPasses->widget (), 2, 1);

    m_autoRedraw = new QCheckBox (renderOptions, "m_autoRedraw");
    m_autoRedraw->setText (tr ("Auto Redraw"));
    QWhatsThis::add (m_autoRedraw, tr ("Enable / disable automatically redraw when scenegraph changes."));
    renderOptionsVLayout->addWidget (m_autoRedraw);

    m_passUpdate = new QCheckBox (renderOptions, "m_passUpdate");
    m_passUpdate->setText (tr ("Render Pass Update"));
    QWhatsThis::add (m_passUpdate, tr ("Enable / disable update on each render pass."));
    renderOptionsVLayout->addWidget (m_passUpdate);

    renderOptionsVLayout->addItem (new QSpacerItem ( 0, 0, QSizePolicy::Expanding,
						    QSizePolicy::Expanding));

    connect (m_transparencyType,SIGNAL (activated (int)),
             this,		SLOT   (setTransparencyType (int)));
    connect (m_redrawPriority,	SIGNAL (newValue (unsigned int)),
             this,		SLOT   (setRedrawPriority (unsigned int)));
    connect (m_antialiasingPasses,SIGNAL(newValue (unsigned int)),
             this,		SLOT   (setAntialiasingPasses (unsigned int)));
    connect (m_antialiasing,	SIGNAL (toggled (bool)),
             this,		SLOT   (setAntialiasing(bool)));
    connect (m_antialiasing,	SIGNAL (toggled (bool)),
             m_antialiasingPasses->widget (),SLOT(setEnabled (bool)));
    connect (m_autoRedraw,	SIGNAL (toggled (bool)),
             this,		SLOT   (setAutoRedraw(bool)));
    connect (m_passUpdate,	SIGNAL (toggled (bool)),
             this,		SLOT   (setPassUpdate(bool)));
    
    QWhatsThis::add (panel (), "Edit GL Render properties");
    QWhatsThis::add (renderOptions, "Edit GL Render properties");
}

void 
IgGLRenderCategory::browserChanged (Ig3DBaseBrowser *source)
{
    if (source)
    {
	IgCommonViewCategory::browserChanged (source);
	ASSERT (m_source->getGLRenderAction ());
	SoGLRenderAction	*ra = m_source->getGLRenderAction ();
	SbBool antialising;
	int    numPass;
	m_source->getAntialiasing (antialising, numPass);
      
	updateTransparencyType (ra->getTransparencyType ());
	m_redrawPriority->setValue (m_source->getRedrawPriority ());
	m_antialiasingPasses->setValue (numPass);
	m_antialiasing->setChecked (antialising);
	m_autoRedraw->setChecked (m_source->isAutoRedraw ());
	m_passUpdate->setChecked (ra->isPassUpdate ());
    }
}

//////////////////////////////////////////////////////////////////////////
void
IgGLRenderCategory::setTransparencyType (int type)
{
    ASSERT (m_source->getGLRenderAction ());
    SoGLRenderAction	*ra = m_source->getGLRenderAction ();
    switch (type)
    {
    case 0:
        ra->setTransparencyType (SoGLRenderAction::SCREEN_DOOR);
        break;
    case 1:
        ra->setTransparencyType (SoGLRenderAction::ADD);
        break;
    case 2:
        ra->setTransparencyType (SoGLRenderAction::DELAYED_ADD);
        break;
    case 3:
        ra->setTransparencyType (SoGLRenderAction::SORTED_OBJECT_ADD);
        break;
    case 4:
        ra->setTransparencyType (SoGLRenderAction::BLEND);
        break;
    case 5:
        ra->setTransparencyType (SoGLRenderAction::DELAYED_BLEND);
        break;
    case 6:
        ra->setTransparencyType (SoGLRenderAction::SORTED_OBJECT_BLEND);
        break;
    default:
        ASSERT (false);
    }
    updateTransparencyTypeHelp ();
}

void
IgGLRenderCategory::updateTransparencyType (int type)
{
    switch (type)
    {
    case SoGLRenderAction::SCREEN_DOOR:
        m_transparencyType->setCurrentItem (0);
        break;
    case SoGLRenderAction::ADD:
        m_transparencyType->setCurrentItem (1);
        break;
    case SoGLRenderAction::DELAYED_ADD:
        m_transparencyType->setCurrentItem (2);
        break;
    case SoGLRenderAction::SORTED_OBJECT_ADD:
        m_transparencyType->setCurrentItem (3);
        break;
    case SoGLRenderAction::BLEND:
        m_transparencyType->setCurrentItem (4);
        break;
    case SoGLRenderAction::DELAYED_BLEND:
        m_transparencyType->setCurrentItem (5);
        break;
    case SoGLRenderAction::SORTED_OBJECT_BLEND:
        m_transparencyType->setCurrentItem (6);
        break;
    default:
        ASSERT (false);
    }
    updateTransparencyTypeHelp ();
}

void
IgGLRenderCategory::updateTransparencyTypeHelp (void)
{
    switch (m_transparencyType->currentItem ())
    {
    case 0:
        QWhatsThis::add	(m_transparencyType, 
			 "Transparent triangles are rendered with a dither pattern. This is a "
			 "fast (on most GFX cards) but not-so-high-quality transparency mode.\n"
			 "One particular feature of this mode is that you are guaranteed that "
			 "it always renders the transparent parts of the scene correct with "
			 "regard to internal depth ordering of objects / polygons, something "
			 "which is not the case for any other transparency mode.");
        break;
    case 1:
        QWhatsThis::add (m_transparencyType, 
			 "Transparent objects are rendered using additive alpha blending. "
			 "Additive blending is probably mostly used to create special "
			 "transparency effects. The new pixel color is calculated as the "
			 "current pixel color plus the source pixel color multiplied with "
			 "the source pixel alpha value.");
        break;
    case 2:
        QWhatsThis::add (m_transparencyType, 
			 "Transparent objects are rendered using additive alpha blending, "
			 "in a second rendering pass with depth buffer updates disabled.");
        break;
    case 3:
        QWhatsThis::add (m_transparencyType, 
			 "Transparent objects are rendered using additive alpha blending. "
			 "Opaque objects are rendered first, and transparent objects are "
			 "rendered back to front with z-buffer updates disabled.");
        break;
    case 4:
        QWhatsThis::add (m_transparencyType, 
			 "Transparent objects are rendered using multiplicative alpha "
			 "blending.\n"	  
			 "Multiplicative alpha blending is the blending type that is most "
			 "often used to render transparent objects. The new pixel value is "
			 "calculated as the old pixel color multiplied with one minus the "
			 "source alpha value, plus the source pixel color multiplied with "
			 "the source alpha value.\n"
			 "We recommend that you use this transparency mode if you have only "
			 "one transparent object in your scene, and you know that it will be "
			 "rendered after the opaque objects.");
        break;
    case 5:
        QWhatsThis::add (m_transparencyType, 
			 "Transparent objects are rendered using multiplicative alpha "
			 "blending, in a second rendering pass with depth buffer "
			 "updates disabled.\n"
			 "Use this transparency type when you have one transparent object, "
			 "or several transparent object that you know will never overlap "
			 "(when projected to screen). Since the transparent objects are "
			 "rendered after opaque ones, you'll not have to worry about putting "
			 "the transparent objects at the end of your scene graph. It will not "
			 "be as fast as the BLEND transparency type, of course, since the "
			 "scene graph is traversed twice.");
        break;
    case 6:
	QWhatsThis::add (m_transparencyType, 
			 "Transparent objects are rendered using multiplicative alpha "
			 "blending, Opaque objects are rendered first, and transparent "
			 "objects are rendered back to front with z-buffer updates disabled.\n"
			 "Use this transparency mode when you have several transparent object "
			 "that you know might overlap (when projected to screen). This method "
			 "will require 1 + num_transparent_objects rendering passes. Path "
			 "traversal is used when rendering transparent objects, of course, "
			 "but it might still be slow if you have lots of state changes before "
			 "your transparent object. When using this mode, we recommend placing "
			 "the transparent objects as early as possible in the scene graph to "
			 "minimize traversal overhead.");
        break;
    default:
        ASSERT (false);
    }
}

void
IgGLRenderCategory::setRedrawPriority (unsigned int priority)
{ m_source->setRedrawPriority (int (priority)); }

void
IgGLRenderCategory::setAntialiasingPasses (unsigned int passes)
{ m_source->setAntialiasing (m_antialiasing->isChecked (), int(passes)); }

void
IgGLRenderCategory::setAntialiasing (bool option)
{ m_source->setAntialiasing (option, int (m_antialiasingPasses->value ())); }

void
IgGLRenderCategory::setAutoRedraw (bool option)
{ 
    bool old = m_source->isAutoRedraw ();
    if (old != option)
	m_source->setAutoRedraw (option);
}

void
IgGLRenderCategory::setPassUpdate (bool option)
{
    ASSERT (m_source->getGLRenderAction ());
    SoGLRenderAction	*ra = m_source->getGLRenderAction ();
    bool old = ra->isPassUpdate ();
    if (old != option)
	ra->setPassUpdate (option);
}

