//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgXMLModel/interface/IgXMLModel.h"
#include "Ig_Modules/IgXMLModel/interface/IgXMLRep.h"
#include <qdom.h>
#include <string>
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

IgXMLModel::IgXMLModel (const std::string &/*documentName*/,
			const std::string &/*documentType*/)
    : m_document (new QDomDocument ()),
      m_dispatcher (new Dispatcher)
{
}

IgXMLModel::~IgXMLModel (void)
{
    ASSERT (m_document);
    ASSERT (m_dispatcher);

    for (std::list<IgXMLRep *>::iterator i = m_repList.begin ();
	 i != m_repList.end ();
	 i++)
    {	
	delete (*i);	
    }
    
    delete m_document;
    delete m_dispatcher;
}


IgXMLModel::Dispatcher *
IgXMLModel::dispatcher (enum EventType eventType)
{
    if (eventType == CHANGED)
    {
	return m_dispatcher;    
    }
    return 0;    
}

void
IgXMLModel::trackRep (IgXMLRep *rep)
{
    m_repList.push_back (rep);    
}

QDomDocument * 
IgXMLModel::document (void)
{
    return m_document;
}
