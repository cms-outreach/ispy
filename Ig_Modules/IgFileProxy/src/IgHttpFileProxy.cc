//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgFileProxy/interface/IgHttpFileProxy.h"
# include <classlib/iobase/Filename.h>
# include <qurloperator.h>
# include <qnetworkprotocol.h>
# include <qnetwork.h>
# include <sys/types.h>
# include <unistd.h>
# include <qapplication.h> 
# include <qeventloop.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>

bool IgHttpFileProxy::ProtocolInit = false;

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

const char *
IgHttpFileProxy::catalogLabel (void)
{ return "http"; }

//////////////////////////////////////////////////////////////////////
IgHttpFileProxy::IgHttpFileProxy ()
  : m_finished (false),
    m_copied (false)
{ 
    if (!ProtocolInit)
    { qInitNetworkProtocols (); ProtocolInit = true;}
    m_client = new QUrlOperator ();
    connect (m_client, SIGNAL (finished (QNetworkOperation*)),
             this,     SLOT   (finished (QNetworkOperation*)));
}

IgHttpFileProxy::~IgHttpFileProxy ()
{ 
    disconnect (m_client, SIGNAL (finished (QNetworkOperation*)),
                this,     SLOT (finished (QNetworkOperation*)));
    delete m_client;
}

std::string
IgHttpFileProxy::fetch (const std::string& file)
{
    static int num = 0;
    if (m_remote2local.find (file) != m_remote2local.end ())
    {
      std::string lfile = m_remote2local[file];
      if (lat::Filename (lfile).exists ())
	return lfile;
      else
        m_remote2local.erase (file);
    }
    
    QString lfile ("/tmp/iguana_http_");
    lfile += QString::number (num++) + "_" + QString::number (getpid ());

    QEventLoop *eloop= QApplication::eventLoop ();
    m_finished = false;
    
    m_client->copy (QString(file.c_str ()),lfile, false, false);

    while (!m_finished)
      eloop->processEvents (QEventLoop::AllEvents);
    
    if (!m_copied) return "";
    
    FileTable::iterator it;
    for (it = m_remote2local.begin (); it != m_remote2local.end(); ++it)
    {
      if (it->second == lfile.latin1 ())
      {
        m_remote2local.erase (it->first);
	break;
      }
    }
    m_remote2local[file] = lfile.latin1 ();
    return lfile.latin1 ();
}

void
IgHttpFileProxy::finished (QNetworkOperation * op)
{  
  if (op->operation () == QNetworkProtocol::OpPut)
  {
    m_finished = true;
    if (op->state () == QNetworkProtocol::StDone)
      m_copied = true;
    else
      m_copied = false;
  }
}
