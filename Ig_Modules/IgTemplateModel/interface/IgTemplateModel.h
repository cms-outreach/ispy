#ifndef IG_TEMPLATE_MODEL_IG_TEMPLATE_MODEL_H
# define IG_TEMPLATE_MODEL_IG_TEMPLATE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTemplateModel/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgTemplateRep;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TEMPLATE_MODEL_API IgTemplateModel : public IgModel
{
public:
    IgTemplateModel (IgState *state);
    ~IgTemplateModel (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    void rep (IgTemplateRep *rep);
    void textTemplate (const std::string &textTemplate);
    std::string text (void);    
private:
    IgState *		m_state;
    std::string 	m_template;
    IgTemplateRep * 	m_rep;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TEMPLATE_MODEL_IG_TEMPLATE_MODEL_H
