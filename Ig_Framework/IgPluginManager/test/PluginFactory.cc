//<<<<<< INCLUDES                                                       >>>>>>

#include "PluginFactory.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

TestPluginFactory TestPluginFactory::s_instance;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

TestPluginFactory::TestPluginFactory (void)
    : IgPluginFactory<TestPluginType *(int, double)> ("Test")
{}

TestPluginFactory *
TestPluginFactory::get (void)
{ return &s_instance; }
