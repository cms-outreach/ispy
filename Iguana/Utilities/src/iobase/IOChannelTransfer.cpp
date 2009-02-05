//<<<<<< INCLUDES                                                       >>>>>>

// Kludged to remove _FILE_OFFSET_BITS definition from classlib/sysapi/config.h.
// This is required to be able to use sys/sendfile.h that comes on Linux.

#include "classlib/sysapi/system.h"
#undef _FILE_OFFSET_BITS

#include "classlib/iobase/IOChannel.h"
#include "classlib/utils/IntTraits.h"
#include "classlib/iobase/IOError.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/FileMapping.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/IOChannel.h"
#include "classlib/sysapi/IOChannelTransfer.h"

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Send @a length bytes from @a src to @a dest, starting at offset
    @a pos in @a src.  Transfer is done in the most efficient available
    method, using kernel space buffers if possible.  The @a dest argument
    may be a socket; @a src must be a file that can be mapped into memory.

    On return @a pos is updated to the next byte following the last
    byte read from @a src.  The return value is the number of bytes
    actually sent.  The current file offset of @a src is not modified.

    If @a dest is a socket, use #OptTCPCork option for best
    performance.  */
IOSize
IOChannel::transfer (IOChannel *src, IOChannel *dest,
		     IOOffset &pos, IOSize length)
{
    // FIXME: IOOffset length?

    ASSERT (src);
    ASSERT (dest);
    ASSERT (src->fd () != IOFD_INVALID);
    ASSERT (dest->fd () != IOFD_INVALID);

#if HAVE_LINUX_SENDFILE
    // Make sure the offset and requested length fit in available range
    if (pos >= IntTraits<off_t>::Max || signed(length) > IntTraits<off_t>::Max - pos)
	throw IOError ("sendfile()", EINVAL);

    ssize_t sent;
    off_t offset = pos;

    if ((sent = sendfile (dest->fd (), src->fd (), &offset, length)) == -1)
	throw IOError ("sendfile()", errno);

    pos = offset;
    return sent;

#elif HAVE_SOLARIS_SENDFILE
    // FIXME: Supported only with sockets?
    ASSERT (length <= IntTraits<ssize_t>::Max);
    ssize_t sent;
    if ((sent = sendfile (dest->fd (), src->fd (), &pos, count)) == -1)
	throw IOError ("sendfile()", errno);

    return sent;

#elif HAVE_FREEBSD_SENDFILE
    // FIXME: Supported only with sockets?
    // FreeBSD return error if does a partial write to non-blocking socket
    ASSERT (length <= IntTraits<ssize_t>::Max);
    off_t sent = 0;
    if (sendfile (src->fd (), dest->fd (), pos, length, 0, &sent, 0) == -1
	&& ! (errno == EAGAIN && sent > 0))
	throw IOError ("sendfile()", errno);

    pos += sent;
    return sent;

#else
    // FIXME: fall back from above if sendfile() doesn't work?
    File	srcfile (src->fd (), false);
    FileMapping input (&srcfile);
    IOSize	written
	= dest->xwrite (input.map (pos, length,
				   FileMapping::ProtRead,
				   FileMapping::MapPrivate));

    pos += written;
    return written;
#endif
}

} // namespace lat
