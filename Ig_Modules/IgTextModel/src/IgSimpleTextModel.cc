//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTextModel/interface/IgSimpleTextModel.h"
#include "Ig_Modules/IgTextModel/interface/IgTextModelEvent.h"
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

IgSimpleTextModel::IgSimpleTextModel (void)
    : m_current (0)
{}

void
IgSimpleTextModel::listen (EventType /* event */, const Listener &listener)
{ m_listeners.push_back (listener); }

void
IgSimpleTextModel::unlisten (EventType /* event */, const Listener &listener)
{
    Listeners::iterator pos = std::find (m_listeners.begin (),
					 m_listeners.end(),
					 listener);
    ASSERT (pos != m_listeners.end ());
    m_listeners.erase (pos);
}

void
IgSimpleTextModel::changed (void)
{
    IgTextModelEvent event (this);
    for (Listeners::iterator pos = m_listeners.begin ();
	 pos != m_listeners.end (); ++pos)
	(*pos) (event);
}

void
IgSimpleTextModel::set (IgTextRep *rep)
{
    IgTextRep *old = m_current;
    m_current = rep;
    if (old != m_current)
	changed ();
}

IgTextRep *
IgSimpleTextModel::text (void) const
{ return m_current; }
