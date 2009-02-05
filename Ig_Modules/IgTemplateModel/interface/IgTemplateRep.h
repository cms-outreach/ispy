#ifndef IG_TEMPLATE_MODEL_IG_TEMPLATE_REP_H
# define IG_TEMPLATE_MODEL_IG_TEMPLATE_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTemplateModel/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgRep.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgRepContext;
class IgModel;
class IgTemplateModel;


//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TEMPLATE_MODEL_API IgTemplateRep : public IgRep
{
public:
    IgTemplateRep (IgTemplateModel *model);
    ~IgTemplateRep (void);
    IgModel * 	model (void) const;
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    IgRepContext *context (void) const;
protected:
    void 	context (IgRepContext *context);
private:
    IgModel *		m_model;
    IgRepContext *	m_context;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TEMPLATE_MODEL_IG_TEMPLATE_REP_H
