//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgIVView/interface/IgIVView.h"
#include "Ig_Modules/IgStudio/interface/IgViewDB.h"
#include "Ig_Modules/IgIVView/interface/IgSoTwig.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserMethods.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DRep.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DModel.h"
#include "Ig_Modules/IgCommon3DReps/interface/IgCommon3DReps.h"
#include "Ig_Framework/IgObjectBrowser/interface/xtypeinfo.h"
#include "Ig_Modules/IgCommonData/interface/xtypeinfo.h"
#include "Ig_Modules/Ig3DModel/interface/xtypeinfo.h"
#include "Ig_Modules/IgIVView/interface/xtypeinfo.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

XTYPEINFO_DEF_1 (IgSoTwig, IgSimpleTwig, false);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (IgSoTwig *twig, Ig3DRep *rep, unsigned mask))
{
    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    if (! twig->visible ())
        return;

    int max = twig->node ()->getNumChildren ();
    for (int i = 0; i < max;  i++)
        rep->node ()->addChild (twig->node ()->getChild(i));
    rep->magic (twig->magic ());
}
