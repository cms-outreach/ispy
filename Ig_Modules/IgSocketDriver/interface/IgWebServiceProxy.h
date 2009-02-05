#ifndef IG_SOCKET_DRIVER_IG_WEB_SERVICE_PROXY_H
# define IG_SOCKET_DRIVER_IG_WEB_SERVICE_PROXY_H

//<<<<<< INCLUDES                                                       >>>>>>
# include <Ig_Modules/IgSocketDriver/interface/config.h>
# include <Ig_Modules/IgSocketDriver/interface/IgWebServiceDB.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgWebService;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_SOCKET_DRIVER_API IgWebServiceProxy 
{
public:
    typedef IgWebServiceDB::Map::mapped_type PluginInfo;    
    IgWebServiceProxy (IgState *state, PluginInfo pluginInfo);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    IgWebService *webService (void);    
private:
    IgState *		m_state;
    PluginInfo 		m_pluginInfo;    
    IgWebService *	m_webService;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SOCKET_DRIVER_IG_WEB_SERVICE_PROXY_H
