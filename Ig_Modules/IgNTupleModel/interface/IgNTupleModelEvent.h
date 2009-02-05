#ifndef IG_NTUPLE_MODEL_IG_NTUPLE_MODEL_EVENT_H
# define IG_NTUPLE_MODEL_IG_NTUPLE_MODEL_EVENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgNTupleModel/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgNTupleModel;


//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_NTUPLE_MODEL_API IgNTupleModelEvent
{
public:
    IgNTupleModelEvent (IgNTupleModel * model);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    IgNTupleModel	*source (void) const;
private:
    IgNTupleModel	*m_source;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
IgNTupleModelEvent::IgNTupleModelEvent (IgNTupleModel *source)
    : m_source (source)
{}

inline IgNTupleModel *
IgNTupleModelEvent::source (void) const
{ return m_source; }


#endif // IG_NTUPLE_MODEL_IG_NTUPLE_MODEL_EVENT_H
