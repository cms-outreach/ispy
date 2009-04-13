//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/IgDrawFactoryService.h"
#include <stdexcept>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgDrawFactoryService, "Services/Framework/Draw Factory");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgDrawFactoryService::IgDrawFactoryService (IgState *state)
{
    ASSERT (state);
    state->put (s_key, this);
}

IgDrawFactoryService::~IgDrawFactoryService (void) 
{    
    ASSERT (m_state);
    m_state->detach (s_key);
}

bool
IgDrawFactoryService::registerDrawFunction (const std::string &name, CreateDrawCallback createFn) 
{
    return m_callback.insert (CallbackMap::value_type (name, createFn)).second;
}

bool
IgDrawFactoryService::unregisterDrawFunction (const std::string &name) 
{
    return m_callback.erase (name) == 1;
}

SoNode *
IgDrawFactoryService::draw (const std::string &name, 
			    IgState *state, const std::string &collection,
			    const std::string &rep)
{
    CallbackMap::const_iterator i = m_callback.find (name);
    
    if (i == m_callback.end ())
    {
        throw std::runtime_error ("Unknown Draw Function name.");
    }

    return (i->second)(state, collection, name, rep);
}
