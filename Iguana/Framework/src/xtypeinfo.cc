//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/IgBrowser.h"
#include "Iguana/Framework/interface/IgModel.h"
#include "Iguana/Framework/interface/IgRepresentable.h"
#include "Iguana/Framework/interface/IgRep.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/IgTwig.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

XTYPEINFO_DEF_0 (IgBrowser);
XTYPEINFO_DEF_0 (IgModel);
XTYPEINFO_DEF_0 (IgRepresentable);
XTYPEINFO_DEF_0 (IgRep);

XTYPEINFO_DEF_1 (IgTwig, IgRepresentable, false);
XTYPEINFO_DEF_1 (IgCompoundTwig, IgTwig, false);
XTYPEINFO_DEF_1 (IgSimpleTwig, IgCompoundTwig, false);
