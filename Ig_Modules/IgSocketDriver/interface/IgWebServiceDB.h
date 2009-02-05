#ifndef IG_SOCKET_DRIVER_IG_WEB_SERVICE_DB_H
# define IG_SOCKET_DRIVER_IG_WEB_SERVICE_DB_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSocketDriver/interface/config.h"
#include "Ig_Framework/IgPluginManager/interface/IgPluginFactory.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgWebService;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_SOCKET_DRIVER_API IgWebServiceDB 
    : public IgPluginFactory <IgWebService *(IgState *)>
{
public:
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    static IgWebServiceDB *get ();
private:
    IgWebServiceDB (void);
    static IgWebServiceDB s_instance;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SOCKET_DRIVER_IG_WEB_SERVICE_DB_H
