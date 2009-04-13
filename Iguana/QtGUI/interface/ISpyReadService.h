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

struct ISpyEventMessage
{
    std::string message;
};

class IGUANA_FRAMEWORK_API ISpyReadService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (ISpyReadService);
public:
    typedef IgDispatcher<ISpyEventMessage&> ISpyEventDispatcher;
    enum {NewEvent};

    ISpyReadService (IgState *state);
    ~ISpyReadService (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    IgDataStorage*	dataStorage (void)
	{ return m_storage; }
    IgDataStorage*	esStorage (void)
	{ return m_esStorage; }

    void 		preEventProcessing (void);
    void 		preESProcessing (void);
    void		postEventProcessing (void);
    void		postESProcessing (void);
    ISpyEventDispatcher *dispatcher (unsigned int eventType);
    
private:
    IgState	      	*m_state;
    IgDataStorage     	*m_storage;
    IgDataStorage     	*m_esStorage;

    ISpyEventDispatcher *m_dispatcher;

    void 	      	init (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_READ_SERVICE_H
