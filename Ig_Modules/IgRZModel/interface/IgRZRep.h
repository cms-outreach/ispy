#ifndef IG_RZ_MODEL_IG_RZ_REP_H
# define IG_RZ_MODEL_IG_RZ_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgRZModel/interface/config.h"
# include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRZModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// Never ever use the fact that I can be cast silently to the
// SoSeparator base class!  Always use the methods here.

class IG_RZ_MODEL_API IgRZRep : public Ig3DBaseRep
{
public:
    IgRZRep (IgRZModel *model, IgRZRep *parent, IgRZRep *prev, SoGroup *node);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_RZ_MODEL_IG_RZ_REP_H
