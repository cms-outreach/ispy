#ifndef IG_SOCKET_DRIVER_IG_SOCKET_DRIVER_INFO_H
# define IG_SOCKET_DRIVER_IG_SOCKET_DRIVER_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgSocketDriver/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgServerSocket;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgSocketDriverInfo : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgSocketDriverInfo);    
public:
    IgSocketDriverInfo (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual ~IgSocketDriverInfo (void);
    IgServerSocket *getSocket (void);
    void setSocket (IgServerSocket *sock);
    
private:
    IgState * 		m_state;
    IgServerSocket *	m_socket;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SOCKET_DRIVER_IG_SOCKET_DRIVER_INFO_H
