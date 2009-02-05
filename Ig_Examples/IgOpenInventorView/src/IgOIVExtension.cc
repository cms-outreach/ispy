//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgOpenInventorView/interface/IgOIVExtension.h"
#include "Ig_Modules/IgStudio/interface/IgStudioDriver.h"
#include "Ig_Modules/IgStudio/interface/IgDocument.h"
#include "Ig_Modules/IgStudio/interface/IgPage.h"
#include "Ig_Modules/IgStudio/interface/IgViewDB.h"
#include "Ig_Modules/Ig3DSystem/interface/Ig3DSystem.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#undef emit // zap qt macro
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgOIVExtension, "Services/Studio/Session Types/Vis Example--Open Inventor File Reader; IvReader");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOIVExtension::IgOIVExtension (IgState *state)
    : m_state (state),
      m_document (0)
{
    ASSERT (m_state);
    m_state->put (s_key, this);
}

bool
IgOIVExtension::setup (IgStudioDriver *into)
{
    ASSERT (m_state);
    ASSERT (into);

    // Create document
    ASSERT (! m_document);
    m_document = new IgDocument (into);
    
    // Fill document
    IgPage *page = new IgPage (m_document);
    
    // Initialise SoQt if it hasn't been done yet.
    Ig3DSystem::init (m_state, IgQtSite::selfFrom (into->mainSite ()));
    
    QString name ("Open Inventor View");
    IgViewDB::get ()->create (name.latin1 (), page);

    return true;
}
