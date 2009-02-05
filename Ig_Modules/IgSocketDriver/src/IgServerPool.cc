//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSocketDriver/interface/IgServerPool.h"
#include "Ig_Modules/IgSocketDriver/interface/IgServerSocket.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgArgsElement.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include <classlib/iobase/SubProcess.h>
#include <classlib/iobase/TempFile.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/StringOps.h>
#include <classlib/utils/StringList.h>
#include <classlib/iobase/Filename.h>
#include <classlib/utils/Argz.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <sys/wait.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

IgServerPool* IgServerPool::m_instance = 0;

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgServerPool, "Server pool");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

void childServerFinished (int signum)
{
    if (signum != SIGCHLD) return;
    int status;
    IgServerPool::getInstance(0)->childTerminated(wait (&status));
}
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>
IgServerPool* IgServerPool::getInstance (IgState *state)
{
    if (!m_instance)
        m_instance = new IgServerPool(state);
    return m_instance;
}

IgServerPool::IgServerPool (IgState *state)
    : m_state (state), 
      m_argz (new lat::Argz ("iguana")),
      m_maxProcesses (65000),
      m_processPoolSize(2),
      m_hostname ("")
{
    m_state->put (s_key, this);
    signal (SIGCHLD, childServerFinished);
    
    IgArgsElement *args = IgArgsElement::get (state);
    std::string processPoolSize = QString::number(m_processPoolSize);
    if (args->find ("--ig-process-pool-size", processPoolSize))
        m_processPoolSize = atoi (processPoolSize.c_str ());

    std::string maxProcesses = QString::number(m_maxProcesses);
    if (args->find ("--ig-max-processes", maxProcesses))
        m_maxProcesses = atoi (maxProcesses.c_str ());
	
    ASSERT (m_processPoolSize <= m_maxProcesses);

    m_hostname = IgServerSocket::cmdLineHostname(m_state);
    ASSERT (!m_hostname.empty());

    m_argz->add ("-D");
    m_argz->add ("SOCKET");
    
    if (IgServerSocket::cmdLineProxy(state).empty())
    {
        m_argz->add ("--ig-main-hosturl");
        m_argz->add (m_hostname + ":" +
                     QString::number(IgServerSocket::cmdLinePort(state)));
    }
    // copy the arguments from command-line
    // map of "<command-line arg>=<has value?>" which should not be passed to
    // child server.
    std::map <std::string, bool> ignoreArgs;
    ignoreArgs ["--ig-port"]=true;
    ignoreArgs ["--ig-server"]=false;
    ignoreArgs ["--ig-main-hosturl"]=true;
    ignoreArgs ["--ig-process-pool-size"]=true;
    ignoreArgs ["--ig-max-processes"]=true;
    ignoreArgs ["-D"]=true;
    ignoreArgs ["--driver"]=true;
	
    for (int i = 1 ; i < args->args (); i++)
    {
	std::string arg (args->arg (i));
	if (arg.empty ()) continue;
	if (ignoreArgs.find (arg) != ignoreArgs.end())
	{
	    if (ignoreArgs[arg])
	        i++;
	}
	else
	    m_argz->add (args->arg (i));
    }

    for (int i = 0; i < m_processPoolSize; i++)
	addServer ();
}

IgServerPool::~IgServerPool (void)
{
    m_state->detach (s_key);    
}


std::string
IgServerPool::createProcess (void)
{
    lat::Argz argz (*m_argz);
    int childPort = IgServerSocket::getFreePort ();
    std::string portValue = QString::number (childPort).latin1 ();    
    argz.add ("--ig-port");
    argz.add (portValue);
    lat::SubProcess *process = new lat::SubProcess ();
    
    std::cerr << argz.quote () << std::endl;
    
    lat::Argz shellCommand ("/bin/sh");
    shellCommand.add ("-c");
    lat::Filename directoryName (lat::TempFile::tempdir().asDirectory());
    directoryName.append ("IGUANAWEB/");
    directoryName.makedir(directoryName, 0700, true, true);
    lat::TempFile::dir (directoryName);
    
    shellCommand.add (std::string ("\"cd ") + directoryName.name () + "; "+
                      argz.quote () + "\"");
    std::cerr << shellCommand.quote () << std::endl;
    
    pid_t pid = process->run (shellCommand.argz ());
    
    std::string result = m_hostname + ":" + portValue;    
    m_pidServerMap.insert (std::pair<pid_t, std::string>(pid,result));
    
    return result;
}

std::string
IgServerPool::redirect (const std::string& url, std::string& cookie)
{
    std::string result;
    std::string redir;

    if (cookie.empty())
      IgServerSocket::generateCookie (cookie);
      
    if (m_cookieServerMap.find (cookie) != m_cookieServerMap.end ())
    {
	redir = m_cookieServerMap[cookie];
    }
    else 
    {
	if (m_availableServers.size ())
	{
	    // We have atleast one child server available
	    redir = *(m_availableServers.begin ());
	    m_availableServers.erase (m_availableServers.begin ());
	    m_cookieServerMap.insert (std::pair<std::string, std::string>
	                              (cookie,redir));
	}
	else
	{
	    // Already serving max number of clients.
            result = "HTTP/1.1 200 Ok\r\n"
	             "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n"
	             "<html><body>"	     
	             "<style  type=\"text/css\">"
	             "a {color:#FFFFFF}"
	             "</style>"
	             "<h3>Server is busy.</h3>"
		     "Max number of clients are already connected. Please try again later."
	             "</body>"
	             "</html>\r\n\r\n";
    
            std::cerr << result << std::endl;
    
            return result;    
	}
    }  
    
    return IgServerSocket::redirect (url, redir, cookie);
}

void
IgServerPool::addServer (void)
{
    if (((int)m_pidServerMap.size() < m_maxProcesses) &&
        ((int)m_availableServers.size () < m_processPoolSize))
    {
        m_availableServers.insert (createProcess ());
    }
}


void
IgServerPool::childTerminated (int pid)
{
    PIDToServerMap::iterator pidItr = m_pidServerMap.find(pid);
    if (pidItr != m_pidServerMap.end())
    {
	std::string server = m_pidServerMap[pid];
	std::cerr <<"Server terminated:"<<server<<std::endl;
	m_pidServerMap.erase(pidItr);
	if (m_availableServers.find(server)!=m_availableServers.end())
	    m_availableServers.erase (server);
        CookieToServerMap::iterator itr = m_cookieServerMap.begin();
        for(itr = m_cookieServerMap.begin();
	    itr != m_cookieServerMap.end();
	    itr++)
	{
	    if (itr->second == server)
		m_cookieServerMap.erase (itr);
	}
	addServer ();
    }
}

int 
IgServerPool::availableServers (void)const
{
    return m_availableServers.size ();    
}

int
IgServerPool::processPoolSize (void) const
{ return m_processPoolSize; }
