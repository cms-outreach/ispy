#ifndef IG_PLUGIN_MANAGER_TEST_IG_PLUGIN_FACTORY_ARGS_H
# define IG_PLUGIN_MANAGER_TEST_IG_PLUGIN_FACTORY_ARGS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginFactoryArgsType.h"
# include "Ig_Framework/IgPluginManager/interface/IgPluginFactory.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

#define TEST_FACTORY(fname,proto)			\
  class fname : public IgPluginFactory<proto> {		\
  public: static fname *get (void);			\
  private: fname (void); static fname s_instance; }

TEST_FACTORY (TestPluginFactoryArgsV, void);
TEST_FACTORY (TestPluginFactoryArgs0, TestPluginType0 *(void));
TEST_FACTORY (TestPluginFactoryArgs1, TestPluginType1 *(int));
TEST_FACTORY (TestPluginFactoryArgs2, TestPluginType2 *(int, int));
TEST_FACTORY (TestPluginFactoryArgs3, TestPluginType3 *(int, int, int));
TEST_FACTORY (TestPluginFactoryArgs4, TestPluginType4 *(int, int, int, int));

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_PLUGIN_MANAGER_TEST_IG_PLUGIN_FACTORY_ARGS_H
