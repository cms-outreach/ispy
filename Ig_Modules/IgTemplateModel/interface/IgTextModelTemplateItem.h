#ifndef IG_TEMPLATE_MODEL_IG_TEXT_MODEL_TEMPLATE_ITEM_H
# define IG_TEMPLATE_MODEL_IG_TEXT_MODEL_TEMPLATE_ITEM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTemplateModel/interface/config.h"
# include "Ig_Modules/IgTemplateModel/interface/IgTemplateItem.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgTemplateEngine;
class IgTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TEMPLATE_MODEL_API IgTextModelTemplateItem : public IgTemplateItem
{
public:
    IgTextModelTemplateItem (IgTemplateEngine *t);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    std::string represent (IgTwig *twig);
    static const char *catalogLabel (void);    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TEMPLATE_MODEL_IG_TEXT_MODEL_TEMPLATE_ITEM_H
