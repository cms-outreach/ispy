#ifndef IG_PLUGIN_MANAGER_TEST_SAMPLE_IG_PLUGIN_H
# define IG_PLUGIN_MANAGER_TEST_SAMPLE_IG_PLUGIN_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginFactoryType.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class TestSamplePlugin : public TestPluginType
{
public:
    TestSamplePlugin (int arg, double stuff);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_PLUGIN_MANAGER_TEST_SAMPLE_IG_PLUGIN_H
