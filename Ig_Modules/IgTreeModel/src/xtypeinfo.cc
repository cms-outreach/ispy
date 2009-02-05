//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTreeModel/interface/IgTreeNode.h"
#include "Ig_Modules/IgTreeModel/interface/IgTreeRep.h"
#include "Ig_Modules/IgTreeModel/interface/IgSimpleTreeRep.h"

#include "Ig_Modules/IgTreeModel/interface/IgTreeModel.h"
#include "Ig_Modules/IgTreeModel/interface/IgSimpleTreeModel.h"

#include "Ig_Modules/IgTreeModel/interface/xtypeinfo.h"
#include "Ig_Framework/IgObjectBrowser/interface/xtypeinfo.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

XTYPEINFO_DEF_0 (IgTreeNode);
XTYPEINFO_DEF_2 (IgTreeRep, IgRep, false,  IgTreeNode, false);
XTYPEINFO_DEF_1 (IgSimpleTreeRep, IgTreeRep, false);
XTYPEINFO_DEF_1 (IgTreeModel, IgModel, false);
XTYPEINFO_DEF_1 (IgSimpleTreeModel, IgTreeModel, false);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>
