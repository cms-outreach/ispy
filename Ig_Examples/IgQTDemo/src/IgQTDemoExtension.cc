//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgQTDemo/interface/IgQTDemoExtension.h"
#include "Ig_Modules/IgStudio/interface/IgStudioDriver.h"
#include "Ig_Examples/IgQTDemo/interface/IgQTDemo.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQTDemoExtension, "Services/Studio/Session Types/Vis Example--Qt Demo");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQTDemoExtension::IgQTDemoExtension (IgState *state)
    : m_state (state)
{ ASSERT (m_state); }

IgQTDemoExtension::~IgQTDemoExtension (void)
{ 
    ASSERT (m_state);
    delete m_qtDemo;
}

bool
IgQTDemoExtension::setup (IgStudioDriver *into)
{
    ASSERT (into);
    
    m_qtDemo = new IgQTDemo (m_state, "");
   
    return true;
}
