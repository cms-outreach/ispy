//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSocketDriver/interface/IgWebService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgEnvsElement.h"
#include "Ig_Modules/IgSocketDriver/interface/IgFileManager.h"
#include <qiodevice.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgWebService::IgWebService (IgState *state)
  : m_state(state)
{
    m_fileManager = new IgFileManager (new IgState(state));
    IgEnvsElement* envs = IgEnvsElement::get(state);
    std::string wedDirectory("");
    if (envs->getEnv ("IG_WEB_STUDIO_DIRECTORY", wedDirectory))
        m_fileManager->addSearchPath (wedDirectory);
    
    
    getFileManager ()->addPaths ("/share/images/", true);
    getFileManager ()->addPaths ("/src/", true);

    registerCallback ("getScript", lat::CreateCallback (this, &IgWebService::getScript));
    registerCallback ("getPic", lat::CreateCallback (this, &IgWebService::getPic));
}

IgWebService::~IgWebService (void)
{
}

IgState*
IgWebService::state () const
{ return m_state; }

IgFileManager*
IgWebService::getFileManager () const
{ return m_fileManager; }

void
IgWebService::getScript (Arguments *arguments)
{
    QIODevice *outputDevice = arguments->outputDevice ();
    ArgumentsMap &args = *(arguments->args ());
    QByteArray writeBuffer;
    
    if (args.find ("name") == args.end ())
        return;
    
    std::string fileName = args["name"];
    
    writeBuffer = m_fileManager->lookup (fileName);

    std::string type = "text/html";
    if (args.find ("type") != args.end ())
    {
        type = args["type"];    
    }
    
    this->sendBinary (outputDevice, writeBuffer, type);
}

void
IgWebService::getPic (Arguments *arguments)
{
    QIODevice *outputDevice = arguments->outputDevice ();
    ArgumentsMap &args = *(arguments->args ());
    QByteArray writeBuffer;
    
    if (args.find ("name") == args.end ())
        return;
    
    std::string fileName = args["name"];
    
    writeBuffer = m_fileManager->lookup (fileName);

    std::string type = "png";
    if (args.find ("type") != args.end ())
    {
        type = args["type"];    
    }
    
    this->sendBinary (outputDevice, writeBuffer, "image/" + type);
}

IgWebService::WebServiceCallback
IgWebService::callback (const std::string &label)
{
    return m_callbackMap[label];    
}


void
IgWebService::registerCallback (const std::string &label, 
				const IgWebService::WebServiceCallback &cb)
{
    m_callbackMap.insert (std::pair<std::string, WebServiceCallback > (label, cb));    
}


IgWebService::Arguments::Arguments (IgState *state, 
				    QIODevice *outputDevice, 
				    ArgumentsMap *args,
				    const std::string idCookie,
				    QByteArray bodyData)
    : m_state (state),
      m_outputDevice (outputDevice),
      m_args (args),
      m_idCookie (idCookie),
      m_bodyData (bodyData)
{
}

void
IgWebService::sendDone (QIODevice *socket)
{
    QTextStream os (socket);
    os << "HTTP/1.1 200 Ok\r\n"
        "content-type: text/xml; charset=UTF-8\r\n"
        "\r\n"
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<done/>\r\n";  
}


void
IgWebService::sendError (QIODevice *socket)
{
    QTextStream os (socket);
    os << "HTTP/1.1 200 Ok\r\n"
        "content-type: text/xml; charset=UTF-8\r\n"
        "\r\n"
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<error/>\r\n";  
}


void
IgWebService::sendXML (QIODevice *socket, std::string xmlString)
{
    QTextStream os (socket);
    os << "HTTP/1.1 200 Ok\r\n"
        "content-type: text/xml; charset=UTF-8\r\n"
        "\r\n"
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "" + xmlString + "\r\n";      
}

void
IgWebService::sendBinary (QIODevice *socket, 
			  QByteArray &tmpBuffer, 
			  std::string mimeType)
{
    QTextStream os (socket);
    os << "HTTP/1.0 200 Ok\r\n"
	"Content-Type: " + mimeType + ";\r\n"
	"\r\n";
    
    socket->writeBlock (tmpBuffer.data (), 
			tmpBuffer.size ());
		
    os << "\r\n";
}


IgState *
IgWebService::Arguments::state (void) 
{ return m_state; }

QIODevice *
IgWebService::Arguments::outputDevice (void) 
{ return m_outputDevice; }

IgWebService::ArgumentsMap *
IgWebService::Arguments::args (void) 
{ return m_args; }

std::string
IgWebService::Arguments::idCookie (void) 
{ return m_idCookie; }

QByteArray
IgWebService::Arguments::bodyData (void)
{
    return m_bodyData;    
}
