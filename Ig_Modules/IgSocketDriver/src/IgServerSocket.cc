//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSocketDriver/interface/IgServerSocket.h"
#include "Ig_Modules/IgSocketDriver/interface/IgSocket.h"
#include "Ig_Modules/IgSocketDriver/interface/IgSessionManager.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgArgsElement.h"
#include "Ig_Modules/IgSocketDriver/interface/IgServerPool.h"
#include "Ig_Modules/IgSocketDriver/interface/IgFileManager.h"
#include "Ig_Modules/IgSocketDriver/interface/IgWebService.h"
#include <classlib/utils/StringOps.h>
#include <classlib/utils/StringList.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <classlib/utils/UUID.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>

class IgFreeServerSocket : public QServerSocket
{
public:
    IgFreeServerSocket () : QServerSocket (0, 1, (QObject *) 0, 0)
    {}
    virtual void newConnection (int) {}
};

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>

std::string IgServerSocket::m_hostname = "";
std::string IgServerSocket::m_mainHostUrl = "";
std::string IgServerSocket::m_proxy = "";
int IgServerSocket::m_port = 0;

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgServerSocket, "Server socket");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgServerSocket::IgServerSocket (IgState *state, bool mainPort /* = true */)
    : QServerSocket (cmdLinePort (state, mainPort))
{
    initialize (state);
    if (mainPort)
        new IgSessionManager (state);
}

void
IgServerSocket::initialize (IgState *state)
{
    m_state = state;
    m_state->put (s_key, this);
    m_timeout = -1;
    m_timer = 0;
    m_clientCookieID = "";
    
    cmdLineProxy (state);	
    cmdLinePort (state);
    cmdLineHostname (state);
    ASSERT (!m_hostname.empty());
    
    if (!IgServerPool::get (m_state))
    {
        IgArgsElement *args = IgArgsElement::get (m_state);
	args->find ("--ig-main-hosturl", m_mainHostUrl);
	args->find ("--ig-timeout", m_timeout);
    
        if (m_timeout > 0)
        {
            m_timer = new QTimer (this);
	    connect (m_timer, SIGNAL (timeout()), this, SLOT (timedOut()));
        }
    }
    std::cerr << "Bound to port:" <<this->port () << std::endl;
}

void
IgServerSocket::newConnection (int port)
{
    if (!m_state) return;
    IgSocket *socket = new IgSocket (m_state);
    connect (socket, SIGNAL(readyRead()), socket, SLOT(dump()));
    //connect (socket, SIGNAL(delayedCloseFinished()),
    //         this, SLOT(socketClosed()));
    socket->setSocket (port);
}

void
IgServerSocket::startTimer ()
{
    if (m_timer)
        m_timer->start (m_timeout*1000, true);
}

void
IgServerSocket::timedOut ()
{ exit (0); }

void
IgServerSocket::socketClosed ()
{
    IgSocket* socket = (IgSocket*)sender ();
    if (socket)
        delete socket;
}

void
IgServerSocket::setClientCookieID (const std::string& cookieID)
{ m_clientCookieID = cookieID; }

const std::string&
IgServerSocket::getClientCookieID () const
{ return m_clientCookieID; }

const std::string&
IgServerSocket::cmdLineProxy ()
{ return m_proxy; }

const std::string&
IgServerSocket::cmdLineMainHostUrl ()
{ return m_mainHostUrl; }

const std::string&
IgServerSocket::cmdLineHostname ()
{ return m_hostname; }

int
IgServerSocket::cmdLinePort ()
{ return m_port; }

std::string
IgServerSocket::hostnameUrl ()
{ return m_hostname+":"+QString::number(cmdLinePort ()); }

const std::string&
IgServerSocket::cmdLineHostname (IgState *state)
{
    cmdLineProxy (state);
    IgArgsElement *args = IgArgsElement::get (state);
    args->find ("--ig-hostname", m_hostname, false);
    if (m_proxy.empty() && !m_hostname.empty())
      m_hostname = "http://" + m_hostname;
    return m_hostname;
}

