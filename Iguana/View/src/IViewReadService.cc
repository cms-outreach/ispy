//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewReadService.h"
#include "Iguana/Framework/interface/IgCollection.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

IG_DEFINE_STATE_ELEMENT (IViewReadService, "Services/iView Framework/Reader");

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IViewReadService::IViewReadService (IgState *state)
    : m_state (state),
      m_storage (0),
      m_esStorage (0),
      m_dispatcher (new IViewEventDispatcher ())
{
    ASSERT (state);
    state->put (s_key, this);

    init ();
}

IViewReadService::~IViewReadService (void)
{
    ASSERT (m_state);
    m_state->detach (s_key);
}

void
IViewReadService::init (void)
{
    m_storage = new IgDataStorage;
    m_esStorage = new IgDataStorage;
}

void
IViewReadService::preEventProcessing (void)
{
    if (m_storage != 0)
	delete m_storage;

    m_storage = new IgDataStorage;
}

void
IViewReadService::preESProcessing (void)
{
    if (m_esStorage != 0)
	delete m_esStorage;

    m_esStorage = new IgDataStorage;
}

void
IViewReadService::postEventProcessing (void)
{
    ASSERT (m_storage);
    delete m_storage;
    m_storage = 0;
}

void
IViewReadService::postESProcessing (void)
{
    ASSERT (m_esStorage);
    delete m_esStorage;
    m_esStorage = 0;
}

IViewReadService::IViewEventDispatcher *
IViewReadService::dispatcher (unsigned int /* eventType */)
{
    return m_dispatcher;
}
