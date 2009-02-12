//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgDriverDB.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IgDriverDB IgDriverDB::s_instance;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgDriverDB::IgDriverDB (void)
    : IgPluginFactory<IgDriver *(IgState *)> ("IGUANA Driver")
{}

IgDriverDB *
IgDriverDB::get (void)
{ return &s_instance; }
