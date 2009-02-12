#ifndef IGUANA_FRAMEWORK_IG_NAME_LIST_H
# define IGUANA_FRAMEWORK_IG_NAME_LIST_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include <string>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A list of names.  */
typedef std::list<std::string>		IgNameList;

/** An iterator over #IgNameList.  */
typedef IgNameList::const_iterator	IgNameIterator;

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

/** Check if the string is a name list: that it has at least two words
    separated by spaces.  */
inline bool
isNameList (const std::string &list)
{ return list.find_first_not_of (' ') != std::string::npos; }

/** Split names in name list @a from to the list @a to.  */
inline void
splitNameList (IgNameList &to, const std::string &from)
{
    std::string::size_type start = 0;
    std::string::size_type end;
    do {
	end = from.find_first_of (' ', start);
	to.push_back (from.substr (start, end));
	start = (end == std::string::npos ? end
		 : from.find_first_not_of (' ', end));
    } while (start != std::string::npos);
}

/** Make a name list string from the list @a from.  */
inline std::string
makeNameList (const IgNameList &from)
{
    IgNameIterator	name = from.begin ();
    std::string		result;

    while (true)
    {
	result.append (*name);
	if (++name == from.end ())
	    break;
	result.append (1, ' ');
    }
    return result;
}

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_NAME_LIST_H
