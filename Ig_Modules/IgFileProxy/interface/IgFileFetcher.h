#ifndef IG_FILE_PROXY_IG_FILE_FETCHER_H
# define IG_FILE_PROXY_IG_FILE_FETCHER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgFileProxy/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <string>
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgFileProxy;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_FILE_PROXY_API IgFileFetcher : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgFileFetcher);
public:
    IgFileFetcher (IgState *state);
    ~IgFileFetcher (void);
    // implicit copy constructor
    // implicit assignment operator

    std::string		fetch (const std::string &file);

private:
    typedef std::map<std::string, IgFileProxy *> ProxyTable;
    IgFileProxy *	getProxy (const std::string& protocol);
    
    IgState		*m_state;
    ProxyTable		m_proxies;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_FILE_PROXY_IG_FILE_FETCHER_H
