#ifndef IG_TEXT_MODEL_IG_TEXT_MODEL_EVENT_H
# define IG_TEXT_MODEL_IG_TEXT_MODEL_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTextModel/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TEXT_MODEL_API IgTextModelEvent
{
public:
    IgTextModelEvent (IgTextModel *source);
    // default copy constructor
    // default assignment operator
    // default destructor

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

#endif // IG_TEXT_MODEL_IG_TEXT_MODEL_EVENT_H
