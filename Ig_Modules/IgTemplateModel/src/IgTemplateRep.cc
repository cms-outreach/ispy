//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTemplateModel/interface/IgTemplateRep.h"
#include "Ig_Modules/IgTemplateModel/interface/IgTemplateModel.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgTemplateRep::IgTemplateRep (IgTemplateModel *model)
    : m_model (model)
{
    model->rep (this);    
}

IgTemplateRep::~IgTemplateRep (void)
{
    ASSERT (m_context);
    if (m_context->rep ())
    {	
        ASSERT (m_context->rep () == this);
	m_context->erase (false);
	delete m_context;
    }
}

void
IgTemplateRep::context (IgRepContext *context)
{
    m_context = context;    
}

IgRepContext *
IgTemplateRep::context (void) const
{
    return m_context;    
}

IgModel *
IgTemplateRep::model (void) const
{
    return m_model;    
}

