#ifndef IG_OBJECT_BROWSER_IG_SITE_DB_H
# define IG_OBJECT_BROWSER_IG_SITE_DB_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Framework/IgObjectBrowser/interface/config.h"
# include "Ig_Framework/IgPluginManager/interface/IgPluginFactory.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgSite;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OBJECT_BROWSER_API IgSiteDB
    : public IgPluginFactory<IgSite *(IgState *state, IgSite *parent)>
{
public:
    static IgSiteDB *get (void);

private:
    IgSiteDB (void);
    static IgSiteDB s_instance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OBJECT_BROWSER_IG_SITE_DB_H
