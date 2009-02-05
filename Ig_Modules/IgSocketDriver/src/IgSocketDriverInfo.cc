//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSocketDriver/interface/IgSocketDriverInfo.h"
#include "Ig_Modules/IgSocketDriver/interface/IgServerSocket.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"


//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
IG_DEFINE_STATE_ELEMENT (IgSocketDriverInfo, "Socket Driver Info");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSocketDriverInfo::IgSocketDriverInfo (IgState *state)
    :m_state (state)
{
    m_state->put (s_key, this);    
}

IgSocketDriverInfo::~IgSocketDriverInfo (void)
{
    m_state->detach (s_key);    
}

void
IgSocketDriverInfo::setSocket (IgServerSocket *socket)
{
    m_socket = socket;    
}

IgServerSocket *
IgSocketDriverInfo::getSocket (void)
{
    ASSERT (m_socket);
    return m_socket;    
}
