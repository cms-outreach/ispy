#ifndef IG_SOCKET_DRIVER_IG_SERVER_POOL_H
# define IG_SOCKET_DRIVER_IG_SERVER_POOL_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgSocketDriver/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <classlib/utils/Argz.h>
# include <set>
# include <map>
# include <string>
# include <sys/types.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class lat::Argz;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_SOCKET_DRIVER_API IgServerPool : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgServerPool);
    
public:
    static IgServerPool* getInstance (IgState *state);
    
    std::string 	redirect (const std::string& url,
    				  std::string& cookie);
    void 		addServer (void);    
    int 		availableServers (void)const;
    int   		processPoolSize (void)const ;
    void		childTerminated(int pid);

private:
    IgServerPool (IgState *state);
    ~IgServerPool (void);
    
    std::string		createProcess (void);

    typedef std::set <std::string> 		ServerSet;    
    typedef std::map <std::string, std::string> CookieToServerMap;
    typedef std::map <pid_t, std::string> 	PIDToServerMap;
    
    static IgServerPool*m_instance;
    IgState*		m_state;
    CookieToServerMap 	m_cookieServerMap;
    PIDToServerMap 	m_pidServerMap;
    lat::Argz 	       *m_argz;    
    ServerSet 		m_availableServers;
    int			m_maxProcesses;    
    int			m_processPoolSize;
    std::string		m_hostname;
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SOCKET_DRIVER_IG_SERVER_POOL_H
