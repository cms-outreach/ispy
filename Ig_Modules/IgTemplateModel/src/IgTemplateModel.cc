//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTemplateModel/interface/IgTemplateModel.h"
#include "Ig_Modules/IgTemplateModel/interface/IgTemplateRep.h"
#include "Ig_Modules/IgTemplateModel/interface/IgTemplateEngine.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgTemplateModel::IgTemplateModel (IgState *state)
    : m_state (state),
      m_rep (0)
{
}

IgTemplateModel::~IgTemplateModel (void)
{
    delete m_rep;    
}

void
IgTemplateModel::rep (IgTemplateRep *rep)
{
    if (m_rep != rep)
	delete m_rep;    
    m_rep = rep;    
}

void
IgTemplateModel::textTemplate (const std::string &textTemplate)
{
    m_template = textTemplate;    
}

std::string
IgTemplateModel::text (void)
{
    ASSERT (m_rep);
    
    IgRepContext *context = m_rep->context ();
    IgTwig *object = dynamic_cast <IgTwig *> (context->object ());
    
    ASSERT (object);
    
    IgTemplateEngine engine (m_template, m_state, object);
    return engine.text ();    
}
