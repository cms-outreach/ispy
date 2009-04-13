#ifndef VIEW_IG_SIMPLE_TEXT_MODEL_H
# define VIEW_IG_SIMPLE_TEXT_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/QtGUI/interface/IgTextModel.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgSimpleTextModel : public IgTextModel
{
public:
    typedef std::vector<Listener> Listeners;

    IgSimpleTextModel (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

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

#endif // VIEW_IG_SIMPLE_TEXT_MODEL_H
