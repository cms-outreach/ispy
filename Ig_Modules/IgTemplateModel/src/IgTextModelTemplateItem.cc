//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTemplateModel/interface/IgTextModelTemplateItem.h"
#include "Ig_Modules/IgTemplateModel/interface/IgTemplateEngine.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Modules/IgTextModel/interface/IgSimpleTextModel.h"
#include "Ig_Modules/IgTextModel/interface/IgTextRep.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgTextModelTemplateItem::IgTextModelTemplateItem (IgTemplateEngine *t)
    : IgTemplateItem (t)
{
}

std::string
IgTextModelTemplateItem::represent (IgTwig *twig)
{
    IgSimpleTextModel model;
    IgRepSet::lookup (twig, &model, true);
    return model.text ()->text ();    
}

const char *
IgTextModelTemplateItem::catalogLabel (void)
{
    return "textModel";    
}
