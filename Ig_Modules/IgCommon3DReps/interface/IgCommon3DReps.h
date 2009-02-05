#ifndef IG_COMMON_3D_REPS_IG_COMMON_3D_REPS_H
# define IG_COMMON_3D_REPS_IG_COMMON_3D_REPS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgCommon3DReps/interface/config.h"
# include <classlib/utils/MultiMethod.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRepContext;
class IgTwig;
class IgTextModel;
class IgTextRep;
class Ig3DModel;
class Ig2DModel;
class IgLegoModel;
class IgRZModel;
class IgRPhiModel;
class Ig3DBaseRep;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

IG_COMMON_3D_REPS_API MMM_DECLARE(IgRepContext *, doRepresent,
				  (IgTwig *twig, IgTextModel *model));
IG_COMMON_3D_REPS_API MMM_DECLARE(void,		  doUpdate,
				  (IgTwig *twig, IgTextRep *rep, unsigned mask));

IG_COMMON_3D_REPS_API MMM_DECLARE(IgRepContext *, doRepresent,
				  (IgTwig *twig, Ig3DModel *model));
IG_COMMON_3D_REPS_API MMM_DECLARE(void,		  doUpdate,
				  (IgTwig *twig, Ig3DBaseRep *rep, unsigned mask));

IG_COMMON_3D_REPS_API MMM_DECLARE(IgRepContext *, doRepresent,
				  (IgTwig *twig, Ig2DModel *model));
IG_COMMON_3D_REPS_API MMM_DECLARE(IgRepContext *, doRepresent,
				  (IgTwig *twig, IgLegoModel *model));
IG_COMMON_3D_REPS_API MMM_DECLARE(IgRepContext *, doRepresent,
				  (IgTwig *twig, IgRZModel *model));
IG_COMMON_3D_REPS_API MMM_DECLARE(IgRepContext *, doRepresent,
				  (IgTwig *twig, IgRPhiModel *model));

//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_COMMON_3D_REPS_IG_COMMON_3D_REPS_H
