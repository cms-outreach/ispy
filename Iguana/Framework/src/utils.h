#ifndef IGUANA_FRAMEWORK_UTILS_H
# define IGUANA_FRAMEWORK_UTILS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Framework/IgObjectBrowser/interface/config.h"
# include <SealBase/DebugAids.h>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

template <class T>
class InfoByType
{
    std::string	m_type;
public:
    InfoByType (const std::string &type) : m_type (type) {}
    bool operator() (const T *info) {
	ASSERT (info);
	return info->type () == m_type;
    }
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

template <class Iter>
inline void
destruct (Iter first, Iter last)
{
    for ( ; first != last; ++first)
	delete *first;
}

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_UTILS_H
