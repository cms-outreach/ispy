//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgWebTree/interface/IgWebTreeService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Modules/IgXMLModel/interface/IgAjaxTreeModel.h"
#include "Ig_Modules/IgXMLModel/interface/IgUIDModel.h"
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>
#include <classlib/utils/StringOps.h>
#include <classlib/utils/Callback.h>
#include <qiodevice.h>
#include <qdom.h>
#include <qtextstream.h>
#include <qdatastream.h>
#include <iostream>
#include <typeinfo>
#include <qapplication.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

//FIXME: IgUIDModel should be probably moved to a separate WebService...

IgWebTreeService::IgWebTreeService (IgState *state)
    : IgWebService (state),
      m_uidModel (new IgUIDModel ())
{
    registerCallback ("browse", lat::CreateCallback (this, &IgWebTreeService::browse));
    registerCallback ("getRepID", lat::CreateCallback (this, &IgWebTreeService::getRepID));
    registerCallback ("twigMenu", lat::CreateCallback (this, &IgWebTreeService::twigMenu));
}

void
IgWebTreeService::getRepID (Arguments *arguments)
{
    IgState *state = arguments->state ();    
    QIODevice *outputDevice = arguments->outputDevice ();
    ArgumentsMap &args = *(arguments->args ());
    
    IgDocumentData *dd = IgDocumentData::get (state);
    ASSERT (dd);
    
    std::string documentName = "";
    
    if (args.find ("doc") != args.end ())
    {
	documentName = args["doc"];
    }
    
    IgTwig *twig = dynamic_cast <IgTwig *> (dd->root (documentName));

    if (twig)
    {
	if (args.find ("t") == args.end ())
	{
	    this->sendError (outputDevice);	    
	}
       
	IgTwig *target = dynamic_cast <IgTwig *> (twig->lookup (args["t"]));
	ASSERT (target);
	IgUIDRep *uidRep 
	    = dynamic_cast <IgUIDRep *> (IgRepSet::lookup (target, 
							   m_uidModel, 
							   true));
	ASSERT (uidRep);
	QTextStream os (outputDevice);
	// os.setEncoding (QTextStream::UnicodeUTF8);
	os << "HTTP/1.1 200 Ok\r\n"
	    "content-type: text/xml; charset=UTF-8\r\n"
	    "\r\n"
	    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
	   << "<uid>" << uidRep->id () << "</uid>"
	   << "\r\n";	
    }        
}

void
IgWebTreeService::browse (Arguments *arguments)
{
    qApp->lock ();
    
    IgState *state = arguments->state ();    
    QIODevice *outputDevice = arguments->outputDevice ();
    ArgumentsMap &args = *(arguments->args ());
    
    IgDocumentData *dd = IgDocumentData::get (state);
        
    std::string documentName = "";
    
    if (args.find ("doc") != args.end ())
    {
	documentName = args["doc"];
    }
    
    IgTwig *twig = dynamic_cast <IgTwig *> (dd->root (documentName));
    
    if (twig)
    {
	twig->selfTraverse (true);
	IgAjaxTreeModel model (m_uidModel);

	IgTwig *target;
	
	if (args.find ("i") != args.end ())
	{
	    IgUIDRep * rep = m_uidModel->lookup (atoi (args["i"].c_str ()));	    
	    ASSERT (rep);	    
	    target = dynamic_cast <IgTwig *> (rep->context ()->object ());
	    ASSERT (target);
	}
	else if (args.find ("t") != args.end ())
	{
	    target = dynamic_cast <IgTwig *> (twig->lookup (args["t"]));
	    ASSERT (target);
	}
	else
	{
	    this->sendError (outputDevice);
	    return;	    
	}
	    
	    
	ASSERT (target);
		
	target->selfTraverse (true);

	if (args.find ("setVisible") != args.end ())
	{
	    if (args["setVisible"] == "1")
	    {
		target->selfVisible (true);
	    }
	    else
	    {
		target->selfVisible (false);		
	    }
	}

	IgRepSet::lookup (target, &model, true);
	IgRepSet::update (target, IgTwig::SELF_MASK 
			  | IgTwig::STRUCTURE_MASK 
			  | IgTwig::FLAGS_MASK);		
	target->expand ();
	
	IgRepSet::lookup (target, &model, true);
	
	for (unsigned int i = 0; 
	     i < target->children ();
	     i++)
	{
	    IgRepSet::lookup (target->child (i), 
			      &model, 
			      true);	    
	}

	QTextStream os (outputDevice);
	// os.setEncoding (QTextStream::UnicodeUTF8);
	os << "HTTP/1.1 200 Ok\r\n"
	    "content-type: text/xml; charset=UTF-8\r\n"
	    "\r\n"
	    "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
	   << model.document ()->toString () << "\r\n";
		    
	std::cerr << model.document ()->toString () << std::endl;
    }
    qApp->unlock ();
    
}

void
IgWebTreeService::twigMenu (Arguments *arguments)
{
    QIODevice *outputDevice  = arguments->outputDevice ();
    ArgumentsMap &args = *(arguments->args ());

    if (args.find ("i") == args.end ())
	return;
    
    IgUIDRep *rep = m_uidModel->lookup (atoi (args["i"].c_str ()));
    ASSERT (rep);
    
    IgRepresentable *object = rep->context ()->object ();

    for (MenuHooks::iterator i = m_menuHooks.begin ();	
	 i != m_menuHooks.end ();
	 i++)
    {
	std::string xmlMenuDescription = (*i)(object);
	if (xmlMenuDescription != "")
	{
	    this->sendXML (outputDevice, xmlMenuDescription);
	    return;
	}	
    }    

    this->sendXML (outputDevice, "<menu name=\"default\"/>");    
}

void
IgWebTreeService::registerMenuHook (MenuHook hook)
{
    m_menuHooks.push_back (hook);    
}


IgUIDModel *
IgWebTreeService::uidModel (void)
{
    return m_uidModel;    
}

const char *
IgWebTreeService::catalogLabel (void)
{
    return "Tree";    
}
