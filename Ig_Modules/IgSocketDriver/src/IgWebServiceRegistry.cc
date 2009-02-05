//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSocketDriver/interface/IgWebServiceRegistry.h"
#include "Ig_Modules/IgSocketDriver/interface/IgWebServiceDB.h"
#include "Ig_Modules/IgSocketDriver/interface/IgWebServiceProxy.h"
#include "Ig_Modules/IgSocketDriver/interface/IgWebService.h"
#include "Ig_Modules/IgSocketDriver/interface/IgSessionManager.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Framework/IgPluginManager/interface/IgPluginFactory.h"
#include <classlib/utils/StringOps.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>
#include <qdatastream.h>
#include <qsocket.h>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>

lat::logflag LFweb = { 0, "web", true, -1 };

//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
IG_DEFINE_STATE_ELEMENT (IgWebServiceRegistry, "WebServices");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>



IgWebServiceRegistry::IgWebServiceRegistry (IgState *state)
     : m_state (state)
{
    LOG (0, trace, LFweb, 
	 "Creating and registering services." << lat::indent << std::endl;);
    
    using namespace lat;
    
    state->put (s_key, this);
    IgWebServiceDB *db = IgWebServiceDB::get ();    
    ASSERT (db);
    if (db->begin () == db->end ())
    {
	LOG (0, trace, LFweb, 
	     "Warning: no web services registered" << lat::undent << std::endl);
	return;	
    }
    
    for (IgWebServiceDB::Iterator i = db->begin ();
	 i != db->end ();
	 i ++)
    {
	IgWebServiceProxy *service = new IgWebServiceProxy (state, *i);
	StringList pluginInfo = StringOps::split ((*i)->name (),"?");	
	StringList aliases = StringOps::split (pluginInfo [0], ";");
	for (StringList::iterator j = aliases.begin ();	
	     j != aliases.end ();
	     j++)
	{
	    LOG (0, trace, LFweb, lat::indent 
		 << "Registering service: " << *j << " " 
		 << service << lat::undent << std::endl);
	    this->registerService (*j, service);
	}
    }
    LOG (0, trace, LFweb, lat::undent);
}

IgWebServiceRegistry::~IgWebServiceRegistry (void)
{
    m_state->detach (s_key);
}

void
IgWebServiceRegistry::registerService (const std::string &alias, IgWebServiceProxy *service)
{
    m_serviceMap.insert (std::pair <std::string, IgWebServiceProxy *> (alias,
								  service));
}

IgWebService *
IgWebServiceRegistry::getService (const char *label)
{
    WebServiceMap::iterator i = m_serviceMap.find (label);
    if (i == m_serviceMap.end ())
	return 0;
    return i->second->webService ();    
}

bool
IgWebServiceRegistry::executeURL (QSocket &socket, 
				  const std::string &url, 
				  const std::string &idCookie,
				  QByteArray bodyData)
{
    
    LOG (0, trace, LFweb, "Executing URL: " << url << std::endl);
    
    using namespace lat;
    StringList split1 = StringOps::split (url, "?");
    if (split1.empty ())	
    {
	LOG (0, trace, LFweb, "Empty URL!" << std::endl);	
	return false;
    }
    
    if (split1.size () > 2)
    {
	LOG (0, trace, LFweb, "More than two '?'!" << std::endl);	
	return false;
    }
    
    LOG (0, trace, LFweb, "Web path:" << split1[0] << std::endl);
    
    StringList servicePath = StringOps::split (split1[0], "/", 
					       StringOps::TrimEmpty);
    
    if (servicePath.empty ())
    {
	LOG (0, trace, LFweb, "No path" << std::endl);
	return false;
    }
    
    std::string serviceName = "Studio";
    std::string methodName = "login";
        
    if (servicePath.size () < 2)
    {
	LOG (0, trace, LFweb, "Not enough elements in the path." << std::endl);
	return false;
    }
    else
    {
	serviceName = servicePath[servicePath.size () - 2];
	methodName = servicePath[servicePath.size () - 1];
    }
    
    LOG (0, trace, LFweb, "Service: " << serviceName << " Method: " << methodName << std::endl);
        
    IgWebService::ArgumentsMap arguments;
    if (split1.size () == 2)
    {
	LOG (0, trace, LFweb, "CGI arguments: " << split1[1] << std::endl << lat::indent);	
	StringList args = StringOps::split (split1[1], "&");
	for (StringList::iterator i = args.begin ();
	     i != args.end ();
	     i++)
	{	    
	    LOG (0, trace, LFweb, "Parsing argument: " << *i << std::endl);
	    
	    StringList keyAndValue = StringOps::split (*i, "=");

	    if (keyAndValue.empty ())
	    {
		LOG (0, trace, LFweb, "Empty argument string!" << std::endl << lat::undent << lat::undent);		
		return false;
	    }

	    if (keyAndValue.size () > 2)
	    {
		LOG (0, trace, LFweb, "More then one '=' !" << std::endl << lat::undent << lat::undent);		
		return false;
	    }
	    
	    if (keyAndValue.size () == 1)
		arguments.insert (std::pair<std::string, std::string> (keyAndValue[0], 
								       ""));
	    
	    if (keyAndValue.size () == 2)
	    {
		std::string key = keyAndValue[0];
		std::string value = StringOps::replace (keyAndValue[1], "%20", " ");
		value = StringOps::replace (value, "%3A", ":");
		value = StringOps::replace (value, "%7B", "{");
		value = StringOps::replace (value, "%7D", "}");
		arguments.insert (std::pair<std::string, std::string> (key, value));
	    }
	    
	    LOG (0, trace, LFweb, "Key: " << keyAndValue[0]
		 << " Value: " << keyAndValue[1] << std::endl << lat::undent);			    
	}
    }

    WebServiceMap::iterator i = m_serviceMap.find (serviceName);
    if (i == m_serviceMap.end ())
    {
	LOG (0, trace, LFweb, "Webservice " << serviceName << " not found " 
	     << std::endl);
	for (WebServiceMap::iterator i = m_serviceMap.begin ();
	     i != m_serviceMap.end ();
	     i++)
	{
	    LOG (0, trace, LFweb, "Registered web service: *" 
		 << i->first << "*" << i->second << std::endl);	    
	}
	
	return false;
    }

    IgSessionManager *sm = IgSessionManager::get (m_state);
    ASSERT (sm);
    
    IgState *state = sm->getSession (idCookie);

    if (!state)
    {
	state = sm->createSession (idCookie, m_state);	
    }

    LOG (0, trace, LFweb, "User state:" << state << std::endl);    
    ASSERT (state);
    
    IgWebService *service = i->second->webService ();
    IgWebService::Arguments args (state, 
				  &socket, 
				  &arguments, 
				  idCookie,
				  bodyData);    
    IgWebService::WebServiceCallback callback 
	= service->callback (methodName);
    
    
    if ( ((bool) callback) == false)
    { 
	LOG (0, trace, LFweb, "Method " << methodName << " not available" 
	     << std::endl);	    
	return false;	
    }
    
    callback (&args);
    
    
    return true;    
}
