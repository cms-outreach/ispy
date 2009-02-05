#ifndef CLASSLIB_SYSAPI_WINDOWS_H
# define CLASSLIB_SYSAPI_WINDOWS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include "classlib/utils/AutoLoad.h"
#  include <windows.h>
#  include <powrprof.h>
//#  include <ntdef.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

# define STATUS_SUCCESS ((NTSTATUS) 0x0L)

# define NtQueryInformationProcess	(*MyNtQueryInformationProcess)
# define CallNtPowerInformation		(*MyCallNtPowerInformation)
# ifndef GetComputerNameEx
#  ifdef UNICODE
#   define GetComputerNameExW		(*MyGetComputerNameExW)
#  else
#   define GetComputerNameExA		(*MyGetComputerNameExA)
#  endif
# endif


//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

// FIXME: This stuff lives in ntddk.h/ntddl.h, but apparently doesn't
// necessarily coexist with windows.h very well.  So add the definitions
// we need here.

enum PROCESSINFOCLASS {
    ProcessBasicInformation	= 0,
    ProcessQuotaLimits		= 1,
    ProcessVmCounters		= 3,
    ProcessTimes		= 4
};

typedef LONG  NTSTATUS;
typedef ULONG KAFFINITY;
typedef LONG  KPRIORITY;

struct PROCESS_BASIC_INFORMATION {
    NTSTATUS	ExitStatus;			// MSDN: PVOID Reserved1;
    void	*PebBaseAddress;		// MSDN: PPEB  PebBaseAddress;
    KAFFINITY	AffinityMask;			// MSDN: PVOID Reserved2 [2];
    KPRIORITY	PriorityMask;			//   -- "" --
    ULONG	UniqueProcessId;
    ULONG	InheritedFromUniqueProcessId;	// MSDN: PVOID Reserved3
};

typedef struct _PROCESSOR_POWER_INFORMATION {  
    ULONG Number;
    ULONG MaxMhz;
    ULONG CurrentMhz;
    ULONG MhzLimit;
    ULONG MaxIdleState;
    ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

// Work around functions we don't have appropriate headers for, or
// don't want to force everyone to know which libraries to link against.

extern lat::AutoLoadLib WinStubNTDLL;
extern lat::AutoLoadLib WinStubKernel32;
extern lat::AutoLoadLib WinStubPowrprof;
extern lat::AutoLoad<NTSTATUS (HANDLE hProcess, PROCESSINFOCLASS pic,
                      PVOID pi, ULONG len, PULONG plen)>
    MyNtQueryInformationProcess;
extern lat::AutoLoad<NTSTATUS (POWER_INFORMATION_LEVEL level, PVOID pin,
                      ULONG nin, PVOID pout, ULONG nout)>
    MyCallNtPowerInformation;
extern lat::AutoLoad<BOOL (COMPUTER_NAME_FORMAT fmt, LPWSTR name, LPDWORD size)>
    MyGetComputerNameExW;
extern lat::AutoLoad<BOOL (COMPUTER_NAME_FORMAT fmt, LPSTR name, LPDWORD size)>
    MyGetComputerNameExA;

//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# endif // _WIN32
#endif // CLASSLIB_SYSAPI_WINDOWS_H
