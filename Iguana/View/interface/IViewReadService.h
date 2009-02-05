#ifndef VIEW_IVIEW_READ_SERVICE_H
# define VIEW_IVIEW_READ_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgStateElement.h"
# include "Iguana/Framework/interface/IgDispatcher.h"
# include "Iguana/Framework/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgDataStorage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

struct IViewEventMessage
{
    std::string message;
};

class IGUANA_FRAMEWORK_API IViewReadService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IViewReadService);
public:
    typedef IgDispatcher<IViewEventMessage&> IViewEventDispatcher;
    enum {NewEvent};

    IViewReadService (IgState *state);
    ~IViewReadService (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    IgDataStorage*	dataStorage (void)
	{ return m_storage; }

    void 		preEventProcessing (void);
    void		postEventProcessing (void);
    IViewEventDispatcher *dispatcher (unsigned int eventType);
    
private:
    IgState	      	*m_state;
    IgDataStorage     	*m_storage;

    IViewEventDispatcher *m_dispatcher;

    void 	      	init (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_READ_SERVICE_H
