#ifndef IG_PLUGIN_MANAGER_TEST_IG_PLUGIN_FACTORY_H
# define IG_PLUGIN_MANAGER_TEST_IG_PLUGIN_FACTORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginFactoryType.h"
# include "Ig_Framework/IgPluginManager/interface/IgPluginFactory.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class TestPluginFactory : public IgPluginFactory<TestPluginType *(int, double)>
{
public:
    static TestPluginFactory *get (void);

private:
    TestPluginFactory (void);
    static TestPluginFactory s_instance;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_PLUGIN_MANAGER_TEST_IG_PLUGIN_FACTORY_H
