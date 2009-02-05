#ifndef IG_SOCKET_DRIVER_IG_SESSION_MANAGER_H
# define IG_SOCKET_DRIVER_IG_SESSION_MANAGER_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgSocketDriver/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <string>
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_SOCKET_DRIVER_API IgSessionManager : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgSessionManager);    
public:
    IgSessionManager (IgState *state);
    ~IgSessionManager (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    void setSession (const std::string &idCookie, IgState *state);    
    IgState *getSession (const std::string &idCookie);
    IgState *createSession (const std::string &idCookie, IgState *parent);    
private:
    typedef std::map <std::string, IgState *> StateMap;    
    typedef std::pair <std::string, IgState *> StateMapElement;
    
    IgState *		m_state;
    StateMap 		m_stateMap;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SOCKET_DRIVER_IG_SESSION_MANAGER_H
