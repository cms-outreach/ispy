#ifndef VIEW_IVIEW_QUEUE_PROCESSOR_H
# define VIEW_IVIEW_QUEUE_PROCESSOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgStateElement.h"
# include "Iguana/Framework/interface/IgDispatcher.h"
# include "Iguana/Framework/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

struct IViewEventMessage
{
    std::string message;
};

class IGUANA_FRAMEWORK_API IViewQueueProcessor : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IViewQueueProcessor);
public:
    typedef IgDispatcher<IViewEventMessage&> IViewEventDispatcher;

    IViewQueueProcessor (IgState *state);
    ~IViewQueueProcessor (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    IViewEventDispatcher eventDispatcher (void);

    virtual void	process (void);

    virtual void	scheduleAbort (void);
    virtual void	scheduleNextEvent (void);

private:
    void		doAbort (void);
    void		doNextEvent (void);

    IgState		*m_state;
    bool		m_done;
    IViewEventDispatcher *m_dispatcher;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_QUEUE_PROCESSOR_H
