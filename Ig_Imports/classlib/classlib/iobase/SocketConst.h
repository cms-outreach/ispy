#ifndef CLASSLIB_SOCKET_CONST_H
# define CLASSLIB_SOCKET_CONST_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

namespace lat {
namespace SocketConst {

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

/// Maximum queue length for listen().
CLASSLIB_API extern const int MaxListen;		// SOMAXCONN

/// Shutdown close modes
CLASSLIB_API extern const int CloseDisallowReceive;
CLASSLIB_API extern const int CloseDisallowSend;
CLASSLIB_API extern const int CloseDisallowAll;

/// Socket types
CLASSLIB_API extern const int TypeStream;		// SOCK_STREAM
CLASSLIB_API extern const int TypeDatagram;		// SOCK_DGRAM
CLASSLIB_API extern const int TypeRaw;			// SOCK_RAW
// CLASSLIB_API extern const int TypeRDM;		// SOCK_RDM
// CLASSLIB_API extern const int TypeSeqPacket;	// SOCK_SEQPACKET

/// Socket options: the protocol level at which an option applies
CLASSLIB_API extern const int OptLevelSocket;		// Socket level
CLASSLIB_API extern const int OptLevelTCP;		// TCP level
CLASSLIB_API extern const int OptLevelIP;		// IP level

/// Options for OptLevelSocket
CLASSLIB_API extern const int OptSockDebug;		// SO_DEBUG		int boolean
CLASSLIB_API extern const int OptSockListening;	// SO_ACCEPTCONN	int boolean
CLASSLIB_API extern const int OptSockReuseAddress;	// SO_REUSEADDR		int boolean
CLASSLIB_API extern const int OptSockKeepAlive;	// SO_KEEPALIVE		int boolean
CLASSLIB_API extern const int OptSockDontRoute;	// SO_DONTROUTE		int boolean
CLASSLIB_API extern const int OptSockBroadcast;	// SO_BROADCAST		int boolean
CLASSLIB_API extern const int OptSockUseLoopback;	// SO_USELOOPBACK	int boolean
CLASSLIB_API extern const int OptSockLinger;		// SO_LINGER		struct linger *
CLASSLIB_API extern const int OptSockOOBInline;	// SO_OOBINLINE		int boolean
CLASSLIB_API extern const int OptSockReusePort;	// SO_REUSEPORT		int boolean
CLASSLIB_API extern const int OptSockSendBuffer;	// SO_SNDBUF		int (# of bytes)
CLASSLIB_API extern const int OptSockReceiveBuffer;	// SO_RCVBUF		int (# of bytes)
CLASSLIB_API extern const int OptSockSendLowater;	// SO_SNDLOWAT		int (# of bytes)
CLASSLIB_API extern const int OptSockReceiveLowater;	// SO_RCVLOWAT		int (# of bytes)
CLASSLIB_API extern const int OptSockSendTimeout;	// SO_SNDTIMEO		struct timeval *
CLASSLIB_API extern const int OptSockReceiveTimeout;	// SO_RCVTIMEO		struct timeval *
CLASSLIB_API extern const int OptSockError;		// SO_ERROR		int
CLASSLIB_API extern const int OptSockType;		// SO_TYPE		int (TypeXyz)
CLASSLIB_API extern const int OptSockPassCreds;	// SO_PASSCRED		int boolean
CLASSLIB_API extern const int OptSockPeerCreds;	// SO_PEERCRED		struct ucred *
CLASSLIB_API extern const int OptSockBindToDevice;	// SO_BINDTODEVICE	char * ([IFNAMESIZ])
CLASSLIB_API extern const int OptSockPriority;		// SO_PRIORITY		int
// CLASSLIB_API extern const int OptSockBSDCompat;	// SO_BSDCOMPAT		int boolean

/// Options for OptLevelTCP
CLASSLIB_API extern const int OptTCPNoDelay;		// TCP_NODELAY		int boolean
CLASSLIB_API extern const int OptTCPMaxSegmentSize;	// TCP_MAXSEG		int
CLASSLIB_API extern const int OptTCPCork;		// TCP_CORK		int boolean

/// Options for OptLevelIP
// CLASSLIB_API extern const int OptIPOptions;		// IP_OPTIONS		RFC 791
// CLASSLIB_API extern const int OptIPPacketInfo;	// IP_PKTINFO
// CLASSLIB_API extern const int OptIPReceiveTOS;	// IP_RECVTOS
// CLASSLIB_API extern const int OptIPReceiveTTL;	// IP_RECVTTL
// CLASSLIB_API extern const int OptIPReceiveOpts;	// IP_RECVOPTS
// CLASSLIB_API extern const int OptIPReceiveRawOpts;	// IP_RETOPTS
// CLASSLIB_API extern const int OptIPTOS;		// IP_TOS
// CLASSLIB_API extern const int OptIPTTL;		// IP_TTL
// CLASSLIB_API extern const int OptIPHeaderIncluded;	// IP_HDRINCL
// CLASSLIB_API extern const int OptIPReceiveErrors;	// IP_RECVERR
// CLASSLIB_API extern const int OptIPPMTUDiscovery;	// IP_PMTU_DISCOVER
// CLASSLIB_API extern const int OptIPMTU;		// IP_MTU
// CLASSLIB_API extern const int OptIPRouterAlert;	// IP_ROUTER_ALERT
// CLASSLIB_API extern const int OptIPMulticastTTL;	// IP_MULTICAST_TTL	unsigned char
// CLASSLIB_API extern const int OptIPMulticastLoop;	// IP_MULTICAST_LOOP	unsigned char
// CLASSLIB_API extern const int OptIPAddMembership;	// IP_ADD_MEMBERSHIP	struct ip_mreq *
// CLASSLIB_API extern const int OptIPDropMembership;	// IP_DROP_MEMBERSHIP	struct ip_mreq *
// CLASSLIB_API extern const int OptIPMulticastInterface; // IP_MULTICAST_IF	struct in_addr *

/// Message send/receive options
CLASSLIB_API extern const int MsgOutOfBand;		// MSG_OOB
CLASSLIB_API extern const int MsgPeek;			// MSG_PEEK
CLASSLIB_API extern const int MsgWaitAll;		// MSG_WAITALL
// CLASSLIB_API extern const int MsgNoSignal;		// MSG_NOSIGNAL
// CLASSLIB_API extern const int MsgTruncate;		// MSG_TRUNC
// CLASSLIB_API extern const int MsgErrorQueue;	// MSG_ERRQUEUE
// CLASSLIB_API extern const int MsgNoWait;		// MSG_DONTWAIT

// CLASSLIB_API extern const int MsgRecord;		// MSG_EOR (output)
// CLASSLIB_API extern const int MsgTruncated;		// MSG_TRUNC (output)
// CLASSLIB_API extern const int MsgControlTruncated;	// MSG_CTRUNC (output)
//							// MSG_OOB (output)
//							// MSG_ERRQUEUE (output)
//							// MSG_DONTWAIT (output)

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace SocketConst
} // namespace lat

#endif // CLASSLIB_SOCKET_CONST_H
