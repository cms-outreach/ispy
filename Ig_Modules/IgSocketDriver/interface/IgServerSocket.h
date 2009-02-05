#ifndef IG_SOCKET_DRIVER_IG_SOCKET_SERVER_H
# define IG_SOCKET_DRIVER_IG_SOCKET_SERVER_H

//<<<<<< INCLUDES                                                       >>>>>>
#include "Ig_Modules/IgSocketDriver/interface/config.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
#include <qserversocket.h>
#include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;
class QTimer;
class IgFileManager;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_SOCKET_DRIVER_API IgServerSocket : public QServerSocket,
					    public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgServerSocket);
    Q_OBJECT
public:
    IgServerSocket (IgState *state, bool mainPort = true);
    virtual void newConnection (int port);
    
    void		setClientCookieID (const std::string& cookieID);
    const std::string&	getClientCookieID () const;
    
    void		ping (QIODevice *socket);
    void		clientInitialization (QIODevice *socket);
    void		startTimer ();
    IgFileManager*	getFileManager () const;
    
    static int			getFreePort ();
    static const std::string& 	generateCookie(std::string& cookie);
    
    static const std::string&  	cmdLineProxy (IgState* state);
    static const std::string&  	cmdLineHostname (IgState* state);
    static int  		cmdLinePort (IgState* state, bool mainPort = true);
    static const std::string&  	cmdLineHostname ();
    static const std::string&  	cmdLineProxy ();
    static const std::string&  	cmdLineMainHostUrl ();
    static int			cmdLinePort ();
    static std::string  	hostnameUrl ();
    
    static std::string  redirect (const std::string& url,
    				  const std::string& redir,
				  const std::string& cookie);
    
private slots:
    void		timedOut ();
    void		socketClosed ();
    
private:
    void		initialize (IgState *state);
    
private:
    static std::string	m_hostname;
    static std::string  m_mainHostUrl;
    static std::string  m_proxy;
    static int		m_port;
    
    IgState *		m_state;
    int 		m_timeout;
    QTimer*		m_timer;
    std::string		m_clientCookieID;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SOCKET_DRIVER_IG_SOCKET_SERVER_H
