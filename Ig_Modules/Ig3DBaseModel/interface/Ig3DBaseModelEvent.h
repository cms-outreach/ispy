#ifndef IG_3D_BASE_MODEL_IG_3D_BASE_MODEL_EVENT_H
# define IG_3D_BASE_MODEL_IG_3D_BASE_MODEL_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DBaseModel/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DBaseModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_BASE_MODEL_API Ig3DBaseModelEvent
{
public:
    Ig3DBaseModelEvent (Ig3DBaseModel *source);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    Ig3DBaseModel *	source (void) const;

private:
    Ig3DBaseModel *	m_source;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
Ig3DBaseModelEvent::Ig3DBaseModelEvent (Ig3DBaseModel *source)
    : m_source (source)
{}

inline Ig3DBaseModel *
Ig3DBaseModelEvent::source (void) const
{ return m_source; }

#endif // IG_3D_BASE_MODEL_IG_3D_BASE_MODEL_EVENT_H
