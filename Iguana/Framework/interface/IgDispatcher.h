#ifndef IGUANA_FRAMEWORK_IG_DISPATCHER_H
# define IGUANA_FRAMEWORK_IG_DISPATCHER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "classlib/utils/Callback.h"
# include "classlib/utils/DebugAids.h"
# include <vector>
# include <algorithm>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

template <class Message>
class IgDispatcher
{
public:
    typedef lat::Callback1<Message> Observer;

    IgDispatcher (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    void		add (const Observer &listener);
    void		remove (const Observer &listener);
    void		broadcast (Message event) const;

private:
    typedef std::vector<Observer>		List;
    typedef typename List::iterator		iterator;
    typedef typename List::const_iterator	const_iterator;

    List		m_list;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

template <class Message>
inline
IgDispatcher<Message>::IgDispatcher (void)
{}

template <class Message>
inline void
IgDispatcher<Message>::add (const Observer &listener)
{ m_list.push_back (listener); }

template <class Message>
inline void
IgDispatcher<Message>::remove (const Observer &listener)
{
    iterator pos = std::find (m_list.begin (), m_list.end(), listener);
    ASSERT (pos != m_list.end ());
    m_list.erase (pos);
}

template <class Message>
inline void
IgDispatcher<Message>::broadcast (Message event) const
{
    for (const_iterator pos = m_list.begin (); pos != m_list.end (); ++pos)
	(*pos) (event);
}

#endif // IGUANA_FRAMEWORK_IG_DISPATCHER_H
