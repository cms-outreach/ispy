//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/TarInputStream.h"
#include "classlib/zip/TarMember.h"
#include "classlib/zip/TarError.h"
#include "classlib/utils/DebugAids.h"
#include <numeric>
#include <string>
#include <cstring>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

TarInputStream::TarInputStream (InputStream *input)
    : FilterInputStream (input),
      m_current (0),
      m_remaining (0),
      m_padding (0)
{}

/** Close the archive.  */
TarInputStream::~TarInputStream (void)
{ delete m_current; }

/** Return the number of bytes left to be read from the current
    archive member.

    This returns the number of bytes that should be left of the
    current member, it doesn't actually check whether there is
    anything available to be read from upstream.  */
IOSize
TarInputStream::available (void)
{
    if (! m_current && ! nextMember ())
	// End of archive
	return 0;

    return IOSized (m_remaining);
}

/** Read @a length bytes into @a buffer from the current archive
    member.  */
IOSize
TarInputStream::read (void *into, IOSize n)
{
    // Fetch the first header if necessary
    if (! m_current && ! nextMember ())
	// Oops, end of archive; FIXME: throw?
	return 0;

    // Refuse to read beyond end of current member
    if (! m_remaining)
	return 0;

    // We have a member to read from.  Don't read past its end.
    IOSize did = m_input->read (into, std::min (n, IOSized (m_remaining)));
    m_remaining -= did;
    return did;
}

/// Skip @a n bytes in the current archive member.
IOSize
TarInputStream::skip (IOSize n)
{
    // Fetch the first header if necessary
    if (! m_current && ! nextMember ())
	// Oops, end of archive; FIXME: throw?
	return 0;

    n = m_input->skip (std::min (n, IOSized (m_remaining)));
    m_remaining -= n;
    return n;
}

void
TarInputStream::close (void)
{
    delete m_current;
    m_current = 0;
    m_remaining = m_padding = 0;
    m_input->close ();
}

void
TarInputStream::reset (void)
{
    FilterInputStream::reset ();

    m_remaining = m_padding = 0;
    delete m_current;
    m_current = 0;
}

/** Read the header of the next archive member and get ready to read
    its data content.  */
