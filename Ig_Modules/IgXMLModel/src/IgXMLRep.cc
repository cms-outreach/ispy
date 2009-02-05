//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgXMLModel/interface/IgXMLRep.h"
#include "Ig_Modules/IgXMLModel/interface/IgXMLModel.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include <qdom.h>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgXMLRep::IgXMLRep (IgXMLModel *model, 
		    IgXMLRep *parent,
		    IgXMLRep *before,
		    const std::string &tagName)
    : QDomElement (model->document ()->createElement (tagName.c_str ())), 
      m_model (model),
      m_context (0)
{    
    /* WORKAROUND: Make sure that the IgXMLReps are not inserted using
       the QDomNode API.*/
    model->trackRep (this);
    
    if (parent && before)
    {
	parent->insertBefore (*this, *before);	
    }
    else if (parent)
    {
	parent->appendChild (*this);	
    }
    else 
    {
	model->document ()->appendChild (*this);	
    }    
}

IgXMLRep::~IgXMLRep (void)
{
    ASSERT (m_context);
    if (m_context->rep ())
    {	
        ASSERT (m_context->rep () == this);
	m_context->erase (false);
	delete m_context;
    }
}


IgRepContext *
IgXMLRep::context (void) const
{
    return m_context;    
}

void
IgXMLRep::context (IgRepContext *context)
{
    m_context = context;    
}

IgModel *
IgXMLRep::model (void) const
{
    return m_model;    
}

