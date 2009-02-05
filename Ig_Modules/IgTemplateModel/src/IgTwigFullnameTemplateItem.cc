//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTemplateModel/interface/IgTwigFullnameTemplateItem.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgTwigFullnameTemplateItem::IgTwigFullnameTemplateItem (IgTemplateEngine *t)
    : IgTemplateItem (t)
{    
}

std::string
IgTwigFullnameTemplateItem::represent (IgTwig *twig)
{
    return twig->fullName ();
}

const char *
IgTwigFullnameTemplateItem::catalogLabel (void)
{
    return "twigFullName";
}
