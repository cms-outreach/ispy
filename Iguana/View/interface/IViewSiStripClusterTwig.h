#ifndef VIEW_IVIEW_SI_STRIP_CLUSTER_TWIG_H
# define VIEW_IVIEW_SI_STRIP_CLUSTER_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/View/interface/IViewQueuedTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IViewSiStripClusterTwig : public IViewQueuedTwig
{
public:
    IViewSiStripClusterTwig (IgState *state, IgTwig *parent, 
			     const std::string &name = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void  onNewEvent (IViewEventMessage& message);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_SI_STRIP_CLUSTER_TWIG_H
