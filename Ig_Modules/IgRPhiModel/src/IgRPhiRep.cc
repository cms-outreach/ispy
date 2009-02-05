//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgRPhiModel/interface/IgRPhiRep.h"
#include "Ig_Modules/IgRPhiModel/interface/IgRPhiModel.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgRPhiRep::IgRPhiRep (IgRPhiModel *model,
		      IgRPhiRep *parent,
		      IgRPhiRep *prev,
		      SoGroup *node)
    : Ig3DBaseRep (model, parent, prev, node)
{}
