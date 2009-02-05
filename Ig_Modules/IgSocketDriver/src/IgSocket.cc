//<<<<<< INCLUDES                                                       >>>>>>

#include <classlib/utils/Log.h>
# undef emit

#include "Ig_Modules/IgSocketDriver/interface/IgSocket.h"
#include "Ig_Modules/IgSocketDriver/interface/IgServerSocket.h"
#include "Ig_Modules/IgSocketDriver/interface/IgWebServiceRegistry.h"
#include "Ig_Modules/IgSocketDriver/interface/IgServerPool.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include <classlib/utils/StringOps.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qbuffer.h>
#include <qregexp.h>
#include <qtextstream.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <sys/socket.h>


//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

lat::logflag LFsocket = { 0, "socket", true, -1 };

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSocket::IgSocket (IgState *state)
    : m_state (state),
      m_userAgent("")
{ }

void
IgSocket::dump (void) 
{
    LOG (0, trace, LFsocket, "Connection on port: " 
	 << this->port () << lat::indent << std::endl);

    if (! this->canReadLine ())
    {
	LOG (0, trace, LFsocket, "No HTTP command found " 
	     << lat::undent << std::endl);
	
	return;	
    }
    
    QString commandLine = this->readLine ();
    LOG (0, trace, LFsocket, std::string ("HTTP command line: ") 
	 << commandLine.ascii () << std::endl);
    
    QStringList tokens = QStringList::split (QRegExp ("[ \n\r][ \n\r]*"), 
					     commandLine);
    std::string command = tokens[0].ascii ();
    
    typedef std::map <std::string, std::string> CookieMap;

    CookieMap cookieMap;
        
    if (! this->canReadLine ())
    {
	LOG (0, trace, LFsocket, std::string ("HTTP request has no additional info") 
	     << commandLine.ascii () << std::endl);
    }
    

    QString httpOptLine = this->readLine ();
    int postSize = 0;
    
    while (httpOptLine.contains (":"))
    {	
	LOG (0, trace, LFsocket, std::string ("HTTP Option Line:") 
	     << httpOptLine.ascii () << std::endl);
	    
	QStringList httpOptTokens = QStringList::split (QRegExp ("[ \n\r][ \n\r]*"), 
							httpOptLine);

	if (httpOptTokens[0] == "Cookie:")
	{
	    for (unsigned int i = 1;
		 i < httpOptTokens.size ();
		 i++)
	    {
		QStringList keyValue = QStringList::split ("=", httpOptTokens[i]);
		if (keyValue.size () == 2)
		{
		    cookieMap.insert (CookieMap::value_type (keyValue [0], 
							     keyValue [1].remove (";")));            
		}           
	    }
	}
	else if (httpOptTokens[0] == "Content-Length:")
	{
	    postSize = atoi (httpOptTokens[1]);	    
	    LOG (0, trace, LFsocket, std::string ("Post size: ") 
		 << httpOptTokens[1].ascii () << std::endl);
	}
	else if(httpOptTokens[0] == "User-Agent:")
	{
	    m_userAgent = httpOptLine.mid(11).stripWhiteSpace ().latin1();
	}
	else
	{	
	    LOG (0, trace, LFsocket, std::string ("Unrecognized option:") 
		 << httpOptTokens[0].ascii () << std::endl);

	}	    
	httpOptLine = this->readLine ();
    }
    
    QByteArray bodyData;
    
    // Get the remainings.
    if (postSize > 0)
    {
	std::string continueMsg = "HTTP/1.1 100 Continue\r\n\r\nConnection: close\r\n\r\n";
	
	this->writeBlock (continueMsg.c_str (), continueMsg.size ());
	this->flush ();

	LOG (0, trace, LFsocket, "Sending request to continue" << std::endl);
	
	char * buffer = new char[postSize];
       	int sizeRead = this->readBlock (buffer, postSize);
	
	if (sizeRead != postSize)
	{
	    LOG (0, trace, LFsocket, std::string ("WARNING: expecting ")
		 << QString::number (postSize).ascii () << " bytes, but only "
		 << QString::number (sizeRead).ascii () << " read" << std::endl);
	}
	
	bodyData.assign (buffer, postSize);
	LOG (0, trace, LFsocket, std::string ("Body:")
	     << QString (bodyData).ascii () << std::endl);
    }    	               

    
    // Cookie logging
    LOG (0, trace, LFsocket, "Client sent the following cookies: " 
	 << lat::indent << std::endl);
    for (CookieMap::iterator i = cookieMap.begin ();
	 i != cookieMap.end ();
	 i++)
    {
	LOG (0, trace, LFsocket, i->first 
             << "=" 
             << i->second << std::endl);        
    }
    LOG (0, trace, LFsocket, lat::undent);
        
    std::string idCookie = cookieMap["USER"];

    LOG (0, trace, LFsocket, "IDCookie: " << idCookie << std::endl);
    std::string url = tokens[1].ascii ();
    
    IgServerSocket *server = IgServerSocket::get (m_state);
    std::string proxy = server->cmdLineProxy();
    std::string url1(url);
    int index = url1.size()-1;
    while((index>0) && (url1.rfind("/") == index))
    {url1=url1.substr(0, index); index--;}
    if ((cookieMap.find("cookies")==cookieMap.end()) || 
        (url1 == proxy + "/iguanainit") ||
	(url1 == proxy + "/iguana") ||
	(url1 == proxy + "/"))
    {
        server->clientInitialization (this);
        return;
    }
    
    if (url1 == proxy + "/ping")
    {
        server->ping (this);
        return;
    }
    else if (url1 == "/favicon.ico")
    {
        this->close ();
	return;
    }
    
    IgServerPool *serverPool = IgServerPool::get (m_state);
    if (serverPool)
    {
	sendData (serverPool->redirect (url, idCookie));
	serverPool->addServer ();
    }
    else
    {
        IgWebServiceRegistry *registry 
	    = IgWebServiceRegistry::get (m_state);
        if (! registry)
        {
	    LOG (0, trace, LFsocket, "FATAL: webservice registry not found "
	          << std::endl);
        }

	std::string clientCookieID = server->getClientCookieID();
	bool executeURL = false;
	if (clientCookieID.empty ())
	{
	    //We will be here only when this server is not already serving
	    //any other client. This mean either main server has redirected
	    //a client to this or a client has directly contacted.
	    
	    //If cookie is "unknown user" then this mean that client has tried
	    //to connect directly. In this case redirect him/her to main server
	    //else reserve this server for this client
	    url = server->cmdLineProxy() + "/Studio/login";
	    if (idCookie.empty ())
	    {
		server->generateCookie (idCookie);
		if (!server->cmdLineMainHostUrl ().empty ())
		{
		    sendData (server->redirect (url, server->cmdLineMainHostUrl (),
		                                idCookie));
		}
		else
		{
		    server->setClientCookieID(idCookie);
		    sendData (server->redirect (url, server->hostnameUrl (),
		                                idCookie));
		}
	    }
	    else
	    {
		server->setClientCookieID(idCookie);
		executeURL = true;
	    }
	}
	else if (clientCookieID != idCookie)
	{
	    //This server is already serving a different clicnt
	    //so redirect it to main server if exists otherwise 
	    //tell the client that server is already busy
	    if (!server->cmdLineMainHostUrl ().empty ())
	    {
		sendData (server->redirect (proxy + "/Studio/login",
		                            server->cmdLineMainHostUrl (),
		                            server->generateCookie (idCookie)));
	    }
	    else
		serverBusyMessage ();
	}
	else
	    executeURL = true;
        if (executeURL)
        {
	    registry->executeURL (*this, 
	  	                  url, 
			          idCookie, 
			          bodyData);
	    this->close ();
	    server->startTimer ();
        }
    }

    LOG (0, trace, LFsocket, lat::undent);
}

const std::string&
IgSocket::userAgent (void) const
{ return m_userAgent; }

void
IgSocket::sendData (const std::string& data)
{
    QTextStream client (this);
    client << data;
    this->close ();
    this->flush ();
}

void
IgSocket::serverBusyMessage ()
{
    std::string result = "HTTP/1.1 200 Ok\r\n"
	         "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n"
	         "<html><body>"	     
	         "<style  type=\"text/css\">"
	         "a {color:#FFFFFF}"
	         "</style>"
	         "<h3>Server is busy.</h3>"
		 "Server is already serving to other client please try server."
	         "</body>"
	         "</html>\r\n\r\n";
    sendData(result);		 
}
