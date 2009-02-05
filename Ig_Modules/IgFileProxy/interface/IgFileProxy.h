#ifndef IG_FILE_PROXY_IG_FILE_PROXY_H
# define IG_FILE_PROXY_IG_FILE_PROXY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgFileProxy/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_FILE_PROXY_API IgFileProxy
{
public:
    virtual ~IgFileProxy ();
    // implicit copy constructor
    // implicit assignment operator

    virtual std::string fetch (const std::string &file) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_FILE_PROXY_IG_FILE_PROXY_H
