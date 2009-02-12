//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgExtensionDB.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IgExtensionDB IgExtensionDB::s_instance;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgExtensionDB::IgExtensionDB (void)
    : IgPluginFactory<IgExtension *(IgState *)> ("IGUANA Extension")
{}

IgExtensionDB *
IgExtensionDB::get (void)
{ return &s_instance; }
