#ifndef IG_FILE_PROXY_IG_FILE_PROXY_DB_H
# define IG_FILE_PROXY_IG_FILE_PROXY_DB_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgFileProxy/interface/IgFileProxy.h"
# include "Ig_Modules/IgFileProxy/interface/config.h"
# include "Ig_Framework/IgPluginManager/interface/IgPluginFactory.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_FILE_PROXY_API IgFileProxyDB : public IgPluginFactory<IgFileProxy *(void)>
{
public:
    static IgFileProxyDB *get (void);

private:
    IgFileProxyDB (void);
    static IgFileProxyDB s_instance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_FILE_PROXY_IG_FILE_PROXY_DB_H