TarMember *
TarInputStream::nextMember (void)
{
    // If a member is already open, finish reading it
    if (m_current)
	closeMember ();

    // File contents are not transformed in any way.  In header, all
    // integer values are stored as zero-filled ascii octal numbers
    // followed by a space and a null; names (name, linkname, magic,
    // uname and gname) are stored null-terminated strings;

    // Must be looking at the header and have closed previous one.
    ASSERT (m_input);
    ASSERT (! m_current);
    ASSERT (m_remaining == 0);
    ASSERT (m_padding == 0);

    // Read off the next header; we are in the right position to find
    // one here.  The end of archive is indicated by a block of zeroes
    // (block is 512 bytes = HEADER_SIZR), though it may not be there.
    // There can be trailing garbage after the end marker: the archive
    // has probably been rounded up to the record size (20 blocks by
    // default).
    // 
    // We don't get rid of the trailing junk: we don't know the record
    // size so we can't tell when it would end (FIXME?).  So once we
    // indicate stream end immediately upon seeing the marker.  Bad
    // news to anyone trying to read from the same stream after us...
    unsigned char	buf [HEADER_SIZE];
    IOSize		n;

    if (! extract (m_input, buf, HEADER_SIZE, HEADER_SIZE))
	throw TarError ();

    // Check if this is the end-of-archive marker of all zeroes
    for (n = 0; n < HEADER_SIZE && !buf [n]; ++n) ;
    if (n == HEADER_SIZE)
	return 0;

    // Validate the header (FIXME: version seems like gobbledy gook,
    // we expect "00" but space + null or other weird stuff; should
    // we check for it as an octal number -- this would match?)
    if (strncmp    ((const char *)buf+MAGIC_OFFSET,   MAGIC,   MAGIC_LENGTH)
	// || strncmp ((const char *)buf+VERSION_OFFSET, VERSION, VERSION_LENGTH)

	// Check that all the numeric fields are ok
	|| !validate (buf + MODE_OFFSET,     MODE_LENGTH)
	|| !validate (buf + UID_OFFSET,      UID_LENGTH)
	|| !validate (buf + GID_OFFSET,      GID_LENGTH)
	|| !validate (buf + SIZE_OFFSET,     SIZE_LENGTH)
	|| !validate (buf + MTIME_OFFSET,    MTIME_LENGTH)
	|| !validate (buf + CHKSUM_OFFSET,   CHKSUM_LENGTH)
	|| !validate (buf + DEVMAJOR_OFFSET, DEVMAJOR_LENGTH)
	|| !validate (buf + DEVMINOR_OFFSET, DEVMINOR_LENGTH)

	// Check that string fields required to be null-terminated are
	|| !memchr (buf + UNAME_OFFSET, 0, UNAME_LENGTH)
	|| !memchr (buf + GNAME_OFFSET, 0, GNAME_LENGTH))
	throw TarError ();

#if 0
    // Check the header checksum: sum of all 8-bit unsigned bytes in
    // the header, with checksum itself taken as blanks.  Some old
    // tars calculate the checksum from signed 8-bit bytes so accept
    // either.  FIXME: We are not getting a sum nowhere right.
    unsigned chksum = getOctal (buf + CHKSUM_OFFSET, CHKSUM_LENGTH-2);
    memset (buf + CHKSUM_OFFSET, CHKSUM_LENGTH, '\0');
    if (std::accumulate (buf, buf + HEADER_SIZE, 0u) != chksum
	&& std::accumulate ((signed char *) buf,
			    (signed char *) buf + HEADER_SIZE, 0)
	!= (int) chksum)
	throw TarError ();
#endif

    // OK, it sure is a valid TAR header.  Extract the various
    // values.
    int		  mode     = getOctal (buf+MODE_OFFSET,     MODE_LENGTH-1);
    int		  uid      = getOctal (buf+UID_OFFSET,      UID_LENGTH-1);
    int		  gid      = getOctal (buf+GID_OFFSET,      GID_LENGTH-1);
    IOOffset	  size     = getOctal (buf+SIZE_OFFSET,     SIZE_LENGTH-1);
    unsigned	  mtime    = getOctal (buf+MTIME_OFFSET,    MTIME_LENGTH-1);
    int		  devmajor = getOctal (buf+DEVMAJOR_OFFSET, DEVMAJOR_LENGTH-1);
    int		  devminor = getOctal (buf+DEVMINOR_OFFSET, DEVMINOR_LENGTH-1);
    unsigned char type     = buf [TYPE_OFFSET];

    // Convert aliased type to canonical
    if (type == '\0')
	type = REGULAR;

    // Unknown mode bits should be ignored

    // Check that the header is consistent
    if (! (type == REGULAR
	   || type == HARD_LINK
	   || type == SYM_LINK
	   || type == CHAR_DEVICE
	   || type == BLOCK_DEVICE
	   || type == DIRECTORY
	   || type == FIFO
	   || type == CONTIGUOUS)
	// Only links can have a link name
	|| (buf [LINK_OFFSET] && ! (type == HARD_LINK || type == SYM_LINK))
	// Most things can't have a size (FIXME: directories could
	// have maximum allowed size?)
	|| ((type == HARD_LINK      || type == SYM_LINK
	     || type == CHAR_DEVICE || type == BLOCK_DEVICE
	     || type == DIRECTORY   || type == FIFO)
	    && size)
	// Device numbers are only valid for devices
	|| ((devmajor || devminor)
	    && ! (type == CHAR_DEVICE || type == BLOCK_DEVICE)))
	throw TarError ();


    // Now grab the various file names.
    std::string prefix (extractStr (buf + PREFIX_OFFSET, PREFIX_LENGTH));
    std::string name   (extractStr (buf + NAME_OFFSET,   NAME_LENGTH));
    std::string link   (extractStr (buf + LINK_OFFSET,   LINK_LENGTH));

    if (buf [PREFIX_OFFSET])
	name = prefix + '/' + name;

    // Create a new archive member
    m_current = new TarMember (name);
    m_current->mode (mode);
    m_current->mtime (mtime);
    m_current->size (size);
    m_current->uid (uid);
    m_current->uname ((const char *) buf + UNAME_OFFSET);
    m_current->gid (gid);
    m_current->gname ((const char *) buf + GNAME_OFFSET);
    m_current->type (type);
    m_current->linkname (link);
    m_current->device (devmajor, devminor);

    // Calculate how many bytes must be read off after this file to
    // get back to the block boundary
    m_remaining = size;
    m_padding = (size + HEADER_SIZE-1) / HEADER_SIZE * HEADER_SIZE - size;
    ASSERT ((size + m_padding) % HEADER_SIZE == 0);

    // Return the new member.
    return m_current;
}

/** Skip to the end of the current member and position the stream to
    be ready to read the next archive member.  */
void
TarInputStream::closeMember (void)
{
    VERIFY (IOOffset (m_input->skip (IOSized (m_remaining + m_padding)))
	    == m_remaining + m_padding);
    m_remaining = m_padding = 0;
    delete m_current;
    m_current = 0;
}


/** Check whether @a length bytes from @a buf represent a valid octal
    number.  TAR format documentation says the fields are zero-filled
    ASCII octal integer terminated with a space and a null character.
    The reality is different.  We accept anything that is zero- or
    space-filled, has the right length, and has any number of trailing
    spaces and nulls.  */
bool
TarInputStream::validate (const void *buffer, IOSize length)
{
    const unsigned char *buf = static_cast<const unsigned char *> (buffer);

    // Allow leading spaces (should be zeroes, but often are not)
    for ( ; length && *buf == ' '; --length, ++buf)
	;

    // Now check for digits
    for (--length ; length; --length, ++buf)
	if (*buf < '0' || *buf > '7')
	    break;

    // Remaining data can be nulls or spaces
    for (++length; length; --length, ++buf)
	if (*buf != ' ' && *buf != '\0')
	    return false;

    return true;
}

/** Extract a possibly null-terminated string starting at @a buffer and
    of maximum length @a max.  */
std::string
TarInputStream::extractStr (const void *buffer, IOSize max)
{
    const unsigned char *buf = static_cast<const unsigned char *> (buffer);
    const unsigned char *end = (const unsigned char *) memchr (buf, 0, max);
    return std::string ((const char *) buf, end ? end - buf : max);
}

} // namespace lat
