//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyQueuedTwig.h"
#include "Iguana/QtGUI/interface/ISpySceneGraphService.h"
#include "Iguana/QtGUI/interface/debug.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include <Inventor/nodes/SoSeparator.h>
#include <QRegExp>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/// #logstream flag for this package.
lat::logflag LFfwvis = { 0, "fwvis", true, -1 };

class ISpyQueuedTwig::ExactMatch
{
    std::string	m_name;

public:
    ExactMatch (std::string name) : m_name (name) {}

    bool operator() (const std::string twig)
	{ 
	    QRegExp rx (QString (twig.c_str ()), Qt::CaseInsensitive, QRegExp::Wildcard);
	    return rx.exactMatch (QString (m_name.c_str ()));
	}
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


ISpyQueuedTwig::ISpyQueuedTwig (IgState *state, IgTwig *parent,
				const std::string &name /* = "" */)
    : IgSimpleTwig (parent, name, true, false, true),
      ISpyObserver (state),
      m_rep (new SoSeparator),
      m_state (state)
{
    ASSERT (m_state); 
    m_rep->ref ();
    
    ISpySceneGraphService *sceneGraphService = ISpySceneGraphService::get (state);
    ASSERT (sceneGraphService);
    SoSeparator *mainScene = dynamic_cast<SoSeparator *>(sceneGraphService->sceneGraph ());
    ASSERT (mainScene);	
    mainScene->addChild (m_rep);	
}

ISpyQueuedTwig::~ISpyQueuedTwig (void)
{
    ASSERT (m_rep);
    m_rep->unref ();
}

void
ISpyQueuedTwig::clear (void)
{
    ASSERT (m_rep);
    dynamic_cast<SoSeparator *>(m_rep)->removeAllChildren ();
    m_rep->unref ();
    m_rep = new SoSeparator;
    m_rep->ref ();

    ISpySceneGraphService *sceneGraphService = ISpySceneGraphService::get (state ());
    ASSERT (sceneGraphService);
    SoSeparator *mainScene = dynamic_cast<SoSeparator *>(sceneGraphService->sceneGraph ());
    ASSERT (mainScene);	
    mainScene->addChild (m_rep);
}

SoNode*
ISpyQueuedTwig::rep (void)
{
    ASSERT (m_rep);
    return m_rep;
}

void
ISpyQueuedTwig::onNewEvent (ISpyEventMessage& message) 
{
    LOG (0, trace, LFfwvis, "ISpyQueuedTwig[" << name ()
	 << "]::onNewEvent(" << message.message << ")\n");
    clear ();
}

void
ISpyQueuedTwig::onBaseInvalidate (void)
{
    LOG (0, trace, LFfwvis, "ISpyQueuedTwig[" << name ()
         << "]::onBaseInvalidate()\n");
    
    // Lock the main thread while we walk the ancestors of this twig
    // and (possibly) create a burst of requests to update my reps.
    // Note that this does not yet actually update the reps, we only
    // invoke VisQueue::push() to queue calls.  We are executing
    // in the event consumer thread so the main thread is the only one
    // we need to block.  Note that always have to update the object
    // even if this is not visible.  Otherwise the object may get
    // hidden, get a notification (ignored here) and then made visible
    // again -- with the old, wrong contents.
    
    IgRepSet::invalidate (this, SELF_MASK);
}

//////////////////////////////////////////////////////////////////////
void
ISpyQueuedTwig::update (Ig3DRep *)
{ LOG (0, trace, LFfwvis, "ISpyQueuedTwig[" << name ()
       << "]::update(Ig3DRep)\n"); }

void
ISpyQueuedTwig::update (IgLegoRep *)
{ LOG (0, trace, LFfwvis, "ISpyQueuedTwig[" << name ()
       << "]::update(IgLegoRep)\n"); }

void
ISpyQueuedTwig::update (IgTextRep *)
{ LOG (0, trace, LFfwvis, "ISpyQueuedTwig[" << name ()
       << "]::update(IgTextRep)\n"); }

void
ISpyQueuedTwig::update (IgRPhiRep *)
{ LOG (0, trace, LFfwvis, "ISpyQueuedTwig[" << name ()
       << "]::update(IgRPhiRep)\n"); }

void
ISpyQueuedTwig::update (IgRZRep *)
{ LOG (0, trace, LFfwvis, "ISpyQueuedTwig[" << name ()
       << "]::update(IgRZRep)\n"); }

