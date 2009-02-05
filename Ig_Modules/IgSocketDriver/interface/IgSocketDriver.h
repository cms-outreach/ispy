#ifndef IG_SOCKET_DRIVER_IG_SOCKET_DRIVER_H
# define IG_SOCKET_DRIVER_IG_SOCKET_DRIVER_H

//<<<<<< INCLUDES                                                       >>>>>>
#include "Ig_Modules/IgSocketDriver/interface/config.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgDriver.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;


//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IgSocketDriver : public IgDriver
{
public:
    IgSocketDriver (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual int		run (void);
    static const char *	catalogLabel (void);
private:
    IgState		*m_state;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SOCKET_DRIVER_IG_SOCKET_DRIVER_H
