//<<<<<< INCLUDES                                                       >>>>>>

#include "VisExamples/Iggi/src/Iggi.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Studio/interface/IgDocument.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgMDIView.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Studio/interface/IgStudioDriver.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include <qapplication.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (Iggi, "Services/Studio/Session Types/Iggi Demo; iggi");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Iggi::Iggi (IgState *state)
    : m_state (state),
      m_studio (0),
      m_document (0)
{ ASSERT (m_state); m_state->put (s_key, this); }

bool
Iggi::setup (IgStudioDriver *into)
{
    ASSERT (m_state);
    ASSERT (! m_studio);

    ASSERT (into);
    m_studio = into;
    
    // Create document
    ASSERT (! m_document);
    m_document = new IgDocument (m_studio);
    
    // Change window caption.
    m_document->setName ("Iggi Demo");

    // Fill document
    IgPage *page = new IgPage (m_document);
    
    new IgMDIView (page);

    // Load all data proxies
    m_document->addContentProxy ("Iggi/Core");
    m_document->load ("Services/Iggi/GUI/", true);

    IgRepSet::invalidate (m_document->data ()->root (), IgTwig::STRUCTURE_MASK);
    return true;
}
