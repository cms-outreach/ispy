#ifndef IGUANA_FRAMEWORK_IG_BROWSER_DB_H
# define IGUANA_FRAMEWORK_IG_BROWSER_DB_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "Iguana/Framework/interface/IgPluginFactory.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgBrowser;
class IgState;
class IgSite;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgBrowserDB
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

#endif // IGUANA_FRAMEWORK_IG_BROWSER_DB_H
