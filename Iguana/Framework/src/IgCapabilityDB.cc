//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgCapabilityDB.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IgCapabilityDB IgCapabilityDB::s_instance;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgCapabilityDB::IgCapabilityDB (void)
    : IgPluginFactory<void> ("IGUANA Capability")
{}

IgCapabilityDB *
IgCapabilityDB::get (void)
{ return &s_instance; }
