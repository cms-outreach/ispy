#ifndef IG_HELLO_DRIVER_IG_HELLO_DRIVER_H
# define IG_HELLO_DRIVER_IG_HELLO_DRIVER_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgHelloDriver/interface/config.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgDriver.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_HELLO_DRIVER_API IgHelloDriver : public IgDriver
{
public:
    IgHelloDriver (IgState *state);
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

#endif // IG_HELLO_DRIVER_IG_HELLO_DRIVER_H
