//<<<<<< INCLUDES                                                       >>>>>>

#include "PluginFactoryArgs.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

#define DEF_TEST_FACTORY(fname,proto,name)			\
  fname fname::s_instance;					\
  fname::fname (void) : IgPluginFactory<proto> (name) {}	\
  fname *fname::get (void) { return &s_instance; }

DEF_TEST_FACTORY (TestPluginFactoryArgsV, void, "TestV")
DEF_TEST_FACTORY (TestPluginFactoryArgs0, TestPluginType0 *(void), "Test0")
DEF_TEST_FACTORY (TestPluginFactoryArgs1, TestPluginType1 *(int), "Test1")
DEF_TEST_FACTORY (TestPluginFactoryArgs2, TestPluginType2 *(int, int), "Test2")
DEF_TEST_FACTORY (TestPluginFactoryArgs3, TestPluginType3 *(int, int, int), "Test3")
DEF_TEST_FACTORY (TestPluginFactoryArgs4, TestPluginType4 *(int, int, int, int), "Test4")
