#ifndef CLASSLIB_BZ_CONSTANTS_H
# define CLASSLIB_BZ_CONSTANTS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/ZipUtils.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Constants related to BZLIB compression and BZIP archives.  */
struct BZConstants : ZipUtils
{
    // Compression block size
    enum { BEST_COMPRESSION		= 9 };  //< Best compression level
    enum { DEFAULT_COMPRESSION		= 9 };  //< Default compression level
    enum { BEST_SPEED			= 1 };  //< Fastest compression level

    // Compression engine behaviour control
    enum { DEFAULT_WORK_FACTOR		= 0 };  /*< Effort to expend before
						    falling back on the slower
						    sort algorithm.  */
    enum { MAX_WORK_FACTOR		= 250 }; //< Maximum work factor

    // BZIP stream structure (not used, BZLIB encapsulates this fully)
    enum { BZIP_MAGIC = 0x685a42 }; // "BZh"
    enum {
	BZHDR_MAGIC_OFF	= 0, BZHDR_MAGIC_LEN = 3,
	BZHDR_BLKSZ_OFF	= 3, BZHDR_BLKSZ_LEN = 1,
	BZHDR_SIZE      = 4

	// This is followed by data (FIXME: what comes at end?)
	// (FIXME: concatenated bzip2 files?).
    };
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_BZ_CONSTANTS_H
