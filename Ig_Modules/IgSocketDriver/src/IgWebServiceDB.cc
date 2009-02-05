//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSocketDriver/interface/IgWebServiceDB.h"
#include "Ig_Modules/IgSocketDriver/interface/IgWebService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgWebServiceDB IgWebServiceDB::s_instance;


IgWebServiceDB::IgWebServiceDB (void)
    : IgPluginFactory <IgWebService *(IgState *)> ("IGUANA Web Services")
{
}

IgWebServiceDB *
IgWebServiceDB::get (void)
{
    return &s_instance;    
}
