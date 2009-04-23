#ifndef VIEW_IG_TEXT_MODEL_EVENT_H
# define VIEW_IG_TEXT_MODEL_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgTextModelEvent
{
public:
    IgTextModelEvent (IgTextModel *source);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    IgTextModel *		source (void) const;

private:
    IgTextModel *		m_source;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
IgTextModelEvent::IgTextModelEvent (IgTextModel *source)
    : m_source (source)
{}

inline IgTextModel *
IgTextModelEvent::source (void) const
{
    return m_source;
}

#endif // VIEW_IG_TEXT_MODEL_EVENT_H