const std::string&
IgServerSocket::cmdLineProxy (IgState *state)
{
    IgArgsElement *args = IgArgsElement::get (state);
    args->find ("--ig-proxy", m_proxy, false);
    if(!m_proxy.empty() && m_proxy[0] != '/')
        m_proxy = "/"+ m_proxy;
    return m_proxy;
}

int
IgServerSocket::cmdLinePort (IgState *state, bool mainPort /* = true */)
{
    if (mainPort)
    {
        IgArgsElement *args = IgArgsElement::get (state);
        args->find ("--ig-port", m_port, false);
        return m_port;
    }
    return 0;
}

const std::string&
IgServerSocket::generateCookie (std::string& cookie)
{
    using namespace lat;
    char *uuidBuffer = new char[37];    
    UUID uuid = UUID::generateTime ();
    uuid.format (uuidBuffer);
    cookie = uuidBuffer;
    delete[] uuidBuffer;
    return cookie;
}

std::string
IgServerSocket::redirect (const std::string& url, const std::string& redir,
                          const std::string& cookie)
{
    std::string targetCookie = "";
    std::string newRedir = redir;
    
    if (!cookie.empty())
    {
        if (m_proxy.empty())
            targetCookie = "Set-Cookie: USER=" + cookie +"; path=/\r\n";
	else
	    targetCookie = "Set-Cookie: USER=" + cookie +"; path=" + m_proxy + "\r\n";
    }
      
    if (!m_proxy.empty())
    {
	if (!redir.empty())
	    targetCookie += "Set-Cookie: igtarget=" + redir + "; path=" + m_proxy + "\r\n";
	newRedir = url;
    }
    else
	newRedir += url;
    
    std::string result = "HTTP/1.1 302 Redirect\r\n"
    	     "Location: " + newRedir + "\r\n"
	     "Content-Type: text/html; charset=\"utf-8\"\r\n"
	     "Connection: Close\r\n"
	     + targetCookie +
	     "\r\n"
	     "<html>"	     
	     "<body>"
	     "<style  type=\"text/css\">"
	     "a {color:#FFFFFF}"
	     "</style>"
	     "<h1><a href=\"" + newRedir + 
	     "\">Click here if not redirected automatically</a></h1>"
	     "</body>"
	     "</html>\r\n\r\n";
    
    std::cerr << result << std::endl;
    
    return result;
}

int
IgServerSocket::getFreePort ()
{
    IgFreeServerSocket* server = new IgFreeServerSocket ();
    while (server->ok () == false)
	usleep (1000);
    int port = server->port ();
    delete server;
    return port;
}

void
IgServerSocket::ping (QIODevice *socket)
{
    IgFileManager* fileManager = getFileManager ();
    QByteArray writeBuffer = fileManager->lookup("images/ping.png");
    IgWebService::sendBinary(socket,writeBuffer,"png");
    socket->close ();
    socket->flush ();
}

void
IgServerSocket::clientInitialization (QIODevice *socket)
{
    IgFileManager* fileManager = getFileManager ();
    QByteArray writeBuffer =
      fileManager->lookup("htdocs/Ig_Modules/IgSocketDriver/html/iguanaInit.html");
    QTextStream client (socket);
    client << "HTTP/1.0 200 Ok\r\n"
	      "Content-Type: text/html;\r\n"
	      "\r\n"
              "<html><body><SCRIPT language=\"JavaScript\">var proxy =\"" +
	      m_proxy + "\";</SCRIPT>\r\n";

    socket->writeBlock (writeBuffer.data (), 
			writeBuffer.size ());
			
    client << "</body></html>\r\n";
    socket->close ();
    socket->flush ();
}

IgFileManager*
IgServerSocket::getFileManager () const
{
    IgFileManager* fileManager = IgFileManager::get (m_state);
    if (fileManager) return fileManager;
    return new IgFileManager (m_state);
}
