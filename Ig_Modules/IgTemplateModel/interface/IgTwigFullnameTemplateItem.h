#ifndef IG_TEMPLATE_MODEL_IG_TWIG_FULLNAME_TEMPLATE_ITEM_H
# define IG_TEMPLATE_MODEL_IG_TWIG_FULLNAME_TEMPLATE_ITEM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTemplateModel/interface/config.h"
# include "Ig_Modules/IgTemplateModel/interface/IgTemplateItem.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgTwig;
class IgTemplateEngine;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TEMPLATE_MODEL_API IgTwigFullnameTemplateItem : public IgTemplateItem
{
public:
    IgTwigFullnameTemplateItem (IgTemplateEngine *t);    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    std::string represent (IgTwig *twig);
    static const char *catalogLabel (void);    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TEMPLATE_MODEL_IG_TWIG_FULLNAME_TEMPLATE_ITEM_H
