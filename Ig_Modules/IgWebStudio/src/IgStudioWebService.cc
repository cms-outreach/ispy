//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgWebStudio/interface/IgStudioWebService.h"
#include "Ig_Modules/IgSocketDriver/interface/IgWebServiceDB.h"
#include "Ig_Modules/IgSocketDriver/interface/IgSocketDriverInfo.h"
#include "Ig_Modules/IgSocketDriver/interface/IgServerSocket.h"
#include "Ig_Modules/IgSocketDriver/interface/IgFileManager.h"
#include "Ig_Modules/IgSocketDriver/interface/IgSocket.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgArgsElement.h"
#include <qfile.h>
#include <qiodevice.h>
#include <qcstring.h>
#include <qstringlist.h>
#include <qregexp.h>
#include <qstring.h>
#include <qapplication.h>
#include <string>
#include <map>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <signal.h>
#undef emit
#include <classlib/utils/Callback.h>
#include <classlib/utils/StringOps.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/SubProcess.h>
#include <classlib/utils/Argz.h>
#include <classlib/utils/Log.h>
#include <classlib/iobase/Filename.h>
#include <unistd.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

lat::logflag LFstudioWeb = { 0, "studioWeb", true, -1 };

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgStudioWebService::IgStudioWebService (IgState *state)
    : IgWebService (state)
{
    //added in reverse order as we are prepending these paths
    getFileManager ()->addPaths ("/share/htdocs/Ig_Modules/IgWebStudio/html/",
    				 true);
    getFileManager ()->addPaths ("/src/Ig_Modules/IgWebStudio/images/", true);
    getFileManager ()->addPaths ("/src/Ig_Modules/IgWebStudio/html/", true);

    registerCallback ("initSession", lat::CreateCallback (this, &IgStudioWebService::initSession));
    registerCallback ("login", lat::CreateCallback (this, &IgStudioWebService::login));    
    registerCallback ("logout", lat::CreateCallback (this, &IgStudioWebService::logout));    
    registerCallback ("launch", lat::CreateCallback (this, &IgStudioWebService::launch));    
}

void
IgStudioWebService::login (Arguments *arguments)
{
    QIODevice *outputDevice = arguments->outputDevice ();
    IgState *state = arguments->state ();
    QTextStream os (outputDevice);
    LOG (0, trace, LFstudioWeb, "Cookie: " 
     << arguments->idCookie () << std::endl);
    
    IgArgsElement *args = IgArgsElement::get (state);
    if (args->exists ("--demo"))
    {
        os << "HTTP/1.1 200 Ok\r\n"
           << "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n"
           << "<html>"
           << "<head><title>DEMO IN PROGRESS</title></head>"
           << "<body><H1>DEMO IN PROGRESS</H1>In order to avoid "
           << "interference with the demo, external users cannot login "
           << "to the IGUANA server.<br/><br/>"
           << "You are kindly requested to look at the speaker.</body>"
           << "</html>\r\n";        
    }
    else
	os << IgServerSocket::redirect (IgServerSocket::cmdLineProxy() + "/Studio/initSession", "", "");
}

void
IgStudioWebService::logout (Arguments *arguments)
{
    QIODevice *outputDevice = arguments->outputDevice ();
    QTextStream os (outputDevice);
    LOG (0, trace, LFstudioWeb, "Cookie: " 
     << arguments->idCookie () << std::endl);
    
    os << "HTTP/1.1 200 Ok\r\n"
       << "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n"
       << "<html><head><title>IGUANA Web Server</title></head><body>"
       <<"<H2>IGUANA Server Logout</H2>Thanks a lot for using IGUANA Web Server. "
       <<"Please do visit <A HREF=\""<<IgServerSocket::cmdLineProxy()<<"\"> IGUANA server</A> again.<br/><br/>"
       <<"<b>--IGUANA Team</b></body></html>\r\n";
    outputDevice->close();
    outputDevice->flush();
    exit (0);
}

