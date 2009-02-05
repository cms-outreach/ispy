#ifndef IG_FILE_PROXY_IG_FILE_LOCAL_PROXY_H
# define IG_FILE_PROXY_IG_FILE_LOCAL_PROXY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgFileProxy/interface/IgFileProxy.h"
# include "Ig_Modules/IgFileProxy/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_FILE_PROXY_API IgFileLocalProxy : public IgFileProxy
{
public:
    virtual std::string fetch (const std::string &file);
    static const char *	catalogLabel (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_FILE_PROXY_IG_FILE_LOCAL_PROXY_H
