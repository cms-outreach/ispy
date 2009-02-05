//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgFileProxy/interface/IgFileProxyDB.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IgFileProxyDB IgFileProxyDB::s_instance;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgFileProxyDB::IgFileProxyDB (void)
    : IgPluginFactory<IgFileProxy *(void)> ("IGUANA File Proxy")
{}

IgFileProxyDB *
IgFileProxyDB::get (void)
{ return &s_instance; }
