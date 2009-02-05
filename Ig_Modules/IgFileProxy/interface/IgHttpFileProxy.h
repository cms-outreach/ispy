#ifndef IG_FILE_PROXY_IG_HTTP_FILE_PROXY_H
# define IG_FILE_PROXY_IG_HTTP_FILE_PROXY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgFileProxy/interface/IgFileProxy.h"
# include "Ig_Modules/IgFileProxy/interface/config.h"
# include <map>
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QUrlOperator;
class QNetworkOperation;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_FILE_PROXY_API IgHttpFileProxy : public QObject, public IgFileProxy
{
    Q_OBJECT
public:
    IgHttpFileProxy ();
    ~IgHttpFileProxy ();
    
    virtual std::string fetch (const std::string &file);

    static const char *	catalogLabel (void);

public slots:
    void finished (QNetworkOperation *);


private:    
    typedef std::map<std::string, std::string> FileTable;
    
    static bool		ProtocolInit;
    QUrlOperator	*m_client;
    bool		m_finished;
    bool		m_copied;
    FileTable 		m_remote2local;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_FILE_PROXY_IG_HTTP_FILE_PROXY_H