void
IgStudioWebService::launch (Arguments *arguments)
{
    QIODevice *outputDevice = arguments->outputDevice ();
    ArgumentsMap &args = *(arguments->args ());
    QTextStream os (outputDevice);
    
    if (args.find ("script") == args.end ())
    {
        os << IgServerSocket::redirect (IgServerSocket::cmdLineProxy() + "/Studio/login", "", "");	
	return;
    }
    
    if (m_sessionUrl.empty ())
   	m_sessionUrl = args["script"];
    
    os << IgServerSocket::redirect (m_sessionUrl, "", "");
}

void
IgStudioWebService::initSession (Arguments *arguments)
{
    QIODevice *outputDevice = arguments->outputDevice ();
    QTextStream os (outputDevice);
    LOG (0, trace, LFstudioWeb, "Cookie: " 
     << arguments->idCookie () << std::endl);
    
    if (! m_sessionUrl.empty ())
    {
    	os << IgServerSocket::redirect(m_sessionUrl, "", "");
	return;
    }
    
    IgSocket *socket = dynamic_cast<IgSocket *>(outputDevice);
    bool validUserAgent = true;
    std::string userAgent ("");
    if(socket)
    {
        userAgent = socket->userAgent();
        QStringList agents = QStringList::split (QRegExp ("[ \n\r][ \n\r]*"),
					         userAgent.c_str());
        for(int i=0; i<agents.size();i++)
        {
            if(agents[i]=="MSIE")
	    {
	        validUserAgent=false;
	        break;
	    }
        }
    }
    
    IgWebServiceDB *wsdb = IgWebServiceDB::get ();
    std::string proxy = IgServerSocket::cmdLineProxy ();
    
    os << "HTTP/1.0 200 Ok\r\n"
    "Content-Type: text/html; charset=\"utf-8\"\r\n"  
    "\r\n";
    
    os << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
        "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
    "<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"en\" xml:lang=\"en\">\n"
    "<head>\n"
    "<title>IGUANA WEB STUDIO</title>"
    "<link rel=\"stylesheet\" type=\"text/css\" href=\""+proxy+"/Studio/getScript?name=main.css&amp;type=text/css\"/>\n"
    "</head><body>\n"
    "<div style=\"display: none;\">Welcome user " + arguments->idCookie () + "</div>"
    "<div class=\"main\"><div align=right><a href=\""<<proxy<<"/Studio/logout\">Logout</a></div>"
    "<div class=\"title\">IGUANA WEB INTERFACE</div><table><tr>";
    int j = 0;
    
    for (IgWebServiceDB::Iterator i = wsdb->begin ();
     i != wsdb->end (); 
     i++)
    {   
        if (j > 4)
        {
            j = 0;
            os << "</tr><tr>";
        }

        using namespace lat;
        std::map <std::string, std::string> pluginOpts;
    
        StringList pluginInfo = StringOps::split ((*i)->name (), "?");
        if (pluginInfo.size () == 2)
        {
            StringList keyValues = StringOps::split (pluginInfo [1], "&");
            for (StringList::iterator i = keyValues.begin ();
             i != keyValues.end ();
             i++)
            {
                StringList keyValue = StringOps::split ((*i), "=");
                if (keyValue.size () == 2)
                {
                    typedef std::pair <std::string, std::string> MapPair;
                    pluginOpts.insert (MapPair (keyValue[0], keyValue [1]));
                }
            }
        
            if (pluginOpts.find ("app") != pluginOpts.end ())
            {
                os << "<td><a href=\""<<proxy<< "/Studio/launch?script=" <<proxy<< pluginOpts["url"] << "\">"
                    "<img src=\"" + pluginOpts["img"] + "\" alt=\"No Image\"/><br/>"
                    + pluginOpts["name"] + "</a></td>";
                j++;
            }
        }
    }
    os << "</tr></table>";
    if(!validUserAgent)
    {
        os <<"<br><div class=\"warning\"><center><b>IGUANA Server Warning:</b></center>"
	     "We are sorry but IGUANA server might not work properly for your broswer \"" + userAgent + "\".</div>\n";
    }
    os << "</div></body></html>\r\n";
}


const char *
IgStudioWebService::catalogLabel (void)
{
    return "Studio";    
}
