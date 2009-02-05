//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTextModel/interface/IgTextRep.h"
#include "Ig_Modules/IgTextModel/interface/IgSimpleTextRep.h"
#include "Ig_Modules/IgTextModel/interface/IgTextModel.h"
#include "Ig_Modules/IgTextModel/interface/IgSimpleTextModel.h"
#include "Ig_Modules/IgTextModel/interface/xtypeinfo.h"
#include "Ig_Framework/IgObjectBrowser/interface/xtypeinfo.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

XTYPEINFO_DEF_1 (IgTextRep, IgRep, false);
XTYPEINFO_DEF_1 (IgSimpleTextRep, IgTextRep, false);
XTYPEINFO_DEF_1 (IgTextModel, IgModel, false);
XTYPEINFO_DEF_1 (IgSimpleTextModel, IgTextModel, false);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>
