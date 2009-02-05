//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgLegoWindow/interface/IgLegoViewPropertiesCategory.h"
#include "Ig_Modules/IgLegoBrowser/interface/IgLegoBrowser.h"
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

IG_DEFINE_STATE_ELEMENT (IgLegoViewPropertiesCategory, "/Control Centre/Categories/Lego View Properties");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgLegoViewPropertiesCategory::IgLegoViewPropertiesCategory (IgState *state,
							    IgLegoBrowser *source)
    : IgControlCategory (state, "Lego View"),
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
    QWhatsThis::add (panel (), "Lego-View Properties Category");

    QWhatsThis::add (m_axisAlignment->widget (),
                   "Axis alignment of the lego view");

    connect (m_axisAlignment,	SIGNAL (dirty ()),
             this,		SLOT (alignAxis ()));    
    state->put (s_key, this);
}

IgLegoViewPropertiesCategory::~IgLegoViewPropertiesCategory (void)
{
    state ()->detach (s_key);    
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
IgLegoViewPropertiesCategory::alignAxis (void)
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
IgLegoViewPropertiesCategory::registerBrowser (IgState *state, IgLegoBrowser *source)
{
    IgQtAppContextService *cs = IgQtAppContextService::get (state);
    ASSERT (cs);
    cs->addOnFocusIn (lat::CreateCallback (this,
					   &IgLegoViewPropertiesCategory::browserChanged,
					   source));
    IgControlCategory::registerMe (state);
    m_source = source;
}

void 
IgLegoViewPropertiesCategory::browserChanged (IgLegoBrowser *source)
{
    if (source)
      m_source = source;	
}
