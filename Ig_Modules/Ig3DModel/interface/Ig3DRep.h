#ifndef IG_3D_MODEL_IG_3D_REP_H
# define IG_3D_MODEL_IG_3D_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DModel/interface/config.h"
# include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// Never ever use the fact that I can be cast silently to the
// SoSeparator base class!  Always use the methods here.

class IG_3D_MODEL_API Ig3DRep : public Ig3DBaseRep
{
public:
    Ig3DRep (Ig3DModel *model, Ig3DRep *parent, Ig3DRep *prev, SoGroup *node);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_MODEL_IG_3D_REP_H
