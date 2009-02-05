#ifndef IG_SOCKET_DRIVER_IG_WEB_SERVICE_H
# define IG_SOCKET_DRIVER_IG_WEB_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgSocketDriver/interface/config.h"
# include <map>
# include <string>
# include <classlib/utils/Callback.h>
# include <qcstring.h>
# include <qtextstream.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;
class QIODevice;
class IgFileManager;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_SOCKET_DRIVER_API IgWebService
{
public:
    typedef std::map<std::string, std::string> ArgumentsMap;

    class Arguments
    {
    public:	
	Arguments (IgState *state, 
		   QIODevice *socket, 
		   ArgumentsMap *args,
		   const std::string idCookie,
		   QByteArray bodyData);	
	IgState *	state (void);
	QIODevice *	outputDevice (void);
	ArgumentsMap *	args (void);
	std::string 	idCookie (void);
	QByteArray	bodyData (void);	
    private:
	IgState *	m_state;
	QIODevice *	m_outputDevice;
	ArgumentsMap *	m_args;	
	std::string 	m_idCookie;
	QByteArray	m_bodyData;	
    };
    
    typedef lat::Callback1<Arguments *> WebServiceCallback;

    IgWebService (IgState *state);
    virtual ~IgWebService (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    IgState*			state () const;
    IgFileManager*		getFileManager() const;
    void 			getScript (Arguments *arguments);
    void 			getPic (Arguments *arguments);    
    
    virtual WebServiceCallback callback (const std::string &label);
    
    
    static void sendDone (QIODevice *socket);
    static void sendError (QIODevice *socket);    
    static void sendXML (QIODevice *socket, std::string xmlString);
    static void sendBinary (QIODevice *socket, 
		            QByteArray &tmpBuffer, 
		            std::string mimeType);    
protected:
    void registerCallback (const std::string &label, 
			   const WebServiceCallback &cb);    
private:
    IgState*					m_state;
    IgFileManager*				m_fileManager;
    std::map <std::string, WebServiceCallback> 	m_callbackMap;    
    QByteArray					m_bodyData;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SOCKET_DRIVER_IG_WEB_SERVICE_H
