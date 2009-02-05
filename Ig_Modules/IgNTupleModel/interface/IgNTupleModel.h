#ifndef IG_NTUPLE_MODEL_IG_NTUPLE_MODEL_H
# define IG_NTUPLE_MODEL_IG_NTUPLE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgNTupleModel/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
# include <classlib/utils/Callback.h>
# include <list>
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgNTupleModelEvent;
class IgNTupleRep;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_NTUPLE_MODEL_API IgNTupleModel : public IgModel
{
public:
    typedef lat::Callback1<IgNTupleModelEvent> Listener;
    enum EventType { ModelChanged };

    IgNTupleModel (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    void 		add (IgNTupleRep *rep);
    void		remove (IgNTupleRep *rep);
    void		clear (void);
    
    void 		listen (EventType /* event */, 
				const Listener &listener);
    void		unlisten (EventType /* event */, 
				  const Listener &listener);
    /* To be called to notify observers that the model has changed */
    void 		changed (void);
    typedef std::string Binding;
    typedef std::vector<Binding> Bindings;

    const Bindings & 	bindings (void) const;
    void		bindings (const Bindings& binds);    
private:
    typedef std::vector<Listener> Listeners;
    typedef std::list<IgNTupleRep *> Reps;
    
    IgState * 			m_state;    
    Reps			m_reps;
    Listeners			m_listeners;
    Bindings			m_bindings;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_NTUPLE_MODEL_IG_NTUPLE_MODEL_H
