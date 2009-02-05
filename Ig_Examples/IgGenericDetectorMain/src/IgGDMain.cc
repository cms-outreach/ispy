//<<<<<< INCLUDES                                                       >>>>>>
#include "Ig_Examples/IgGenericDetectorMain/interface/IgGDMain.h"
#include "Ig_Examples/IgGenericDetectorSetup/interface/IgGDSetup.h"
#include "Ig_Modules/IgStudio/interface/IgStudioDriver.h"
#include "Ig_Modules/IgStudio/interface/IgDocument.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Modules/IgStudio/interface/IgMDIView.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgDocumentDataRoot.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#undef emit // zap qt macro
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgGDMain, "Services/Generic/Detector");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgGDMain::IgGDMain (IgState *state)
    : m_state (state),
      m_studio (0),
      m_document (0)
{
    ASSERT (m_state);
    m_state->put (s_key, this);
}

// delete m_twigs;

bool
IgGDMain::setup (IgGDSetup *from)
{
    ASSERT (m_state);
    ASSERT (! m_studio);

    ASSERT (from);
    ASSERT (from->state () == m_state);
    ASSERT (from->studio ());

    m_studio = from->studio ();
    
    // Create document
    ASSERT (! m_document);
    m_document = new IgDocument (m_studio);
    
    // Change window caption.
    const char* mName = "Generic/Detector";
    IgDocumentDataRoot::currentRoot (mName);
    
    new IgMDIView (new IgPage (m_document));
    
    m_document->addContentProxy (mName);

    IgRepSet::invalidate (m_document->data ()->root (mName),
		          IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
    return true;
}
