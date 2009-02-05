//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig2DWindow/interface/Ig2DViewPropertiesCategory.h"
#include "Ig_Modules/Ig2DBrowser/interface/Ig2DBrowser.h"
#include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtCallbacks.h"
#include "Ig_Modules/Ig2DModel/interface/Ig2DModel.h"
#include "Ig_Modules/Ig2DModel/interface/Ig2DRep.h"
#include "Ig_Modules/Ig3DControls/interface/IgQtAxisAlignment.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCentre.h"
#include "Ig_Modules/IgControlCentre/interface/IgStudioCentre.h"
#include <Inventor/nodes/SoTransform.h>

#include <qlayout.h>
#include <qstring.h>
#include <qwhatsthis.h>
#include <classlib/utils/DebugAids.h>


//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
IG_DEFINE_STATE_ELEMENT (Ig2DViewPropertiesCategory, "/Control Centre/Categories/2D View Properties");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig2DViewPropertiesCategory::Ig2DViewPropertiesCategory (IgState *state,
							Ig3DBaseBrowser *source)
    : IgControlCategory (state, "2D View"),
      m_source (source),
      m_root (0),
      m_axisAlignment (0)
{
    int		row   = bodyLayout ()->numRows ();
    ASSERT (row >= 0);
    
    //Add ComboBox to change the type of selected light
    QHBoxLayout *topRow = new QHBoxLayout (topLayout ());
    topRow->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));


    // Alignment widget
    QWidget *area = bodyArea ();    
    QWidget     *vlab = makeIndentedLabel ("Align Camera",
                                            area);
    bodyLayout ()->addWidget (vlab, row, 0);

    m_axisAlignment = new IgQtAxisAlignment (area);

    m_axisAlignment->setMaxAxisAlign (1.);
    m_axisAlignment->setMinAxisAlign (0.);
    bodyLayout ()->addWidget (makeSpacedBox (area, m_axisAlignment->widget ()),
                              row++, 1);

    // Tooltips
    QWhatsThis::add (panel (), "2D-View Properties Category");

    QWhatsThis::add (m_axisAlignment->widget (),
                   "Axis alignment of the 2D view");

    connect (m_axisAlignment,	SIGNAL (dirty ()),
    	     this,		SLOT (setCuttingPlane ()));    
    state->put (s_key, this);
}

Ig2DViewPropertiesCategory::~Ig2DViewPropertiesCategory (void)
{
    state ()->detach (s_key);    
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
Ig2DViewPropertiesCategory::setCuttingPlane (void)
{
    ASSERT (m_root);
    bodyArea ()->setCursor (WaitCursor);
    
    Ig2DModel *model = dynamic_cast <Ig2DModel *> (m_root->model ());
    ASSERT (model);

    SbVec3f axis;
    float angle = M_PI/2;	    

    if (m_axisAlignment->xAlign ())
	axis = SbVec3f (0, 1, 0);
    else if (m_axisAlignment->yAlign ())
	axis = SbVec3f (1, 0, 0);
    else if (m_axisAlignment->zAlign ())
	axis = SbVec3f (0, 0, 1);
	
    model->setCutTransform (axis, angle);
    m_axisAlignment->setClean ();    
    bodyArea ()->setCursor (ArrowCursor);
}


//////////////////////////////////////////////////////////////////////

void
Ig2DViewPropertiesCategory::update (void)
{}

//////////////////////////////////////////////////////////////////////

void
Ig2DViewPropertiesCategory::apply (void)
{}

//////////////////////////////////////////////////////////////////////
void
Ig2DViewPropertiesCategory::registerBrowser (IgState *state, Ig3DBaseBrowser *browser)
{
    IgQtAppContextService *cs = IgQtAppContextService::get (state);
    ASSERT (cs);    
    cs->addOnFocusIn (lat::CreateCallback (this,
					   &Ig2DViewPropertiesCategory::browserChanged,
					   browser));    
    IgControlCategory::registerMe (state);
    browserChanged (browser);
}

void
Ig2DViewPropertiesCategory::browserChanged (Ig3DBaseBrowser *browser)
{
  m_source = browser;
  if(dynamic_cast<Ig2DBrowser *>(m_source))
    m_root = dynamic_cast<Ig2DBrowser *>(m_source)->model ()->attachPoint ();
  else
    m_root = 0;
}
