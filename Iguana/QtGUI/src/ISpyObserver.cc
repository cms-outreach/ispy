//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyObserver.h"
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpyObserver::ISpyObserver (IgState *state)
    : m_state (state)
{
    ASSERT (m_state);

    ISpyReadService* readService = ISpyReadService::get (m_state);
    ASSERT (readService);

    readService->dispatcher (ISpyReadService::NewEvent)->add (lat::CreateCallback (this, &ISpyObserver::onNewEvent));
}

void
ISpyObserver::onNewEvent (ISpyEventMessage& message) 
{
    std::cout << "Recieved New event" << std::endl;
    std::cout << message.message << std::endl;
}

void
ISpyObserver::init (void)
{
    std::cout << "Init" << std::endl;
}
