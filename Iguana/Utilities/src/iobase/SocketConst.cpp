//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/SocketConst.h"
#include "classlib/sysapi/Socket.h"
#include <climits>

namespace lat {
namespace SocketConst {

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define NONEXISTENT	INT_MIN

#ifndef SO_ACCEPTCONN
# define SO_ACCEPTCONN	NONEXISTENT
#endif
#ifndef SO_USELOOPBACK
# define SO_USELOOPBACK	NONEXISTENT
#endif
#ifndef SO_REUSEPORT
# define SO_REUSEPORT	NONEXISTENT
#endif
#ifndef SO_SNDLOWAT
# define SO_SNDLOWAT	NONEXISTENT
# define SO_RCVLOWAT	NONEXISTENT
#endif
#ifndef SO_SNDTIMEO
# define SO_SNDTIMEO	NONEXISTENT
# define SO_RCVTIMEO	NONEXISTENT
#endif
#ifndef SO_PASSCRED
# define SO_PASSCRED	NONEXISTENT
# define SO_PEERCRED	NONEXISTENT
#endif
#ifndef SO_BINDTODEVICE
# define SO_BINDTODEVICE NONEXISTENT
#endif
#ifndef SO_PRIORITY
# define SO_PRIORITY	NONEXISTENT
#endif

#ifndef TCP_NODELAY
# define TCP_NODELAY	NONEXISTENT
#endif
#ifndef TCP_MAXSEG
# define TCP_MAXSEG	NONEXISTENT
#endif
#ifndef TCP_CORK
# define TCP_CORK	NONEXISTENT
#endif

#ifndef SOL_TCP
# define SOL_TCP	6  // FIXME: getprotobyname ("tcp")->p_proto?
#endif
#ifndef SOL_IP
# define SOL_IP		0  // FIXME: getprotobyname ("ip")->p_proto?
#endif

#ifndef MSG_WAITALL
# define MSG_WAITALL 0
#endif
#ifndef MSG_NOSIGNAL
# define MSG_NOSIGNAL 0
#endif
#ifndef MSG_TRUNC
# define MSG_TRUNC 0
#endif
#ifndef MSG_ERRQUEUE
# define MSG_ERRQUEUE 0
#endif
#ifndef MSG_DONTWAIT
# define MSG_DONTWAIT 0
#endif

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

//////////////////////////////////////////////////////////////////////
const int MaxListen		= SOMAXCONN;

//////////////////////////////////////////////////////////////////////
const int CloseDisallowReceive	= 0;
const int CloseDisallowSend	= 1;
const int CloseDisallowAll	= 2;
    
//////////////////////////////////////////////////////////////////////
const int TypeStream		= SOCK_STREAM;
const int TypeDatagram		= SOCK_DGRAM;
const int TypeRaw		= SOCK_RAW;
const int TypeRDM		= SOCK_RDM;
const int TypeSeqPacket		= SOCK_SEQPACKET;

//////////////////////////////////////////////////////////////////////
const int OptLevelSocket	= SOL_SOCKET;
const int OptLevelTCP		= SOL_TCP;
const int OptLevelIP		= SOL_IP;

//////////////////////////////////////////////////////////////////////
const int OptSockDebug		= SO_DEBUG;
const int OptSockListening	= SO_ACCEPTCONN;
const int OptSockReuseAddress	= SO_REUSEADDR;
const int OptSockKeepAlive	= SO_KEEPALIVE;
const int OptSockDontRoute	= SO_DONTROUTE;
const int OptSockBroadcast	= SO_BROADCAST;
const int OptSockUseLoopback	= SO_USELOOPBACK;
const int OptSockLinger		= SO_LINGER;
const int OptSockOOBInline	= SO_OOBINLINE;
const int OptSockReusePort	= SO_REUSEPORT;
const int OptSockSendBuffer	= SO_SNDBUF;
const int OptSockReceiveBuffer	= SO_RCVBUF;
const int OptSockSendLowater	= SO_SNDLOWAT;
const int OptSockReceiveLowater	= SO_RCVLOWAT;
const int OptSockSendTimeout	= SO_SNDTIMEO;
const int OptSockReceiveTimeout	= SO_RCVTIMEO;
const int OptSockError		= SO_ERROR;
const int OptSockType		= SO_TYPE;
const int OptSockPassCreds	= SO_PASSCRED;
const int OptSockPeerCreds	= SO_PEERCRED;
const int OptSockBindToDevice	= SO_BINDTODEVICE;
const int OptSockPriority	= SO_PRIORITY;

const int OptTCPNoDelay		= TCP_NODELAY;
const int OptTCPMaxSegmentSize  = TCP_MAXSEG;
const int OptTCPCork		= TCP_CORK;

// const int OptIPOptions	= IP_OPTIONS;
// const int OptIPPacketInfo	= IP_PKTINFO;
// const int OptIPReceiveTOS	= IP_RECVTOS;
// const int OptIPReceiveTTL	= IP_RECVTTL;
// const int OptIPReceiveOpts	= IP_RECVOPTS;
// const int OptIPReceiveRawOpts= IP_RETOPTS;
// const int OptIPTOS		= IP_TOS;
// const int OptIPTTL		= IP_TTL;
// const int OptIPHeaderIncluded= IP_HDRINCL;
// const int OptIPReceiveErrors	= IP_RECVERR;
// const int OptIPPMTUDiscovery	= IP_PMTU_DISCOVER;
// const int OptIPMTU		= IP_MTU;
// const int OptIPRouterAlert	= IP_ROUTER_ALERT;
// const int OptIPMulticastTTL	= IP_MULTICAST_TTL;
// const int OptIPMulticastLoop	= IP_MULTICAST_LOOP;
// const int OptIPAddMembership	= IP_ADD_MEMBERSHIP;
// const int OptIPDropMembership = IP_DROP_MEMBERSHIP;
// const int OptIPMulticastInterface = IP_MULTICAST_IF;

//////////////////////////////////////////////////////////////////////
const int MsgOutOfBand		= MSG_OOB;
const int MsgPeek		= MSG_PEEK;
const int MsgWaitAll		= MSG_WAITALL;
const int MsgNoSignal		= MSG_NOSIGNAL;
const int MsgTruncate		= MSG_TRUNC;
const int MsgErrorQueue		= MSG_ERRQUEUE;
const int MsgNoWait		= MSG_DONTWAIT;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>
    
} // namespace SocketConst
} // namespace lat
