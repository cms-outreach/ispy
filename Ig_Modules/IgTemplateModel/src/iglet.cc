//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgCommonData/interface/xtypeinfo.h"
#include "Ig_Modules/IgTemplateModel/interface/IgTemplateModel.h"
#include "Ig_Modules/IgTemplateModel/interface/IgTemplateRep.h"
#include "Ig_Modules/IgTemplateModel/interface/xtypeinfo.h"

#include "Ig_Framework/IgObjectBrowser/interface/IgCapabilityDB.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowserMethods.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"

#include "Ig_Modules/IgTemplateModel/interface/IgTemplateItemDB.h"
#include "Ig_Modules/IgTemplateModel/interface/IgTwigFullnameTemplateItem.h"
#include "Ig_Framework/IgPluginManager/interface/IgModuleDef.h"
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgTemplateItemDB, IgTwigFullnameTemplateItem, IgTwigFullnameTemplateItem::catalogLabel ());

XTYPEINFO_DEF_1 (IgTemplateRep, IgRep, false);
XTYPEINFO_DEF_1 (IgTemplateModel, IgModel, false);

lat::logflag LFcommonTemplate = { 0, "commonTemplate", true, -1 };

DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, IgBrowserMethods::key
		       (typeid (IgTemplateModel).name (),
			typeid (IgTwig).name ()));



//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (IgTwig *twig, IgTemplateRep */*rep*/, unsigned mask))
{
    LOG (0, trace, LFcommonTemplate, "doUpdate(IgTwig[" << twig->name ()
	 << "],IgTemplateRep," << mask << ")\n" << lat::indent);

    LOG (0, trace, LFcommonTemplate, lat::undent);
}

MMM_DEFUN_FUNC (IgRepContext *, IgBrowserMethods::, doRepresent,
		(IgTwig *twig, IgTemplateModel *model))	
{
    LOG (0, trace, LFcommonTemplate, "doRepresent(IgTwig[" << twig->name ()
	 << "],IgTemplateModel)\n" << lat::indent);

    IgRep *rep = new IgTemplateRep (model);
    IgRepContext *context = new IgRepContext (twig, rep);
    
    LOG (0, trace, LFcommonTemplate, lat::undent);
    return context;    
}
