#ifndef IG_LEGO_MODEL_IG_LEGO_MODEL_H
# define IG_LEGO_MODEL_IG_LEGO_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgLegoModel/interface/config.h"
# include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_LEGO_MODEL_API IgLegoModel : public Ig3DBaseModel
{
public:
    IgLegoModel (IgState *state);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_LEGO_MODEL_IG_LEGO_MODEL_H
