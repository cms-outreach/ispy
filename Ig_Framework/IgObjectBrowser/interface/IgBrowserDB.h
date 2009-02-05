#ifndef IG_OBJECT_BROWSER_IG_BROWSER_DB_H
# define IG_OBJECT_BROWSER_IG_BROWSER_DB_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Framework/IgObjectBrowser/interface/config.h"
# include "Ig_Framework/IgPluginManager/interface/IgPluginFactory.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgBrowser;
class IgState;
class IgSite;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OBJECT_BROWSER_API IgBrowserDB
    : public IgPluginFactory<IgBrowser *(IgState *state, IgSite *host)>
{
public:
    static IgBrowserDB *get (void);

private:
    IgBrowserDB (void);
    static IgBrowserDB s_instance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OBJECT_BROWSER_IG_BROWSER_DB_H
