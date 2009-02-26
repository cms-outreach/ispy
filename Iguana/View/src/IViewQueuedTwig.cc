//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewQueuedTwig.h"
#include "Iguana/View/interface/debug.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include <QRegExp>
#include <QSettings>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

class IViewQueuedTwig::ExactMatch
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


IViewQueuedTwig::IViewQueuedTwig (IgState *state, IgTwig *parent,
				  const std::string &name /* = "" */)
    : IgSimpleTwig (parent, name, true, false, true),
      IViewObserver (state),
      m_state (state)
{
    ASSERT (m_state); 

    QString str ("twigs/visibility/");
    str.append (fullName ().c_str ());
    
    QSettings settings;
    if (settings.contains (str))
    {
	bool flag = settings.value (str).value<bool> ();
	selfVisible (flag);	
    }
}

void
IViewQueuedTwig::onNewEvent (IViewEventMessage& message) 
{ LOG (0, trace, LFfwvis, "IViewQueuedTwig[" << name ()
       << "]::onNewEvent(" << message.message << ")\n");}

void
IViewQueuedTwig::onBaseInvalidate (void)
{
    LOG (0, trace, LFfwvis, "IViewQueuedTwig[" << name ()
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
IViewQueuedTwig::update (Ig3DRep *)
{ LOG (0, trace, LFfwvis, "IViewQueuedTwig[" << name ()
       << "]::update(Ig3DRep)\n"); }

void
IViewQueuedTwig::update (IgLegoRep *)
{ LOG (0, trace, LFfwvis, "IViewQueuedTwig[" << name ()
       << "]::update(IgLegoRep)\n"); }

void
IViewQueuedTwig::update (IgTextRep *)
{ LOG (0, trace, LFfwvis, "IViewQueuedTwig[" << name ()
       << "]::update(IgTextRep)\n"); }

void
IViewQueuedTwig::update (IgRPhiRep *)
{ LOG (0, trace, LFfwvis, "IViewQueuedTwig[" << name ()
       << "]::update(IgRPhiRep)\n"); }

void
IViewQueuedTwig::update (IgRZRep *)
{ LOG (0, trace, LFfwvis, "IViewQueuedTwig[" << name ()
       << "]::update(IgRZRep)\n"); }

