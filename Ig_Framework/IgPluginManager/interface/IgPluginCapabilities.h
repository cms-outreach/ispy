#ifndef IG_PLUGIN_MANAGER_IG_PLUGIN_CAPABILITIES_H
# define IG_PLUGIN_MANAGER_IG_PLUGIN_CAPABILITIES_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Framework/IgPluginManager/interface/IgPluginFactory.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgPluginCapabilities : public IgPluginFactory<void>
{
public:
    static IgPluginCapabilities *get (void);

private:
    IgPluginCapabilities (void);
    static IgPluginCapabilities s_instance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_PLUGIN_MANAGER_IG_PLUGIN_CAPABILITIES_H
