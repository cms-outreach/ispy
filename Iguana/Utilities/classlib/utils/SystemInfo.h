#ifndef CLASSLIB_SYSTEM_INFO_H
# define CLASSLIB_SYSTEM_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <string>
# include <cstdlib>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Obtain information about the system.  */
class SystemInfo
{
public:
    // Memory status
    static size_t	pagesize (void);
    static size_t	allocGranularity (void);
    static size_t	physicalPages (void);
    static size_t	physicalPagesAvailable (void);
    static size_t	virtualPages (void);
    static size_t	virtualPagesAvailable (void);

    // Processors
    static size_t	processors (void);
    static size_t	processorsOnline (void);

    // Computer and operating system info
    static std::string	nodename (void);
    static std::string	domainname (void);
    static std::string	osname (void);
    static std::string	osrelease (void);
    static std::string	osversion (void);
    static std::string	machine (void);
    static std::string	processor (void);

    // Windows-specific
    static std::string	windowsDirectory (void);
    static std::string	windowsSystemDirectory (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_SYSTEM_INFO_H
