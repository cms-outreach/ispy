//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSocketDriver/interface/IgSession.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSession::IgSession (IgState *state)
    : m_state (state)
{
}

IgSession::~IgSession (void)
{
}

IgState *
IgSession::state (void)
{
    return m_state;    
}
