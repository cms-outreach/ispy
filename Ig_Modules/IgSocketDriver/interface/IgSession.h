#ifndef IG_SOCKET_DRIVER_IG_SESSION_H
# define IG_SOCKET_DRIVER_IG_SESSION_H

//<<<<<< INCLUDES                                                       >>>>>>
#include "Ig_Modules/IgSocketDriver/interface/config.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_SOCKET_DRIVER_API IgSession : public IgStateElement
{
public:
    IgSession (IgState *state);
    virtual ~IgSession (void);    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    IgState *		state (void);    
private:
    IgState *		m_state;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SOCKET_DRIVER_IG_SESSION_H
