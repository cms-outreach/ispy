//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgViewDB.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IgViewDB IgViewDB::s_instance;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgViewDB::IgViewDB (void)
    : IgPluginFactory<IgView *(IgPage *)> ("IGUANA View")
{}

IgViewDB *
IgViewDB::get (void)
{ return &s_instance; }
