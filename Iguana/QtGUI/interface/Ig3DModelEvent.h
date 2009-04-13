#ifndef VIEW_IG3DMODEL_EVENT_H
# define VIEW_IG3DMODEL_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API Ig3DModelEvent
{
public:
    Ig3DModelEvent (Ig3DModel *source);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    Ig3DModel *	source (void) const;

private:
    Ig3DModel *	m_source;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
Ig3DModelEvent::Ig3DModelEvent (Ig3DModel *source)
    : m_source (source)
{}

inline Ig3DModel *
Ig3DModelEvent::source (void) const
{ return m_source; }

#endif // VIEW_IG3DMODEL_EVENT_H
