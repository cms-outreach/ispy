#ifndef IG_LEGO_MODEL_IG_LEGO_REP_H
# define IG_LEGO_MODEL_IG_LEGO_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgLegoModel/interface/config.h"
# include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgLegoModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// Never ever use the fact that I can be cast silently to the
// SoSeparator base class!  Always use the methods here.

class IG_LEGO_MODEL_API IgLegoRep : public Ig3DBaseRep
{
public:
    IgLegoRep (IgLegoModel *model, IgLegoRep *parent, IgLegoRep *prev, SoGroup *node);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_LEGO_MODEL_IG_LEGO_REP_H
