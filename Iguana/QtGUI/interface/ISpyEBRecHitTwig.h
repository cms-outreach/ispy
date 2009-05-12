#ifndef VIEW_IVIEW_EB_REC_HIT_TWIG_H
# define VIEW_IVIEW_EB_REC_HIT_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/QtGUI/interface/ISpyQueuedTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpyEBRecHitTwig : public ISpyQueuedTwig
{
public:
    ISpyEBRecHitTwig (IgState *state, IgTwig *parent, 
		      const std::string &name = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void  onNewEvent (ISpyEventMessage& message);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_EB_REC_HIT_TWIG_H
