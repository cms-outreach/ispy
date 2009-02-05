#ifndef IG_2D_MODEL_IG_2D_REP_H
# define IG_2D_MODEL_IG_2D_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig2DModel/interface/config.h"
# include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig2DModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// Never ever use the fact that I can be cast silently to the
// SoSeparator base class!  Always use the methods here.

class IG_2D_MODEL_API Ig2DRep : public Ig3DBaseRep
{
public:
    Ig2DRep (Ig2DModel *model, Ig2DRep *parent, Ig2DRep *prev, SoGroup *node);
    virtual void repositionChildren (void);    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_2D_MODEL_IG_2D_REP_H
