#ifndef IG_RZ_MODEL_IG_RZ_MODEL_H
# define IG_RZ_MODEL_IG_RZ_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgRZModel/interface/config.h"
# include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_RZ_MODEL_API IgRZModel : public Ig3DBaseModel
{
public:
    IgRZModel (IgState *state);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_RZ_MODEL_IG_RZ_MODEL_H
