#ifndef IG_TEXT_MODEL_IG_SIMPLE_TEXT_MODEL_H
# define IG_TEXT_MODEL_IG_SIMPLE_TEXT_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTextModel/interface/IgTextModel.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TEXT_MODEL_API IgSimpleTextModel : public IgTextModel
{
public:
    typedef std::vector<Listener> Listeners;

    IgSimpleTextModel (void);
    // default copy constructor
    // default destructor
    // default assignment operator

    virtual void	listen (EventType event, const Listener &listener);
    virtual void	unlisten (EventType event, const Listener &listener);
    virtual void	changed (void);

    virtual IgTextRep *	text (void) const;
    virtual void	set (IgTextRep *rep);

private:
    IgTextRep		*m_current;
    Listeners		m_listeners;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TEXT_MODEL_IG_SIMPLE_TEXT_MODEL_H
