#ifndef CLASSLIB_HOOK_LIST_H
# define CLASSLIB_HOOK_LIST_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/Hook.h"
# include <list>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class HookList : public std::list< Hook<bool> >
{
public:
    bool	call (void);
};

template <class T1>
class HookList1 : public std::list< Hook1<bool,T1> >
{
public:
    bool	call (const T1 &arg_1);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline bool HookList::call (void)
{
    bool	stopped = false;
    iterator	first;
    for (first = begin (); first != end () && ! stopped; ++first)
	stopped = (*first) ();

    return stopped;
}

/////////////////////////////////////////////////////////////////////////////

template <class T1>
inline bool HookList1<T1>::call (const T1 &arg1)
{
    typedef typename std::list< Hook1<bool,T1> >::iterator iterator;

    bool	stopped = false;
    iterator	first;
    for (first = this->begin (); first != this->end () && ! stopped; ++first)
	stopped = (*first) (arg1);

    return stopped;
}

} // namespace lat
#endif // CLASSLIB_HOOK_LIST_H
