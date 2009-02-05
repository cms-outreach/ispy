#ifndef IG_3D_MODEL_IG_3D_MODEL_H
# define IG_3D_MODEL_IG_3D_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DModel/interface/config.h"
# include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_MODEL_API Ig3DModel : public Ig3DBaseModel
{
public:
    Ig3DModel (IgState *state);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_MODEL_IG_3D_MODEL_H
