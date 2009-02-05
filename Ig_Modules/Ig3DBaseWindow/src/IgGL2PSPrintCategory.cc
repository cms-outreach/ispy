//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DBaseWindow/interface/IgGL2PSPrintCategory.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"
#include <gl2ps.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <classlib/utils/DebugAids.h>
#include <qwhatsthis.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
bool IgGL2PSPrintCategory::updated = false;
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgGL2PSPrintCategory, "/ControlCentre/Categories/View Properties/GL2PS Printing");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgGL2PSPrintCategory::IgGL2PSPrintCategory (IgState *state)
    : IgCommonViewCategory (state, "GL2PS Printing"),
      m_gl2psSimpleLineOffset (0),
      m_gl2psBestRoot         (0),
      m_gl2psOcclusionCulling (0),
      m_gl2psBackGround       (0),      
      m_gl2psLandScape        (0)
{
    addGL2PSOptionTab ();
    state->put (s_key, this);
}

IgGL2PSPrintCategory::~IgGL2PSPrintCategory (void)
{ state()->detach (s_key); }

// Print Options
void
IgGL2PSPrintCategory::addGL2PSOptionTab (void)
{
    QWidget	*printOptions = new QWidget (topArea ());
    topLayout ()->addWidget (printOptions);
    
    QVBoxLayout *printOptionsVLayout = new QVBoxLayout (printOptions, 5, 6, "printOptionsLayout");
    QGridLayout	*printOptionsLayout  = new QGridLayout (printOptionsVLayout);
    

    m_gl2psSimpleLineOffset = new QCheckBox (printOptions, "m_gl2psSimpleLineOffset");
    m_gl2psSimpleLineOffset->setText (tr ("Simple Line Offset"));
    QWhatsThis::add (m_gl2psSimpleLineOffset, tr ("Enable/disable simple line offset. "
                                                  "A small offset is added in the z-buffer "
						  "to all the lines in the scene graph."));

    printOptionsLayout->addWidget (m_gl2psSimpleLineOffset, 0, 0);
    
    m_gl2psBestRoot = new QCheckBox (printOptions, "m_gl2psBestRoot");
    m_gl2psBestRoot->setText (tr ("Best Root"));
    QWhatsThis::add (m_gl2psBestRoot, tr ("Enable/disable best root finding for BSP. "
    					  "The construction of the BSP tree is optimized "
					  "by choosing the root primitives leading to "
					  "the minimum number of splits."));

    printOptionsLayout->addWidget (m_gl2psBestRoot, 0, 1);
    
    m_gl2psOcclusionCulling = new QCheckBox (printOptions, "m_gl2psOcclusionCulling");
    m_gl2psOcclusionCulling->setText (tr ("Occlusion Culling"));
    QWhatsThis::add (m_gl2psOcclusionCulling, tr ("Enable/disable occlusion culling. "
                                                  "All the hidden polygons are removed from the "
						  "output, thus substantially reducing the size "
						  "of the output file."));

    printOptionsLayout->addWidget (m_gl2psOcclusionCulling, 1, 0);
    
    m_gl2psBackGround = new QCheckBox (printOptions, "m_gl2psBackGround");
    m_gl2psBackGround->setText (tr ("Back ground"));
    QWhatsThis::add (m_gl2psBackGround, tr ("Enable/disable back ground printing."));

    printOptionsLayout->addWidget (m_gl2psBackGround, 1, 1);
    
    m_gl2psLandScape = new QCheckBox (printOptions, "m_gl2psLandScape");
    m_gl2psLandScape->setText (tr ("Land scape orientation"));
    QWhatsThis::add (m_gl2psLandScape, tr ("Landscape orientation instead of portrait."));

    printOptionsLayout->addWidget (m_gl2psLandScape, 2, 0);
    
    printOptionsVLayout->addItem (new QSpacerItem ( 0, 0, QSizePolicy::Expanding,
						    QSizePolicy::Expanding));

    connect (m_gl2psSimpleLineOffset, SIGNAL (clicked (void)),
	     this,		      SLOT   (setGL2PSOptions(void)));
    connect (m_gl2psBestRoot,         SIGNAL (clicked (void)),
	     this,		      SLOT   (setGL2PSOptions(void)));
    connect (m_gl2psOcclusionCulling, SIGNAL (clicked (void)),
	     this,		      SLOT   (setGL2PSOptions(void)));
    connect (m_gl2psBackGround,       SIGNAL (clicked (void)),
	     this,		      SLOT   (setGL2PSOptions(void)));
    connect (m_gl2psLandScape,        SIGNAL (clicked (void)),
	     this,		      SLOT   (setGL2PSOptions(void)));
    
    QWhatsThis::add (panel (), "Edit GL2PS printing properties");
    QWhatsThis::add (printOptions, "Edit GL2PS printing properties");
}

void 
IgGL2PSPrintCategory::browserChanged (Ig3DBaseBrowser *source)
{
    if (source)
    {
	IgCommonViewCategory::browserChanged (source);
	if (!updated)
	  updateGL2PSOptions ();
        else
	  setGL2PSOptions ();
    }
}

//////////////////////////////////////////////////////////////////////////
void
IgGL2PSPrintCategory::updateGL2PSOptions (void)
{
    int gl2psOptions = m_source->getGL2PSOptions ();
    m_gl2psBackGround->setChecked (gl2psOptions & GL2PS_DRAW_BACKGROUND);
    m_gl2psSimpleLineOffset->setChecked (gl2psOptions & GL2PS_SIMPLE_LINE_OFFSET);
    m_gl2psBestRoot->setChecked (gl2psOptions & GL2PS_BEST_ROOT);
    m_gl2psOcclusionCulling->setChecked (gl2psOptions & GL2PS_OCCLUSION_CULL);
    m_gl2psLandScape->setChecked (gl2psOptions & GL2PS_LANDSCAPE);
    updated = true;
}

void
IgGL2PSPrintCategory::setGL2PSOptions (void)
{
    int gl2psOptions = 0;
    
    if ( m_gl2psBackGround->isChecked ())
      gl2psOptions |= GL2PS_DRAW_BACKGROUND;
    if ( m_gl2psSimpleLineOffset->isChecked ())
      gl2psOptions |= GL2PS_SIMPLE_LINE_OFFSET;
    if ( m_gl2psBestRoot->isChecked ())
      gl2psOptions |= GL2PS_BEST_ROOT;
    if ( m_gl2psOcclusionCulling->isChecked ())
      gl2psOptions |= GL2PS_OCCLUSION_CULL;
    if ( m_gl2psLandScape->isChecked ())
      gl2psOptions |= GL2PS_LANDSCAPE;
      
    m_source->setGL2PSOptions (gl2psOptions);
}
