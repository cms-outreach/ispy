//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/SystemError.h"
#include "classlib/utils/StringFormat.h"
#include "classlib/sysapi/SystemError.h"
#include <cstring>

namespace lat {
using namespace SysErr;
    
//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define SYSERR(sys,port)  { port, sys }
#ifndef _WIN32
# ifndef EPERM
#  define EPERM 0
# endif
# ifndef ENOENT
#  define ENOENT 0
# endif
# ifndef ESRCH
#  define ESRCH 0
# endif
# ifndef EINTR
#  define EINTR 0
# endif
# ifndef EIO
#  define EIO 0
# endif
# ifndef ENXIO
#  define ENXIO 0
# endif
# ifndef E2BIG
#  define E2BIG 0
# endif
# ifndef ENOEXEC
#  define ENOEXEC 0
# endif
# ifndef EBADF
#  define EBADF 0
# endif
# ifndef ECHILD
#  define ECHILD 0
# endif
# ifndef EAGAIN
#  define EAGAIN 0
# endif
# ifndef EWOULDBLOCK
#  define EWOULDBLOCK 0
# endif
# ifndef ENOMEM
#  define ENOMEM 0
# endif
# ifndef EACCES
#  define EACCES 0
# endif
# ifndef EFAULT
#  define EFAULT 0
# endif
# ifndef ENOTBLK
#  define ENOTBLK 0
# endif
# ifndef EBUSY
#  define EBUSY 0
# endif
# ifndef EEXIST
#  define EEXIST 0
# endif
# ifndef ENOTEMPTY
#  define ENOTEMPTY 0
# endif
# ifndef EXDEV
#  define EXDEV 0
# endif
# ifndef ENODEV
#  define ENODEV 0
# endif
# ifndef ENOTDIR
#  define ENOTDIR 0
# endif
# ifndef EISDIR
#  define EISDIR 0
# endif
# ifndef EINVAL
#  define EINVAL 0
# endif
# ifndef ENFILE
#  define ENFILE 0
# endif
# ifndef EMFILE
#  define EMFILE 0
# endif
# ifndef ENOTTY
#  define ENOTTY 0
# endif
# ifndef ETXTBSY
#  define ETXTBSY 0
# endif
# ifndef EFBIG
#  define EFBIG 0
# endif
# ifndef ENOSPC
#  define ENOSPC 0
# endif
# ifndef ESPIPE
#  define ESPIPE 0
# endif
# ifndef EROFS
#  define EROFS 0
# endif
# ifndef EMLINK
#  define EMLINK 0
# endif
# ifndef EPIPE
#  define EPIPE 0
# endif
# ifndef EDOM
#  define EDOM 0
# endif
# ifndef ERANGE
#  define ERANGE 0
# endif
# ifndef EDEADLK
#  define EDEADLK 0
# endif
# ifndef EDEADLOCK
#  define EDEADLOCK 0
# endif
# ifndef ENAMETOOLONG
#  define ENAMETOOLONG 0
# endif
# ifndef ENOLCK
#  define ENOLCK 0
# endif
# ifndef ENOSYS
#  define ENOSYS 0
# endif
# ifndef ELOOP
#  define ELOOP 0
# endif
# ifndef ENOMSG
#  define ENOMSG 0
# endif
# ifndef EIDRM
#  define EIDRM 0
# endif
# ifndef ECHRNG
#  define ECHRNG 0
# endif
# ifndef EL2NSYNC
#  define EL2NSYNC 0
# endif
# ifndef EL3HLT
#  define EL3HLT 0
# endif
# ifndef EL3RST
#  define EL3RST 0
# endif
# ifndef ELNRNG
#  define ELNRNG 0
# endif
# ifndef EUNATCH
#  define EUNATCH 0
# endif
# ifndef ENOCSI
#  define ENOCSI 0
# endif
# ifndef EL2HLT
#  define EL2HLT 0
# endif
# ifndef EBADE
#  define EBADE 0
# endif
# ifndef EBADR
#  define EBADR 0
# endif
# ifndef EXFULL
#  define EXFULL 0
# endif
# ifndef ENOANO
#  define ENOANO 0
# endif
# ifndef EBADRQC
#  define EBADRQC 0
# endif
# ifndef EBADSLT
#  define EBADSLT 0
# endif
# ifndef EBFONT
#  define EBFONT 0
# endif
# ifndef ENOSTR
#  define ENOSTR 0
# endif
# ifndef ENODATA
#  define ENODATA 0
# endif
# ifndef ETIME
#  define ETIME 0
# endif
# ifndef ENOSR
#  define ENOSR 0
# endif
# ifndef ENONET
#  define ENONET 0
# endif
# ifndef ENOPKG
#  define ENOPKG 0
# endif
# ifndef EREMOTE
#  define EREMOTE 0
# endif
# ifndef ENOLINK
#  define ENOLINK 0
# endif
# ifndef EADV
#  define EADV 0
# endif
# ifndef ESRMNT
#  define ESRMNT 0
# endif
# ifndef ECOMM
#  define ECOMM 0
# endif
# ifndef EPROTO
#  define EPROTO 0
# endif
# ifndef EMULTIHOP
#  define EMULTIHOP 0
# endif
# ifndef EDOTDOT
#  define EDOTDOT 0
# endif
# ifndef EBADMSG
#  define EBADMSG 0
# endif
# ifndef EOVERFLOW
#  define EOVERFLOW 0
# endif
# ifndef ENOTUNIQ
#  define ENOTUNIQ 0
# endif
# ifndef EBADFD
#  define EBADFD 0
# endif
# ifndef EREMCHG
#  define EREMCHG 0
# endif
# ifndef ELIBACC
#  define ELIBACC 0
# endif
# ifndef ELIBBAD
#  define ELIBBAD 0
# endif
# ifndef ELIBSCN
#  define ELIBSCN 0
# endif
# ifndef ELIBMAX
#  define ELIBMAX 0
# endif
# ifndef ELIBEXEC
#  define ELIBEXEC 0
# endif
# ifndef EILSEQ
#  define EILSEQ 0
# endif
# ifndef ERESTART
#  define ERESTART 0
# endif
# ifndef ESTRPIPE
#  define ESTRPIPE 0
# endif
# ifndef EUSERS
#  define EUSERS 0
# endif
# ifndef ENOTSOCK
#  define ENOTSOCK 0
# endif
# ifndef EDESTADDRREQ
#  define EDESTADDRREQ 0
# endif
# ifndef EMSGSIZE
#  define EMSGSIZE 0
# endif
# ifndef EPROTOTYPE
#  define EPROTOTYPE 0
# endif
# ifndef ENOPROTOOPT
#  define ENOPROTOOPT 0
# endif
# ifndef EPROTONOSUPPORT
#  define EPROTONOSUPPORT 0
# endif
# ifndef ESOCKTNOSUPPORT
#  define ESOCKTNOSUPPORT 0
# endif
# ifndef EPFNOSUPPORT
#  define EPFNOSUPPORT 0
# endif
# ifndef EAFNOSUPPORT
#  define EAFNOSUPPORT 0
# endif
# ifndef EADDRINUSE
#  define EADDRINUSE 0
# endif
# ifndef EADDRNOTAVAIL
#  define EADDRNOTAVAIL 0
# endif
# ifndef ENETDOWN
#  define ENETDOWN 0
# endif
# ifndef ENETUNREACH
#  define ENETUNREACH 0
# endif
# ifndef ENETRESET
#  define ENETRESET 0
# endif
# ifndef ECONNABORTED
#  define ECONNABORTED 0
# endif
# ifndef ECONNRESET
#  define ECONNRESET 0
# endif
# ifndef ENOBUFS
#  define ENOBUFS 0
# endif
# ifndef EISCONN
#  define EISCONN 0
# endif
# ifndef ENOTCONN
#  define ENOTCONN 0
# endif
# ifndef ESHUTDOWN
#  define ESHUTDOWN 0
# endif
# ifndef ETOOMANYREFS
#  define ETOOMANYREFS 0
# endif
# ifndef ETIMEDOUT
#  define ETIMEDOUT 0
# endif
# ifndef ECONNREFUSED
#  define ECONNREFUSED 0
# endif
# ifndef EHOSTDOWN
#  define EHOSTDOWN 0
# endif
# ifndef EHOSTUNREACH
#  define EHOSTUNREACH 0
# endif
# ifndef EALREADY
#  define EALREADY 0
# endif
# ifndef EINPROGRESS
#  define EINPROGRESS 0
# endif
# ifndef ESTALE
#  define ESTALE 0
# endif
# ifndef EUCLEAN
#  define EUCLEAN 0
# endif
# ifndef ENOTNAM
#  define ENOTNAM 0
# endif
# ifndef ENAVAIL
#  define ENAVAIL 0
# endif
# ifndef EISNAM
#  define EISNAM 0
# endif
# ifndef EREMOTEIO
#  define EREMOTEIO 0
# endif
# ifndef EDQUOT
#  define EDQUOT 0
# endif
# ifndef ENOMEDIUM
#  define ENOMEDIUM 0
# endif
# ifndef EMEDIUMTYPE
#  define EMEDIUMTYPE 0
# endif
# ifndef ENOTSUP
#  define ENOTSUP 0
# endif
# ifndef EOPNOTSUPP
#  define EOPNOTSUPP 0
# endif
# ifndef ECANCELED
#  define ECANCELED 0
# endif
# ifndef EOWNERDEAD
#  define EOWNERDEAD 0
# endif
# ifndef ENOTRECOVERABLE
#  define ENOTRECOVERABLE 0
# endif
# ifndef ELOCKUNMAPPED
#  define ELOCKUNMAPPED 0
# endif
# ifndef ENOSYM
#  define ENOSYM 0
# endif
# ifndef EREMOTERELEASE
#  define EREMOTERELEASE 0
# endif
# ifndef ENOSHARE
#  define ENOSHARE 0
# endif
# ifndef EPROCLIM
#  define EPROCLIM 0
# endif
# ifndef EDQUOT
#  define EDQUOT 0
# endif
#endif // !_WIN32

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

SysErr::Code
SystemError::translateFromErrno (int syserr)
{
    static const struct Mapping { SysErr::Code portable; int syserr; } mappings [] = {
#ifndef _WIN32
	SYSERR (EPERM,				ErrOperationNotPermitted),
	SYSERR (ENOENT,				ErrNoSuchEntity),
	SYSERR (ESRCH,				ErrNoSuchProcess),
	SYSERR (EINTR,				ErrInterruptedSystemCall),
	SYSERR (EIO,				ErrIO),
	SYSERR (ENXIO,				ErrNoSuchDeviceOrAddress),
	SYSERR (E2BIG,				ErrArgListTooLong),
	SYSERR (ENOEXEC,			ErrExecFormat),
	SYSERR (EBADF,				ErrBadFileNumber),
	SYSERR (ECHILD,				ErrNoChildProcesses),
	SYSERR (EAGAIN,				ErrTryAgain),
	SYSERR (EWOULDBLOCK,			ErrTryAgain),
	SYSERR (ENOMEM,				ErrOutOfMemory),
	SYSERR (EACCES,				ErrPermissionDenied),
	SYSERR (EFAULT,				ErrBadAddress),
	SYSERR (ENOTBLK,			ErrBlockDeviceRequired),
	SYSERR (EBUSY,				ErrDeviceOrResourceBusy),
	SYSERR (EEXIST,				ErrFileExists),
	SYSERR (ENOTEMPTY,			ErrFileExists),
	SYSERR (EXDEV,				ErrCrossDeviceLink),
	SYSERR (ENODEV,				ErrNoSuchDevice),
	SYSERR (ENOTDIR,			ErrNotDirectory),
	SYSERR (EISDIR,				ErrIsDirectory),
	SYSERR (EINVAL,				ErrInvalidArgument),
	SYSERR (ENFILE,				ErrFileTableOverflow),
	SYSERR (EMFILE,				ErrTooManyOpenFiles),
	SYSERR (ENOTTY,				ErrNotTTY),
	SYSERR (ETXTBSY,			ErrTextFileBusy),
	SYSERR (EFBIG,				ErrFileTooLarge),
	SYSERR (ENOSPC,				ErrNoSpaceLeftOnDevice),
	SYSERR (ESPIPE,				ErrIllegalSeek),
	SYSERR (EROFS,				ErrReadOnlyFileSystem),
	SYSERR (EMLINK,				ErrTooManyLinks),
	SYSERR (EPIPE,				ErrBrokenPipe),
	SYSERR (EDOM,				ErrMathArgumentOutOfDomain),
	SYSERR (ERANGE,				ErrMathResultOutOfRange),
	SYSERR (EDEADLK,			ErrResourceDeadlockWouldOccur),
	SYSERR (EDEADLOCK,			ErrResourceDeadlockWouldOccur),
	SYSERR (ENAMETOOLONG,			ErrFileNameTooLong),
	SYSERR (ENOLCK,				ErrNoLocksAvailable),
	SYSERR (ENOSYS,				ErrFunctionNotImplemented),
	SYSERR (ELOOP,				ErrTooManySymbolicLinks),
	SYSERR (ENOMSG,				ErrNoRequestedMessage),
	SYSERR (EIDRM,				ErrIdentifierRemoved),
	SYSERR (ECHRNG,				ErrChannelOutOfRange),
	SYSERR (EL2NSYNC,			ErrLevel2NotSynchronised),
	SYSERR (EL3HLT,				ErrLevel3Halted),
	SYSERR (EL3RST,				ErrLevel3Reset),
	SYSERR (ELNRNG,				ErrLinkNumberOutOfRange),
	SYSERR (EUNATCH,			ErrProtocolDriverNotAtached),
	SYSERR (ENOCSI,				ErrNoCSIStructure),
	SYSERR (EL2HLT,				ErrLevel2Halted),
	SYSERR (EBADE,				ErrInvalidExchange),
	SYSERR (EBADR,				ErrInvalidRequestDescriptor),
	SYSERR (EXFULL,				ErrExchangeFull),
	SYSERR (ENOANO,				ErrNoANode),
	SYSERR (EBADRQC,			ErrInvalidRequestCode),
	SYSERR (EBADSLT,			ErrInvalidSlot),
	SYSERR (EBFONT,				ErrBadFontFileFormat),
	SYSERR (ENOSTR,				ErrDeviceNotStream),
	SYSERR (ENODATA,			ErrNoDataAvailable),
	SYSERR (ETIME,				ErrTimerExpired),
	SYSERR (ENOSR,				ErrOutOfStreamsResources),
	SYSERR (ENONET,				ErrNoNetwork),
	SYSERR (ENOPKG,				ErrPackageNotInstalled),
	SYSERR (EREMOTE,			ErrObjectIsRemote),
	SYSERR (ENOLINK,			ErrSeveredLink),
	SYSERR (EADV,				ErrAdvertise),
	SYSERR (ESRMNT,				ErrSrmount),
	SYSERR (ECOMM,				ErrOnSendCommunication),
	SYSERR (EPROTO,				ErrProtocol),
	SYSERR (EMULTIHOP,			ErrMultihopAttempted),
	SYSERR (EDOTDOT,			ErrRFSSpecific),
	SYSERR (EBADMSG,			ErrNotDataMessage),
	SYSERR (EOVERFLOW,			ErrValueTooLarge),
	SYSERR (ENOTUNIQ,			ErrNameNotUniqueOnNetwork),
	SYSERR (EBADFD,				ErrBadFileDescriptorState),
	SYSERR (EREMCHG,			ErrRemoteAddressChanged),
	SYSERR (ELIBACC,			ErrInaccessibleSharedLibrary),
	SYSERR (ELIBBAD,			ErrCorruptedSharedLibrary),
	SYSERR (ELIBSCN,			ErrCorruptedLibrarySection),
	SYSERR (ELIBMAX,			ErrTooManySharedLibraries),
	SYSERR (ELIBEXEC,			ErrCannotExecuteSharedLibrary),
	SYSERR (EILSEQ,				ErrIllegalByteSequence),
	SYSERR (ERESTART,			ErrRestartSystemCall),
	SYSERR (ESTRPIPE,			ErrStreamsPipe),
	SYSERR (EUSERS,				ErrTooManyUsers),
	SYSERR (ENOTSOCK,			ErrNotSocket),
	SYSERR (EDESTADDRREQ,			ErrDestinationAddressRequired),
	SYSERR (EMSGSIZE,			ErrMessageTooLong),
	SYSERR (EPROTOTYPE,			ErrWrongProtocolType),
	SYSERR (ENOPROTOOPT,			ErrUnavailableProtocol),
	SYSERR (EPROTONOSUPPORT,		ErrUnsupportedProtocol),
	SYSERR (ESOCKTNOSUPPORT,		ErrUnsupportedSocketType),
	SYSERR (EPFNOSUPPORT,			ErrUnsupportedProtocolFamily),
	SYSERR (EAFNOSUPPORT,			ErrUnsupportedAdressFamily),
	SYSERR (EADDRINUSE,			ErrAddressInUse),
	SYSERR (EADDRNOTAVAIL,			ErrAddressNotAvailable),
	SYSERR (ENETDOWN,			ErrNetworkDown),
	SYSERR (ENETUNREACH,			ErrNetworkUnreachable),
	SYSERR (ENETRESET,			ErrNetworkReset),
	SYSERR (ECONNABORTED,			ErrConnectionAborted),
	SYSERR (ECONNRESET,			ErrConnectionResetByPeer),
	SYSERR (ENOBUFS,			ErrNoBuffersAvailable),
	SYSERR (EISCONN,			ErrAlreadyConnected),
	SYSERR (ENOTCONN,			ErrNotConnected),
	SYSERR (ESHUTDOWN,			ErrConnectionAlreadyShutDown),
	SYSERR (ETOOMANYREFS,			ErrTooManyReferences),
	SYSERR (ETIMEDOUT,			ErrConnectionTimedOut),
	SYSERR (ECONNREFUSED,			ErrConnectionRefused),
	SYSERR (EHOSTDOWN,			ErrHostDown),
	SYSERR (EHOSTUNREACH,			ErrNoRouteToHost),
	SYSERR (EALREADY,			ErrOperationAlreadyInProgress),
	SYSERR (EINPROGRESS,			ErrOperationInProgress),
	SYSERR (ESTALE,				ErrStaleNFSHandle),
	SYSERR (EUCLEAN,			ErrStructureNeedsCleaning),
	SYSERR (ENOTNAM,			ErrNotNamedTypeFile),
	SYSERR (ENAVAIL,			ErrNoSemaphoresAvailable),
	SYSERR (EISNAM,				ErrIsNamedTypeFile),
	SYSERR (EREMOTEIO,			ErrRemoteIO),
	SYSERR (EDQUOT,				ErrQuotaExceeded),
	SYSERR (ENOMEDIUM,			ErrNoMediumFound),
	SYSERR (EMEDIUMTYPE,			ErrWrongMediumType),
	SYSERR (ENOTSUP,			ErrOperationNotSupported),
	SYSERR (EOPNOTSUPP,			ErrOperationNotSupported),
	SYSERR (ECANCELED,			ErrOperationCanceled),
	SYSERR (EOWNERDEAD,			ErrProcessDiedWithLock),
	SYSERR (ENOTRECOVERABLE,		ErrLockNotRecovarable),
	SYSERR (ELOCKUNMAPPED,			ErrLockUnmapped),
	SYSERR (ENOSYM,				ErrNoSuchSymbolInExecutable),
	SYSERR (EREMOTERELEASE,			ErrRemotePeerReleasedConnection),
	SYSERR (ENOSHARE,			ErrNoSuchShare),
	SYSERR (EPROCLIM,			ErrProcessLimitExceeded),
	SYSERR (EDQUOT,				ErrDiskQuotaExceeded),
#else // _WIN32
	SYSERR (ERROR_NOT_OWNER,		ErrOperationNotPermitted),
	SYSERR (ERROR_FILE_NOT_FOUND,		ErrNoSuchEntity),
	SYSERR (ERROR_PATH_NOT_FOUND,		ErrNoSuchEntity),
	SYSERR (WSAEINTR,			ErrInterruptedSystemCall),
	SYSERR (ERROR_INVALID_AT_INTERRUPT_TIME, ErrInterruptedSystemCall),
	SYSERR (ERROR_NO_SIGNAL_SENT,		ErrIO),
	SYSERR (ERROR_IO_DEVICE,		ErrIO),
	SYSERR (ERROR_CRC,			ErrIO),
	SYSERR (ERROR_FILE_INVALID,		ErrNoSuchDeviceOrAddress),
	SYSERR (WSAEBADF,			ErrBadFileNumber),
	SYSERR (ERROR_INVALID_HANDLE,		ErrBadFileNumber),
	SYSERR (WSAEWOULDBLOCK,			ErrTryAgain),
	SYSERR (ERROR_MORE_DATA,		ErrTryAgain),
	SYSERR (ERROR_OPEN_FILES,		ErrTryAgain),
	SYSERR (ERROR_ACTIVE_CONNECTIONS,	ErrTryAgain),
	SYSERR (ERROR_DEVICE_IN_USE,		ErrTryAgain),
	SYSERR (ERROR_NOT_ENOUGH_MEMORY,	ErrOutOfMemory),
	SYSERR (WSAEACCES,			ErrPermissionDenied),
	SYSERR (ERROR_ACCESS_DENIED,		ErrPermissionDenied),
	SYSERR (ERROR_SHARING_VIOLATION,	ErrPermissionDenied),
	SYSERR (ERROR_LOCK_VIOLATION,		ErrPermissionDenied),
	SYSERR (WSAEFAULT,			ErrBadAddress),
	SYSERR (ERROR_PROCESS_ABORTED,		ErrBadAddress),
	SYSERR (ERROR_NOACCESS,			ErrBadAddress),
	SYSERR (ERROR_PIPE_BUSY,		ErrDeviceOrResourceBusy),
	SYSERR (ERROR_PIPE_CONNECTED,		ErrDeviceOrResourceBusy),
	SYSERR (WSAENOTEMPTY,			ErrFileExists),
	SYSERR (ERROR_ALREADY_EXISTS,		ErrFileExists),
	SYSERR (ERROR_NOT_SAME_DEVICE,		ErrCrossDeviceLink),
	SYSERR (ERROR_BAD_UNIT,			ErrNoSuchDevice),
	SYSERR (ERROR_BAD_DEVICE,		ErrNoSuchDevice),
	SYSERR (ERROR_DIRECTORY,		ErrNotDirectory),
	SYSERR (WSAEINVAL,			ErrInvalidArgument),
	SYSERR (ERROR_INVALID_DATA,		ErrInvalidArgument),
	SYSERR (ERROR_FILENAME_EXCED_RANGE,	ErrInvalidArgument),
	SYSERR (ERROR_META_EXPANSION_TOO_LONG,	ErrInvalidArgument),
	SYSERR (ERROR_INVALID_SIGNAL_NUMBER,	ErrInvalidArgument),
	SYSERR (ERROR_THREAD_1_INACTIVE,	ErrInvalidArgument),
	SYSERR (ERROR_BAD_PIPE,			ErrInvalidArgument),
	SYSERR (ERROR_NO_TOKEN,			ErrInvalidArgument),
	SYSERR (ERROR_BAD_USERNAME,		ErrInvalidArgument),
	SYSERR (ERROR_NEGATIVE_SEEK,		ErrInvalidArgument),
	SYSERR (ERROR_NO_MORE_FILES,		ErrFileTableOverflow), // FIXME: ENMFILE?
	SYSERR (WSAEMFILE,			ErrTooManyOpenFiles),
	SYSERR (ERROR_TOO_MANY_OPEN_FILES,	ErrTooManyOpenFiles),
	SYSERR (ERROR_HANDLE_DISK_FULL,		ErrNoSpaceLeftOnDevice),
	SYSERR (ERROR_END_OF_MEDIA,		ErrNoSpaceLeftOnDevice),
	SYSERR (ERROR_EOM_OVERFLOW,		ErrNoSpaceLeftOnDevice),
	SYSERR (ERROR_NO_DATA_DETECTED,		ErrNoSpaceLeftOnDevice),
	SYSERR (ERROR_DISK_FULL,		ErrNoSpaceLeftOnDevice),
	SYSERR (ERROR_BEGINNING_OF_MEDIA,	ErrIllegalSeek),
	SYSERR (ERROR_SETMARK_DETECTED,		ErrIllegalSeek),
	SYSERR (ERROR_WRITE_PROTECT,		ErrReadOnlyFileSystem),
	SYSERR (ERROR_POSSIBLE_DEADLOCK,	ErrResourceDeadlockWouldOccur),
	SYSERR (WSAENAMETOOLONG,		ErrFileNameTooLong),
	SYSERR (ERROR_SHARING_BUFFER_EXCEEDED,	ErrNoLocksAvailable),
	SYSERR (ERROR_NOT_SUPPORTED,		ErrFunctionNotImplemented),
	SYSERR (WSAELOOP,			ErrTooManySymbolicLinks),
	SYSERR (ERROR_INVALID_FUNCTION,		ErrInvalidRequestCode),
	SYSERR (ERROR_HANDLE_EOF,		ErrNoDataAvailable),
	SYSERR (ERROR_REM_NOT_LIST,		ErrNoNetwork),
	SYSERR (WSAEREMOTE,			ErrObjectIsRemote),
	SYSERR (ERROR_NOT_CONNECTED,		ErrSeveredLink),
	SYSERR (ERROR_PIPE_NOT_CONNECTED,	ErrOnSendCommunication),
	SYSERR (ERROR_PIPE_LISTENING,		ErrOnSendCommunication),
	SYSERR (ERROR_DUP_NAME,			ErrNameNotUniqueOnNetwork),
	SYSERR (WSAEUSERS,			ErrTooManyUsers),
	SYSERR (WSAENOTSOCK,			ErrNotSocket),
	SYSERR (WSAEDESTADDRREQ,		ErrDestinationAddressRequired),
	SYSERR (WSAEMSGSIZE,			ErrMessageTooLong),
	SYSERR (WSAEPROTOTYPE,			ErrWrongProtocolType),
	SYSERR (WSAENOPROTOOPT,			ErrUnavailableProtocol),
	SYSERR (WSAEPROTONOSUPPORT,		ErrUnsupportedProtocol),
	SYSERR (WSAESOCKTNOSUPPORT,		ErrUnsupportedSocketType),
	SYSERR (WSAEPFNOSUPPORT,		ErrUnsupportedProtocolFamily),
	SYSERR (WSAEAFNOSUPPORT,		ErrUnsupportedAdressFamily),
	SYSERR (WSAEADDRINUSE,			ErrAddressInUse),
	SYSERR (WSAEADDRNOTAVAIL,		ErrAddressNotAvailable),
	SYSERR (WSAENETDOWN,			ErrNetworkDown),
	SYSERR (WSAENETUNREACH,			ErrNetworkUnreachable),
	SYSERR (WSAENETRESET,			ErrNetworkReset),
	SYSERR (WSAECONNABORTED,		ErrConnectionAborted),
	SYSERR (WSAECONNRESET,			ErrConnectionResetByPeer),
	SYSERR (WSAENOBUFS,			ErrNoBuffersAvailable),
	SYSERR (WSAEISCONN,			ErrAlreadyConnected),
	SYSERR (WSAENOTCONN,			ErrNotConnected),
	SYSERR (WSAESHUTDOWN,			ErrConnectionAlreadyShutDown),
	SYSERR (WSAETOOMANYREFS,		ErrTooManyReferences),
	SYSERR (WSAETIMEDOUT,			ErrConnectionTimedOut),
	SYSERR (WSAECONNREFUSED,		ErrConnectionRefused),
	SYSERR (WSAEHOSTDOWN,			ErrHostDown),
	SYSERR (WSAEHOSTUNREACH,		ErrNoRouteToHost),
	SYSERR (WSAEALREADY,			ErrOperationAlreadyInProgress),
	SYSERR (WSAEINPROGRESS,			ErrOperationInProgress),
	SYSERR (WSAESTALE,			ErrStaleNFSHandle),
	SYSERR (ERROR_NOT_READY,		ErrNoMediumFound),
	SYSERR (WSAEOPNOTSUPP,			ErrOperationNotSupported),
	SYSERR (ERROR_BAD_NETPATH,		ErrNoSuchShare),
	SYSERR (ERROR_BAD_NET_NAME,		ErrNoSuchShare),
	SYSERR (WSAEPROCLIM,			ErrProcessLimitExceeded),
	SYSERR (WSAEDQUOT,			ErrDiskQuotaExceeded),
	SYSERR (WSANOTINITIALISED,		ErrNotInitialised),
	SYSERR (WSASYSNOTREADY,			ErrSystemNotReady),
	SYSERR (WSAVERNOTSUPPORTED,		ErrVersionNotSupported),
#endif // WIN32
	{ ErrNone,				0 }
    };

    for (const Mapping *m = mappings; m->portable != ErrNone; ++m)
	if (m->syserr == syserr)
	    return m->portable;

    return ErrUnknown;
}
    
SysErr::Code
SystemError::translateFromHErrno (int syserr)
{
    static const struct Mapping { SysErr::Code portable; int syserr; } mappings [] = {
#ifndef _WIN32
	SYSERR (HOST_NOT_FOUND,		ErrHostNotFound),
	SYSERR (TRY_AGAIN,		ErrHostTryAgain),
	SYSERR (NO_RECOVERY,		ErrHostNonRecoverableError),
	SYSERR (NO_DATA,		ErrHostNoDataRecord),
	SYSERR (NO_ADDRESS,		ErrHostNoAddressRecord),
#else // _WIN32
	SYSERR (WSAHOST_NOT_FOUND,	ErrHostNotFound),
	SYSERR (WSATRY_AGAIN,		ErrHostTryAgain),
	SYSERR (WSANO_RECOVERY,		ErrHostNonRecoverableError),
	SYSERR (WSANO_DATA,		ErrHostNoDataRecord),
	SYSERR (WSANO_ADDRESS,		ErrHostNoAddressRecord),
#endif
	{ ErrNone,			0 }
    };

    for (const Mapping *m = mappings; m->portable != ErrNone; ++m)
	if (m->syserr == syserr)
	    return m->portable;

    return ErrUnknown;
}
    
SystemError::SystemError (int syserror, SysErr::Code portable /* = ErrNone */)
    : Error (0),
      m_error (syserror),
      m_portable (portable == ErrNone ? translateFromErrno (syserror) : portable)
{}

SystemError::SystemError (Error *next, int syserror, SysErr::Code portable /* = ErrNone */)
    : Error (next),
      m_error (syserror),
      m_portable (portable == ErrNone ? translateFromErrno (syserror) : portable)
{}

std::string
SystemError::explainSelf (void) const
{
  return StringFormat ("System error: %1 (<#%2,#%3>)")
	.arg (errorString (m_error))
	.arg (m_error)
	.arg (m_portable)
	.value ();
}

std::string
SystemError::errorString (int syserror)
{
#ifdef _WIN32
    LPVOID buffer = 0;
    ::FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		     0, syserror, MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		     (LPTSTR) &buffer, 0, 0);
    std::string message ((const char *) buffer);
    ::LocalFree (buffer);
    return message;
#elif HAVE_STRERROR
    return strerror (syserror);
#else
    // FIXME: not thread safe! -- use tsd instead?
    static char buf [48];  // ample for 128+ bit ints
    sprintf (buf, "Error %d", syserror);
    return buf;
#endif
}

Error *
SystemError::clone (void) const
{ return new SystemError (*this); }

void
SystemError::rethrow (void)
{ throw *this; }

} // namespace lat
