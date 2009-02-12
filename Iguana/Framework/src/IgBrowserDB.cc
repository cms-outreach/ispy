//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgBrowserDB.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IgBrowserDB IgBrowserDB::s_instance;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgBrowserDB::IgBrowserDB (void)
    : IgPluginFactory<IgBrowser *(IgState *, IgSite *)> ("IGUANA Browser")
{}

IgBrowserDB *
IgBrowserDB::get (void)
{ return &s_instance; }
