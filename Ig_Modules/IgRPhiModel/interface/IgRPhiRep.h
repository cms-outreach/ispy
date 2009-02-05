#ifndef IG_RPHI_MODEL_IG_RPHI_REP_H
# define IG_RPHI_MODEL_IG_RPHI_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgRPhiModel/interface/config.h"
# include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRPhiModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// Never ever use the fact that I can be cast silently to the
// SoSeparator base class!  Always use the methods here.

class IG_RPHI_MODEL_API IgRPhiRep : public Ig3DBaseRep
{
public:
    IgRPhiRep (IgRPhiModel *model, IgRPhiRep *parent, IgRPhiRep *prev, SoGroup *node);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_RPHI_MODEL_IG_RPHI_REP_H
