//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgPluginCapabilities.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IgPluginCapabilities IgPluginCapabilities::s_instance;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgPluginCapabilities::IgPluginCapabilities (void)
    : IgPluginFactory<void> ("Capability")
{}

IgPluginCapabilities *
IgPluginCapabilities::get (void)
{ return &s_instance; }
