//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DModel/interface/Ig3DRep.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DModel.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DRep::Ig3DRep (Ig3DModel *model,
		  Ig3DRep *parent,
		  Ig3DRep *prev,
		  SoGroup *node)
    : Ig3DBaseRep (model, parent, prev, node)
{}
