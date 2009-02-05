//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgNTupleModel/interface/IgNTupleModel.h"
#include "Ig_Modules/IgNTupleModel/interface/IgNTupleModelEvent.h"
#include "Ig_Modules/IgNTupleModel/interface/IgNTupleRep.h"
#include <classlib/utils/DebugAids.h>
#include <algorithm>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgNTupleModel::IgNTupleModel (IgState *state)
    :IgModel (),
     m_state (state)
{    
}

void 		
IgNTupleModel::add (IgNTupleRep *rep)
{
    m_reps.push_back (rep);    
}

void		
IgNTupleModel::remove (IgNTupleRep *rep)
{
    m_reps.remove (rep);    
}

void		
IgNTupleModel::clear (void)
{
    m_reps.clear ();    
}

void
IgNTupleModel::changed (void)
{
    IgNTupleModelEvent event (this);
    for (Listeners::iterator pos = m_listeners.begin ();
	 pos != m_listeners.end (); ++pos)
	(*pos) (event);
}

void
IgNTupleModel::listen (EventType /* event */, const Listener &listener)
{ m_listeners.push_back (listener); }

void
IgNTupleModel::unlisten (EventType /* event */, const Listener &listener)
{
    Listeners::iterator pos = std::find (m_listeners.begin (),
					 m_listeners.end(),
					 listener);
    ASSERT (pos != m_listeners.end ());
    m_listeners.erase (pos);
}

void
IgNTupleModel::bindings (const IgNTupleModel::Bindings &binds)
{
    m_bindings.clear ();
    m_bindings.resize (binds.size ());
    
    for (unsigned int i = 0;
	 i != binds.size ();
	 i++)
    {
	m_bindings[i] = binds[i];	
    }
    
}

const IgNTupleModel::Bindings &
IgNTupleModel::bindings (void) const
{
    return m_bindings;    
}
