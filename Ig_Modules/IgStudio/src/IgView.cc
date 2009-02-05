//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgView.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgView::IgView (IgPage *page)
    : m_page (page),
      m_title ("")
{
    ASSERT (m_page);
    // FIXME: register into the page    
    // FIXME: control panels, wizards etc.
}

IgView::~IgView (void)
{
    // FIXME: deregister from page
    ASSERT (m_page);
}

IgPage *
IgView::page (void)
{
    return m_page;    
}

IgState *
IgView::state (void)
{
    return m_page->state ();    
}

std::string
IgView::title (void) const
{
    return m_title;    
}

void
IgView::setTitle (const char *title)
{
    ASSERT (title);    
    m_title = title;    
}
