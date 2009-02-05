#ifndef CLASSLIB_SYS_ERR_H
# define CLASSLIB_SYS_ERR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

namespace lat {
namespace SysErr {
	
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

    enum Code
    {
	ErrNone = 0,
	ErrUnknown,

	// errno
	ErrOperationNotPermitted,		// EPERM, NOT_OWNER
	ErrNoSuchEntity,			// ENOENT, FILE_NOT_FOUND, PATH_NOT_FOUND
	ErrNoSuchProcess,			// ESRCH
	ErrInterruptedSystemCall,		// EINTR, WSAEINTR, INVALID_AT_INTERRUPT_TIME
	ErrIO,					// EIO, NO_SIGNAL_SENT, IO_DEVICE, CRC
	ErrNoSuchDeviceOrAddress,		// ENXIO, FILE_INVALID
	ErrArgListTooLong,			// E2BIG
	ErrExecFormat,				// ENOEXEC
	ErrBadFileNumber,			// EBADF, WSAEBADF, INVALID_HANDLE
	ErrNoChildProcesses,			// ECHILD
	ErrTryAgain,				// EAGAIN, EWOULDBLOCK, WSAEWOULDBLOCK, MORE_DATA, OPEN_FILES, ACTIVE_CONNECTIONS, DEVICE_IN_USE
	ErrOutOfMemory,				// ENOMEM, NOT_ENOUGH_MEMORY
	ErrPermissionDenied,			// EACCES, WSAEACCESS, ACCESS_DENIED, SHARING_VIOLATION, LOCK_VIOLATION
	ErrBadAddress,				// EFAULT, WSAEFAULT, PROCESS_ABORTED, NOACCESS
	ErrBlockDeviceRequired,			// ENOTBLK
	ErrDeviceOrResourceBusy,		// EBUSY, PIPE_BUSY, PIPE_CONNECTED
	ErrFileExists,				// EEXIST, ENOTEMPTY, WSAENOTEMPTY, ALREADY_EXISTS
	ErrCrossDeviceLink,			// EXDEV, NOT_SAME_DEVICE
	ErrNoSuchDevice,			// ENODEV, BAD_UNIT, BAD_DEVICE
	ErrNotDirectory,			// ENOTDIR, DIRECTORY
	ErrIsDirectory,				// EISDIR
	ErrInvalidArgument,			// EINVAL, WSAEINVAL, INVALID_DATA, FILENAME_EXCED_RANGE, META_EXPANSION_TOO_LONG, INVALID_SIGNAL_NUMBER, THREAD_1_INACTIVE, BAD_PIPE, NO_TOKEN, BAD_USERNAME, NEGATIVE_SEEK
	ErrFileTableOverflow,			// ENFILE, NO_MORE_FILES(ENMFILE?)
	ErrTooManyOpenFiles,			// EMFILE, WSAEMFILE, TOO_MANY_OPEN_FILES
	ErrNotTTY,				// ENOTTY
	ErrTextFileBusy,			// ETXTBSY
	ErrFileTooLarge,			// EFBIG
	ErrNoSpaceLeftOnDevice,			// ENOSPC, HANDLE_DISK_FULL, END_OF_MEDIA, EOM_OVERFLOW, NO_DATA_DETECTED, DISK_FULL
	ErrIllegalSeek,				// ESPIPE, BEGINNING_OF_MEDIA, SETMARK_DETECTED
	ErrReadOnlyFileSystem,			// EROFS, WRITE_PROTECT
	ErrTooManyLinks,			// EMLINK
	ErrBrokenPipe,				// EPIPE
	ErrMathArgumentOutOfDomain,		// EDOM
	ErrMathResultOutOfRange,		// ERANGE
	ErrResourceDeadlockWouldOccur,		// EDEADLK, EDEADLOCK, POSSIBLE_DEADLOCK
	ErrFileNameTooLong,			// ENAMETOOLONG, WSAENAMETOOLONG
	ErrNoLocksAvailable,			// ENOLCK, SHARING_BUFFER_EXCEEDED
	ErrFunctionNotImplemented,		// ENOSYS, NOT_SUPPORTED
	ErrTooManySymbolicLinks,		// ELOOP, WSAELOOP
	ErrNoRequestedMessage,			// ENOMSG
	ErrIdentifierRemoved,			// EIDRM
	ErrChannelOutOfRange,			// ECHRNG
	ErrLevel2NotSynchronised,		// EL2NSYNC
	ErrLevel3Halted,			// EL3HLT
	ErrLevel3Reset,				// EL3RST
	ErrLinkNumberOutOfRange,		// ELNRNG
	ErrProtocolDriverNotAtached,		// EUNATCH
	ErrNoCSIStructure,			// ENOCSI
	ErrLevel2Halted,			// EL2HLT
	ErrInvalidExchange,			// EBADE
	ErrInvalidRequestDescriptor,		// EBADR
	ErrExchangeFull,			// EXFULL
	ErrNoANode,				// ENOANO
	ErrInvalidRequestCode,			// EBADRQC, INVALID_FUNCTION
	ErrInvalidSlot,				// EBADSLT
	ErrBadFontFileFormat,			// EBFONT
	ErrDeviceNotStream,			// ENOSTR
	ErrNoDataAvailable,			// ENODATA, HANDLE_EOF
	ErrTimerExpired,			// ETIME
	ErrOutOfStreamsResources,		// ENOSR
	ErrNoNetwork,				// ENONET, REM_NOT_LIST
	ErrPackageNotInstalled,			// ENOPKG
	ErrObjectIsRemote,			// EREMOTE, WSAEREMOTE
	ErrSeveredLink,				// ENOLINK, NOT_CONNECTED
	ErrAdvertise,				// EADV
	ErrSrmount,				// ESRMNT
	ErrOnSendCommunication,			// ECOMM, PIPE_NOT_CONNECTED, PIPE_LISTENING
	ErrProtocol,				// EPROTO
	ErrMultihopAttempted,			// EMULTIHOP
	ErrRFSSpecific,				// EDOTDOT
	ErrNotDataMessage,			// EBADMSG
	ErrValueTooLarge,			// EOVERFLOW
	ErrNameNotUniqueOnNetwork,		// ENOTUNIQ, DUP_NAME
	ErrBadFileDescriptorState,		// EBADFD
	ErrRemoteAddressChanged,		// EREMCHG
	ErrInaccessibleSharedLibrary,		// ELIBACC
	ErrCorruptedSharedLibrary,		// ELIBBAD
	ErrCorruptedLibrarySection,		// ELIBSCN
	ErrTooManySharedLibraries,		// ELIBMAX
	ErrCannotExecuteSharedLibrary,		// ELIBEXEC
	ErrIllegalByteSequence,			// EILSEQ
	ErrRestartSystemCall,			// ERESTART
	ErrStreamsPipe,				// ESTRPIPE
	ErrTooManyUsers,			// EUSERS, WSAEUSERS
	ErrNotSocket,				// ENOTSOCK, WSAENOTSOCK
	ErrDestinationAddressRequired,		// EDESTADDRREQ, WSAEDESTADDRREQ
	ErrMessageTooLong,			// EMSGSIZE, WSAEMSGSIZE
	ErrWrongProtocolType,			// EPROTOTYPE, WSAEPROTOTYPE
	ErrUnavailableProtocol,			// ENOPROTOOPT, WSAENOPROTOOPT
	ErrUnsupportedProtocol,			// EPROTONOSUPPORT, WSAEPROTONOSUPPORT
	ErrUnsupportedSocketType,		// ESOCKTNOSUPPORT, WSAESOCKTNOSUPPORT
	ErrUnsupportedProtocolFamily,		// EPFNOSUPPORT, WSAEPFNOSUPPORT
	ErrUnsupportedAdressFamily,		// EAFNOSUPPORT, WSAEAFNOSUPPORT
	ErrAddressInUse,			// EADDRINUSE, WSAEADDRINUSE
	ErrAddressNotAvailable,			// EADDRNOTAVAIL, WSAEADDRNOTAVAIL
	ErrNetworkDown,				// ENETDOWN, WSAENETDOWN
	ErrNetworkUnreachable,			// ENETUNREACH, WSAENETUNREACH
	ErrNetworkReset,			// ENETRESET, WSAENETRESET
	ErrConnectionAborted,			// ECONNABORTED, WSAECONNABORTED
	ErrConnectionResetByPeer,		// ECONNRESET, WSAECONNRESET
	ErrNoBuffersAvailable,			// ENOBUFS, WSAENOBUFS
	ErrAlreadyConnected,			// EISCONN, WSAEISCONN
	ErrNotConnected,			// ENOTCONN, WSAENOTCONN
	ErrConnectionAlreadyShutDown,		// ESHUTDOWN, WSAESHUTDOWN
	ErrTooManyReferences,			// ETOOMANYREFS, WSAETOOMANYREFS
	ErrConnectionTimedOut,			// ETIMEDOUT, WSAETIMEDOUT
	ErrConnectionRefused,			// ECONNREFUSED, WSAECONNREFUSED
	ErrHostDown,				// EHOSTDOWN, WSAEHOSTDOWN
	ErrNoRouteToHost,			// EHOSTUNREACH, WSAEHOSTUNREACH
	ErrOperationAlreadyInProgress,		// EALREADY, WSAEALREADY
	ErrOperationInProgress,			// EINPROGRESS, WSAEINPROGRESS
	ErrStaleNFSHandle,			// ESTALE, WSAESTALE
	ErrStructureNeedsCleaning,		// EUCLEAN
	ErrNotNamedTypeFile,			// ENOTNAM
	ErrNoSemaphoresAvailable,		// ENAVAIL
	ErrIsNamedTypeFile,			// EISNAM
	ErrRemoteIO,				// EREMOTEIO
	ErrQuotaExceeded,			// EDQUOT
	ErrNoMediumFound,			// ENOMEDIUM, NOT_READY
	ErrWrongMediumType,			// EMEDIUMTYPE
	ErrOperationNotSupported,		// ENOTSUP, EOPNOTSUPP, WSAEOPNOTSUPP
	ErrOperationCanceled,			// ECANCELED
	ErrProcessDiedWithLock,			// EOWNERDEAD
	ErrLockNotRecovarable,			// ENOTRECOVERABLE
	ErrLockUnmapped,			// ELOCKUNMAPPED
	ErrNoSuchSymbolInExecutable,		// ENOSYM
	ErrRemotePeerReleasedConnection,	// EREMOTERELEASE

	// FIXME
	ErrNoSuchShare,				// ENOSHARE, BAD_NETPATH, BAD_NET_NAME // no such host or network path
	ErrProcessLimitExceeded,		// EPROCLIM, WSAEPROCLIM
	ErrDiskQuotaExceeded,			// EDQUOT, WSAEDQUOT

	ErrNotInitialised,			// WSANOTINITIALISED
	ErrSystemNotReady,			// WSASYSNOTREADY
	ErrVersionNotSupported,			// WSAVERNOTSUPPORTED

	// h_errno
	ErrHostNotFound,			// HOST_NOT_FOUND, WSAHOST_NOT_FOUND
	ErrHostTryAgain,			// TRY_AGAIN, WSATRY_AGAIN
	ErrHostNonRecoverableError,		// NO_RECOVERY, WSANO_RECOVERY
	ErrHostNoDataRecord,			// NO_DATA, WSANO_DATA
	ErrHostNoAddressRecord			// NO_ADDRESS
    };


//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>
} // namespace SysErr
} // namespace lat

#endif // CLASSLIB_SYS_ERR_H
