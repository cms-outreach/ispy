#ifndef IG_SOCKET_DRIVER_IG_WEB_SERVICE_REGISTRY_H
# define IG_SOCKET_DRIVER_IG_WEB_SERVICE_REGISTRY_H

//<<<<<< INCLUDES                                                       >>>>>>
#include "Ig_Modules/IgSocketDriver/interface/config.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
#include <qcstring.h>
#include <string>
#include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;
class IgWebServiceProxy;
class IgWebService;
class QSocket;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_SOCKET_DRIVER_API IgWebServiceRegistry 
    : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgWebServiceRegistry);
public:
    IgWebServiceRegistry (IgState *state);
    ~IgWebServiceRegistry (void);
    
    void 		registerService (const std::string &alias, 
					 IgWebServiceProxy *service);    
    IgWebService *	getService (const char *label);
    
    bool executeURL (QSocket &socket, 
		     const std::string &url, 
		     const std::string &idCookie,
		     QByteArray bodyData);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    IgState *					m_state;    
    typedef std::map <std::string, IgWebServiceProxy *> WebServiceMap;
    WebServiceMap 				m_serviceMap;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SOCKET_DRIVER_IG_WEB_SERVICE_REGISTRY_H
