#ifndef IG_SOCKET_DRIVER_IG_SOCKET_H
# define IG_SOCKET_DRIVER_IG_SOCKET_H

//<<<<<< INCLUDES                                                       >>>>>>
#include "Ig_Modules/IgSocketDriver/interface/config.h"
#include <qsocket.h>
#include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_SOCKET_DRIVER_API IgSocket : public QSocket
{
    Q_OBJECT    
public:
    IgSocket (IgState *state);
    const std::string& userAgent (void) const;

public slots:
    void 		dump (void);

private:
    void		serverBusyMessage ();
    void		sendData (const std::string& data);
    IgState *		m_state;
    std::string		m_userAgent;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SOCKET_DRIVER_IG_SOCKET_H
