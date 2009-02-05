#ifndef CLASSLIB_IO_FLAGS_H
# define CLASSLIB_IO_FLAGS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

namespace lat {
namespace IOFlags {

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

    /// File open mode flags.
    enum OpenMode
    {
	OpenRead	= 1,	/*< Open for read access.  */
	OpenWrite	= 2,	/*< Open for write access.  */
	OpenNonBlock	= 4,	/*< Open in non-blocking mode.
				    Neither the #open() nor any
				    subsequent operation on the
				    returned descriptor cause the
				    process to wait.  Applied to
				    opening a named pipe (a FIFO)
				    allows the pipe to be opened
				    without waiting for the other end
				    to be connected.  */
	OpenAppend	= 8,	/*< Writes to the file should always
				    append to the end of the file.  */
	OpenUnbuffered	= 16,	/*< No write caching should be done on
				    this file; may result in blocking
				    the process until the data has
				    actually been written.  */
	OpenCreate	= 32,	/*< Create the file if it does not exist.  */
	OpenExclusive	= 64,	/*< With #ModeCreate specifies that
				    the creation should fali if the
				    file exists.  */
	OpenTruncate	= 128,	/*< If the file exists, truncate it to
				    zero size.  */
	OpenNotCTTY	= 256	/*< If the specified file is a
				    terminal device, do not make it
				    the controlling terminal for the
				    process even if the process does
				    not have one yet.  */

	// FIXME: Binary/Text -- these should be at higher stream
	// abstraction level.
    };

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace IOFlags
} // namespace lat

#endif // CLASSLIB_IO_FLAGS_H
