//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgLegoModel/interface/IgLegoRep.h"
#include "Ig_Modules/IgLegoModel/interface/IgLegoModel.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgLegoRep::IgLegoRep (IgLegoModel *model,
		      IgLegoRep *parent,
		      IgLegoRep *prev,
		      SoGroup *node)
    : Ig3DBaseRep (model, parent, prev, node)
{}
