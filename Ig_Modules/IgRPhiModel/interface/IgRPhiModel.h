#ifndef IG_RPHI_MODEL_IG_RPHI_MODEL_H
# define IG_RPHI_MODEL_IG_RPHI_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgRPhiModel/interface/config.h"
# include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_RPHI_MODEL_API IgRPhiModel : public Ig3DBaseModel
{
public:
    IgRPhiModel (IgState *state);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_RPHI_MODEL_IG_RPHI_MODEL_H
