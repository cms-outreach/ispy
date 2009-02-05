//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgFileProxy/interface/IgFileLocalProxy.h"
#include "Ig_Modules/IgFileProxy/interface/IgHttpFileProxy.h"
#include "Ig_Modules/IgFileProxy/interface/IgFileProxyDB.h"
#include "Ig_Framework/IgPluginManager/interface/IgModuleDef.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgFileProxyDB, IgFileLocalProxy, IgFileLocalProxy::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgFileProxyDB, IgHttpFileProxy,  IgHttpFileProxy::catalogLabel ());
