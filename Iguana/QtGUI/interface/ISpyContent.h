#ifndef VIEW_IVIEW_CONTENT_H
# define VIEW_IVIEW_CONTENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgExtension.h"
# include "Iguana/Framework/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

namespace lat { class Callback; }

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API ISpyContent : public IgExtension
{
public:
    enum Type { MAIN_THREAD, EVENT_THREAD };
    ISpyContent (IgState *state, IgStateKey key, Type which, lat::Callback action);
    ISpyContent (IgState *state, IgStateKey key,
		  lat::Callback mainAction, lat::Callback eventAction);
    ~ISpyContent (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected:
    IgState *           state (void) const;

private:
    IgState             *m_state;
    IgStateKey          m_key;
};

/** Return the state context of this data proxy.  It is safe to call
    this method in the derived class' callback action.  */
inline IgState *
ISpyContent::state (void) const
{ return m_state; }

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_CONTENT_H
