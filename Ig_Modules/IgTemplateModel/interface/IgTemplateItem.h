#ifndef IG_TEMPLATE_MODEL_IG_TEMPLATE_ITEM_H
# define IG_TEMPLATE_MODEL_IG_TEMPLATE_ITEM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTemplateModel/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgTemplateEngine;
class IgTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TEMPLATE_MODEL_API IgTemplateItem
{
public:
    IgTemplateItem (IgTemplateEngine *);
    virtual ~IgTemplateItem (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    IgTemplateEngine *engine (void);
    virtual std::string represent (IgTwig *) = 0;    
public:
    IgTemplateEngine *m_engine;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TEMPLATE_MODEL_IG_TEMPLATE_ITEM_H
