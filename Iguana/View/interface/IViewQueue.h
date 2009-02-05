#ifndef VIEW_IVIEW_QUEUE_H
# define VIEW_IVIEW_QUEUE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgStateElement.h"
# include "Iguana/Framework/interface/config.h"
# include <QQueue>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

namespace lat { class Callback; }

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IViewQueue: public IgStateElement, public QQueue<lat::Callback>
{
    IG_DECLARE_STATE_ELEMENT (IViewQueue);
public:
    IViewQueue (IgState *state);
    ~IViewQueue (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

private:
    IgState	*m_state;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_QUEUE_H
