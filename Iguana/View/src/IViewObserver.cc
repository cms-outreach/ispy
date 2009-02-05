//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewObserver.h"
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

IViewObserver::IViewObserver (IgState *state)
    : m_state (state)
{
    ASSERT (m_state);

    IViewReadService* readService = IViewReadService::get (m_state);
    ASSERT (readService);

    readService->dispatcher (IViewReadService::NewEvent)->add (lat::CreateCallback (this, &IViewObserver::onNewEvent));
}

void
IViewObserver::onNewEvent (IViewEventMessage& message) 
{
    std::cout << "Recieved New event" << std::endl;
    std::cout << message.message << std::endl;
}

void
IViewObserver::init (void)
{
    std::cout << "Init" << std::endl;
}
