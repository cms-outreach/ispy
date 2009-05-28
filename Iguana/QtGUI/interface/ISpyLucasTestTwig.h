#ifndef I_SPY_LUCAS_TEST_TWIG_H
# define I_SPY_LUCAS_TEST_TWIG_H


//  Playground class for Lucas to develop new drawing functionality
//  without breaking what exists. The idea is that the new drawn items
//  can be compared simulataneously in the viewer with the old ones.
//  If all is OK then code from here is migrated into the main iSpy code.




//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/QtGUI/interface/ISpyQueuedTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpyLucasTestTwig : public ISpyQueuedTwig
{
public:
    ISpyLucasTestTwig (IgState *state, IgTwig *parent, 
			const std::string &name = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void  onNewEvent (ISpyEventMessage& message);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // I_SPY_LUCAS_TEST_TWIG_H
