#ifndef VIEW_IVIEW_QUEUED_TWIG_H
# define VIEW_IVIEW_QUEUED_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/QtGUI/interface/ISpyObserver.h"
# include "Iguana/Framework/interface/IgTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class Ig3DRep;
class IgLegoRep;
class IgTextRep;
class IgRPhiRep;
class IgRZRep;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpyQueuedTwig : public IgSimpleTwig, public ISpyObserver
{
public:
    ISpyQueuedTwig (IgState *state, IgTwig *parent, const std::string &name = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void	onNewEvent (ISpyEventMessage& message);

    virtual void	update (Ig3DRep *rep);
    virtual void	update (IgLegoRep *rep);
    virtual void	update (IgTextRep *rep);
    virtual void        update (IgRPhiRep *rep);
    virtual void        update (IgRZRep *rep);

    IgState *		state (void);

protected:
    void                onBaseInvalidate (void);

private:

    class ExactMatch;
    friend class ExactMatch;

    IgState		       *m_state;		//< State context.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Return the #IgState context in which this object was created.
    This will be the visualisation document context from the main
    framework visualisation application context provided that the twig was
    created through a #ISpyContent.  (FIXME: When we implement the real
    document model, it will be the actual document data structure
    context, i.e. it probably won't be the top level document.)  */
inline IgState *
ISpyQueuedTwig::state (void)
{ return m_state; }

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_QUEUED_TWIG_H
