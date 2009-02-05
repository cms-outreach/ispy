//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgFileProxy/interface/IgFileFetcher.h"
# include "Ig_Modules/IgFileProxy/interface/IgFileProxyDB.h"
# include "Ig_Modules/IgFileProxy/interface/IgFileProxy.h"
# include <qurl.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgFileFetcher, "FIXME/File Fetcher");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgFileFetcher::IgFileFetcher (IgState *state)
    : m_state (state)
{ ASSERT (m_state); m_state->put (s_key, this); }

IgFileFetcher::~IgFileFetcher (void)
{
    for (ProxyTable::iterator i = m_proxies.begin (); i != m_proxies.end (); ++i)
	 delete i->second;

    m_state->detach (s_key);
}

IgFileProxy *
IgFileFetcher::getProxy (const std::string& protocol)
{
    IgFileProxy *&proxy = m_proxies [protocol];
    if (! proxy)
    	proxy = IgFileProxyDB::get ()->create (protocol);

    return proxy;
}

std::string
IgFileFetcher::fetch (const std::string &file)
{ 
    QUrl url(file);
    QString protocol (url.protocol ());
    IgFileProxy *proxy = getProxy (protocol);

    ASSERT (proxy); // FIXME: throw exception if non-existent
    return proxy->fetch (file);
}
