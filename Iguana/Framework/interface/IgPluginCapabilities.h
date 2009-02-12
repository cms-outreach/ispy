#ifndef IGUANA_FRAMEWORK_IG_PLUGIN_CAPABILITIES_H
# define IGUANA_FRAMEWORK_IG_PLUGIN_CAPABILITIES_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgPluginFactory.h"

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

#endif // IGUANA_FRAMEWORK_IG_PLUGIN_CAPABILITIES_H
