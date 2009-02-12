//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgSiteDB.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IgSiteDB IgSiteDB::s_instance;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSiteDB::IgSiteDB (void)
    : IgPluginFactory<IgSite *(IgState *, IgSite *)> ("IGUANA Site")
{}

IgSiteDB *
IgSiteDB::get (void)
{ return &s_instance; }
