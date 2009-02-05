#ifndef IG_OBJECT_BROWSER_IG_CAPABILITY_DB_H
# define IG_OBJECT_BROWSER_IG_CAPABILITY_DB_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Framework/IgObjectBrowser/interface/config.h"
# include "Ig_Framework/IgPluginManager/interface/IgPluginFactory.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OBJECT_BROWSER_API IgCapabilityDB
    : public IgPluginFactory<void>
{
public:
    static IgCapabilityDB *get (void);

private:
    IgCapabilityDB (void);
    static IgCapabilityDB s_instance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OBJECT_BROWSER_IG_CAPABILITY_DB_H
