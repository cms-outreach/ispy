#ifndef IG_TEMPLATE_MODEL_IG_TEMPLATE_ITEM_DB_H
# define IG_TEMPLATE_MODEL_IG_TEMPLATE_ITEM_DB_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTemplateModel/interface/config.h"
# include "Ig_Framework/IgPluginManager/interface/IgPluginFactory.h"
# include "Ig_Framework/IgPluginManager/interface/IgPluginInfo.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTemplateEngine;
class IgTemplateItem;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TEMPLATE_MODEL_API IgTemplateItemDB 
    : public IgPluginFactory <IgTemplateItem * (IgTemplateEngine *)>
{
public:
    IgTemplateItemDB (void);
    
    static IgTemplateItemDB * get (void);    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    static IgTemplateItemDB s_instance;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TEMPLATE_MODEL_IG_TEMPLATE_ITEM_DB_H
