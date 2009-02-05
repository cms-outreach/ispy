#ifndef IG_NTUPLE_MODEL_IG_COMMON_NTUPLE_REPS_H
# define IG_NTUPLE_MODEL_IG_COMMON_NTUPLE_REPS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgNTupleModel/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTwig;
class IgRepContext;
class IgNTupleModel;
class IgNTupleRep;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
IG_NTUPLE_MODEL_API MMM_DECLARE(IgRepContext *, doRepresent,
				(IgTwig *twig, IgNTupleModel *model));
IG_NTUPLE_MODEL_API MMM_DECLARE(void,		  doUpdate,
				(IgTwig *twig, IgNTupleRep *rep, unsigned mask));
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_NTUPLE_MODEL_IG_COMMON_NTUPLE_REPS_H
