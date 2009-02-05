#ifndef VIEW_IVIEW_OBSERVER_H
# define VIEW_IVIEW_OBSERVER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgDispatcher.h"
# include "Iguana/Framework/interface/config.h"
# include "Iguana/View/interface/IViewReadService.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IViewObserver
{
public:
    IViewObserver (IgState *state);
    virtual ~IViewObserver (void) {}
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void	onNewEvent (IViewEventMessage& message);

    void 	       	init (void);

private:
    IgState	       	*m_state;		//< State context.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_OBSERVER_H
