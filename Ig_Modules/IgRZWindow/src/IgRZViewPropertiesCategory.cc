//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgRZWindow/interface/IgRZViewPropertiesCategory.h"
#include "Ig_Modules/IgRZBrowser/interface/IgRZBrowser.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include "Ig_Modules/Ig3DControls/interface/IgQtAxisAlignment.h"
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

IG_DEFINE_STATE_ELEMENT (IgRZViewPropertiesCategory, "/Control Centre/Categories/RZ View Properties");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgRZViewPropertiesCategory::IgRZViewPropertiesCategory (IgState *state,
							IgRZBrowser *source)
    : IgControlCategory (state, "RZ View"),
      m_source (source),
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
    QWhatsThis::add (panel (), "RZ-View Properties Category");

    QWhatsThis::add (m_axisAlignment->widget (),
                   "Axis alignment of the lego view");

    connect (m_axisAlignment,	SIGNAL (dirty ()),
             this,		SLOT (alignAxis ()));    
    state->put (s_key, this);
}

IgRZViewPropertiesCategory::~IgRZViewPropertiesCategory (void)
{
    state ()->detach (s_key);    
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
IgRZViewPropertiesCategory::alignAxis (void)
{
    ASSERT (m_source);
    if (m_axisAlignment->xAlign ())
      m_source->viewPlaneX ();
    else if (m_axisAlignment->yAlign ())
      m_source->viewPlaneY ();
    else if (m_axisAlignment->zAlign ())
      m_source->viewPlaneZ ();
    m_axisAlignment->setClean ();    
}
//////////////////////////////////////////////////////////////////////
void
IgRZViewPropertiesCategory::registerBrowser (IgState *state, IgRZBrowser *source)
{
    IgQtAppContextService *cs = IgQtAppContextService::get (state);
    ASSERT (cs);
    cs->addOnFocusIn (lat::CreateCallback (this,
					   &IgRZViewPropertiesCategory::browserChanged,
					   source));
    IgControlCategory::registerMe (state);
    m_source = source;
}

void 
IgRZViewPropertiesCategory::browserChanged (IgRZBrowser *source)
{
    if (source)
      m_source = source;	
}
