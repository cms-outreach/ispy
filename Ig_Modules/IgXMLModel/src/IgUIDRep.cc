//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgXMLModel/interface/IgUIDRep.h"
#include "Ig_Modules/IgXMLModel/interface/IgUIDModel.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
#include <stdlib.h>
#include <qstring.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgUIDRep::IgUIDRep (IgUIDModel *model)
    : m_model (model),
      m_context (0)
{
    model->add (this);    
}

IgUIDRep::~IgUIDRep (void)
{
    if (! m_context)
	return;
    
    if (m_context->rep ())
    {
	ASSERT (m_context->rep () == this);
	m_context->erase (false);
	delete m_context;	
    }    
}

IgUIDRep::IDType
IgUIDRep::id (void)
{
    return (IDType) (this);
}

std::string
IgUIDRep::stringId (void)
{
    return std::string ((QString::number ((IDType) this)).ascii ());
}

IgRepContext *
IgUIDRep::context (void) const
{
    ASSERT (m_context);    
    return m_context;    
}

void
IgUIDRep::context (IgRepContext *context)
{
    ASSERT (context);    
    ASSERT (! m_context);
    m_context = context;    
}

IgModel *
IgUIDRep::model (void) const
{
    return m_model;    
}
