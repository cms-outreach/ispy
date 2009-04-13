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

struct ISpyEventMessage
{
    std::string message;
};

class IGUANA_FRAMEWORK_API ISpyQueueProcessor : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (ISpyQueueProcessor);
public:
    typedef IgDispatcher<ISpyEventMessage&> ISpyEventDispatcher;

    ISpyQueueProcessor (IgState *state);
    ~ISpyQueueProcessor (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ISpyEventDispatcher eventDispatcher (void);

    virtual void	process (void);

    virtual void	scheduleAbort (void);
    virtual void	scheduleNextEvent (void);

private:
    void		doAbort (void);
    void		doNextEvent (void);

    IgState		*m_state;
    bool		m_done;
    ISpyEventDispatcher *m_dispatcher;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_QUEUE_PROCESSOR_H
