//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Framework/interface/IgCollection.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

IG_DEFINE_STATE_ELEMENT (ISpyReadService, "Services/iView Framework/Reader");

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpyReadService::ISpyReadService (IgState *state)
    : m_state (state),
      m_storage (0),
      m_esStorage (0),
      m_dispatcher (new ISpyEventDispatcher ())
{
    ASSERT (state);
    state->put (s_key, this);

    init ();
}

ISpyReadService::~ISpyReadService (void)
{
    ASSERT (m_state);
    m_state->detach (s_key);
}

void
ISpyReadService::init (void)
{
    m_storage = new IgDataStorage;
    m_esStorage = new IgDataStorage;
}

void
ISpyReadService::preEventProcessing (void)
{
    if (m_storage != 0)
	delete m_storage;

    m_storage = new IgDataStorage;
}

void
ISpyReadService::preESProcessing (void)
{
    if (m_esStorage != 0)
	delete m_esStorage;

    m_esStorage = new IgDataStorage;
}

void
ISpyReadService::postEventProcessing (void)
{
    ASSERT (m_storage);
    delete m_storage;
    m_storage = 0;
}

void
ISpyReadService::postESProcessing (void)
{
    ASSERT (m_esStorage);
    delete m_esStorage;
    m_esStorage = 0;
}

ISpyReadService::ISpyEventDispatcher *
ISpyReadService::dispatcher (unsigned int /* eventType */)
{
    return m_dispatcher;
}
