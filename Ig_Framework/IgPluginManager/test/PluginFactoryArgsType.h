#ifndef IG_PLUGIN_MANAGER_TEST_IG_PLUGIN_TYPE_ARGS_H
# define IG_PLUGIN_MANAGER_TEST_IG_PLUGIN_TYPE_ARGS_H

//<<<<<< INCLUDES                                                       >>>>>>
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

#define TEST_TYPE(name,proto)					\
  class name { public: name proto; virtual ~name (void); }

TEST_TYPE(TestPluginType0, (void));
TEST_TYPE(TestPluginType1, (int));
TEST_TYPE(TestPluginType2, (int, int));
TEST_TYPE(TestPluginType3, (int, int, int));
TEST_TYPE(TestPluginType4, (int, int, int, int));

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_PLUGIN_MANAGER_TEST_IG_PLUGIN_TYPE_ARGS_H
