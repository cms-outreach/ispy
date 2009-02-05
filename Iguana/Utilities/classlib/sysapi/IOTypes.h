#ifndef CLASSLIB_SYSAPI_IO_TYPES_H
# define CLASSLIB_SYSAPI_IO_TYPES_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/LongLong.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/** Invalid channel descriptor constant.  */
#ifdef _WIN32
#define IOFD_INVALID (void *)-1
#else
#define IOFD_INVALID -1
#endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

/** Type for buffer sizes in I/O operations.  It measures units in
    memory: buffer sizes, amounts to read and write, etc., and is
    unsigned.  Never use IOSize to measure file offsets, as it is
    most likely smaller than the file offset type on your system!  */
typedef size_t IOSize;

/** Type for file offsets for I/O operations, including file sizes.
    This type is always compatible with large files (64-bit offsets)
    whether the system supports them or not.  This type is signed.  */
typedef LongLong IOOffset;

/** Type the system uses for channel descriptors.  */
#ifdef _WIN32
typedef void *IOFD;
#else
typedef int   IOFD;
#endif

/** I/O operation mask.  */
enum IOMask {
    IORead	= 0x01,	//< Read
    IOWrite	= 0x02,	//< Write
    IOUrgent	= 0x04,	//< Exceptional or urgent condition
    IOAccept	= 0x08,	//< Socket accept
    IOConnect	= 0x10	//< Socket connect
};

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

/** Safely convert IOOffset @a n into a #IOSize quantity.  If @a n is
    larger than what #IOSize can hold, it is truncated to maximum
    value that can be held in #IOSize.  */
inline IOSize
IOSized (IOOffset n)
{
    // If IOSize and IOOffset are the same size, largest positive value
    // is half of maximum IOSize.  Otherwise all bits of IOSize work.
    IOOffset largest = (sizeof (IOOffset) == sizeof (IOSize)
		    	? ~IOSize(0)/2 : ~IOSize(0));
    return IOSize (n > largest ? largest : n);
}

//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_SYSAPI_IO_TYPES_H
