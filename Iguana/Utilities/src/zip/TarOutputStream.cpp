//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/TarOutputStream.h"
#include "classlib/zip/TarMember.h"
#include "classlib/zip/TarError.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iotools/MemoryStorage.h"
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

TarOutputStream::TarOutputStream (OutputStream *output, bool pad /* = true */)
    : FilterOutputStream (output),
      m_current (0),
      m_buffer (0),
      m_remaining (0),
      m_padding (0),
      m_blocks (0),
      m_pad (pad),
      m_finished (false)
{}

/** Close the archive.  */
TarOutputStream::~TarOutputStream (void)
{ delete m_current; delete m_buffer; }

/** Define whether trailing archive padding should be written.

    If @a pad is @c true, the archive will be padded to the default
    record size (20 blocks of 512 bytes) with null bytes, as per the
    TAR archive format specification.  If @a pad is @c false, only
    the end-of-archive marker is written but no padding.  The padding
    can be omitted if the client knows that the archive is not being
    written to a media requiring the padding (= a tape).  By default
    the padding is on (see the constructor).

    FIXME: Take record size/blocking factor instead?  */
void
TarOutputStream::setPadding (bool pad)
{ m_pad = pad; }

/** Write the header of the next archive member and get ready to write
    its data content.

    Either @a member's size must equal to the amount of data that will
    be written afterwards with write(), or @a autobuffer must be set;
    it is not possible to write any of the member data before its
    total size is known.  If @a autobuffer is off, we simply record
    the size given here and at the end of the member assert that an
    equal amount of data was actually written.  If @a autobuffer is
    on, nothing is written until the member is finished; at that time
    the header and entire contents will be written out.  Writing large
    files with autobuffering is very inefficient -- if you do know the
    size beforehand, do not buffer the contents.

    This class takes ownership of @a member.  */
void
TarOutputStream::nextMember (TarMember *member, bool autobuffer /* = false */)
{
    // Can't be at the end already.
    ASSERT (! m_finished);

    // File contents are not transformed in any way.  In header, all
    // integer values are stored as zero-filled ascii octal numbers
    // followed by a space and a null; names (name, linkname, magic,
    // uname and gname) are stored null-terminated strings.

    // Close previous member if it is still open.
    if (m_current)
	closeMember ();

    // Must be at the end of previous member.
    ASSERT (! m_current);
    ASSERT (! m_buffer);
    ASSERT (m_remaining == 0);
    ASSERT (m_padding == 0);

    ASSERT (member);
    m_current = member;

    // If automatic buffering is requested, postpone everything until
    // we see the end of data stream.  Otherwise write out the header;
    // we are at the right position and know everything we need.
    if (autobuffer)
	// FIXME: we ignore the size -- check that it is zero?
	m_buffer = new MemoryStorage;
    else
	writeHeader ();
}

/** Dump out an archive member header for the current member.  */
void
TarOutputStream::writeHeader (void)
{
    unsigned char buf [HEADER_SIZE];
    memset (buf, 0, sizeof (buf));

    // Get the member values
    std::string		prefix;
    std::string		name	 (m_current->name ());
    std::string		linkname (m_current->linkname ());
    std::string		uname	 = m_current->uname ();
    std::string		gname	 = m_current->gname ();
    int			mode	 = m_current->mode ();
    int			uid	 = m_current->uid ();
    int			gid	 = m_current->gid ();
    IOOffset		size	 = m_current->size ();
    unsigned		mtime	 = m_current->mtime ();
    int			devmajor = m_current->devmajor ();
    int			devminor = m_current->devminor ();
    unsigned char	type	 = m_current->type ();

    // Split the file name into prefix and name parts.  If it is
    // longer than what will fit into name, try to split off a part
    // into prefix, at a slash boundary.  From the end of the file
    // name we look for a slash which would leave `name' no longer
    // than NAME_LENGTH and `prefix' no longer than PREFIX_LENGTH.
    // This may not succeed; if it fails, we raise a format exception
    // to signal that the file cannot be put in a TAR archive.
    if (name.length () > NAME_LENGTH)
    {
	const char *begin = name.c_str ();
	const char *end   = begin + name.length ();
	const char *p     = end;
	do
	    for (--p ; p != begin && *p != '/'; --p) ;
	while (p != begin
	       && p - begin > PREFIX_LENGTH
	       && end - p <= NAME_LENGTH);

	ASSERT (*p == '/');
	ASSERT (p >= begin);
	ASSERT (p < end);

	if (p - begin > PREFIX_LENGTH || end - p > NAME_LENGTH)
	    throw TarError ();

	prefix = std::string (begin, p);
	name   = std::string (p+1, end);
    }

    // Check that no value is larger than field allocated to it
    // and things look otherwise valid.
    if (! (strlen (MAGIC) == MAGIC_LENGTH
	   && strlen (VERSION) == VERSION_LENGTH
	   && uname.length () < UNAME_LENGTH
	   && gname.length () < GNAME_LENGTH
	   && (mode     >= 0 && mode     <= SMALL_MAX)
	   && (uid  	>= 0 && uid      <= SMALL_MAX)
	   && (gid  	>= 0 && gid      <= SMALL_MAX)
	   && (                 size     >= 0)
	   && (                 size     <= LARGE_MAX)
	   && (                 mtime    <= LARGE_MAX)
	   && (devmajor >= 0 && devmajor <= SMALL_MAX)
	   && (devminor >= 0 && devminor <= SMALL_MAX)
	   && (type == REGULAR
	       || type == HARD_LINK
	       || type == SYM_LINK
	       || type == CHAR_DEVICE
	       || type == BLOCK_DEVICE
	       || type == DIRECTORY
	       || type == FIFO
	       || type == CONTIGUOUS)
	   // Only links can have a link name
	   && (linkname.length () == 0 || (type == HARD_LINK
					   || type == SYM_LINK))
	   // Most things can't have a size (FIXME: directories could
	   // have maximum allowed size?)
	   && ((type != HARD_LINK      && type != SYM_LINK
		&& type != CHAR_DEVICE && type != BLOCK_DEVICE
		&& type != DIRECTORY   && type != FIFO)
	       || size == 0)
	   // Device numbers are only valid for devices
	   && ((devmajor == 0 && devminor == 0)
	       || (type == CHAR_DEVICE || type == BLOCK_DEVICE))))
	throw TarError ();

    // Put in the field values
    putOctal (buf +	MODE_OFFSET,     MODE_LENGTH,     mode);
    putOctal (buf +	UID_OFFSET,      UID_LENGTH,      uid);
    putOctal (buf +	GID_OFFSET,      GID_LENGTH,      gid);
    putOctal (buf +	SIZE_OFFSET,     SIZE_LENGTH,     IOSized (size) /*, true*/);
    putOctal (buf +	MTIME_OFFSET,    MTIME_LENGTH,    mtime /*, true*/);
    if (type == CHAR_DEVICE || type == BLOCK_DEVICE)
    {
	putOctal (buf +	DEVMAJOR_OFFSET, DEVMAJOR_LENGTH, devmajor);
	putOctal (buf +	DEVMINOR_OFFSET, DEVMINOR_LENGTH, devminor);
    }

    strncpy  ((char *)buf + MAGIC_OFFSET,   MAGIC,             MAGIC_LENGTH);
    // strncpy  ((char *)buf + VERSION_OFFSET, VERSION,           VERSION_LENGTH);
    buf [VERSION_OFFSET] = ' '; // FIXME: This is what GNU tar does
    buf [VERSION_OFFSET+1] = '\0';
    strncpy  ((char *)buf + NAME_OFFSET,    name.c_str (),     NAME_LENGTH);
    strncpy  ((char *)buf + PREFIX_OFFSET,  prefix.c_str (),   PREFIX_LENGTH);
    strncpy  ((char *)buf + LINK_OFFSET,    linkname.c_str (), LINK_LENGTH);
    strncpy  ((char *)buf + UNAME_OFFSET,   uname.c_str (),    UNAME_LENGTH);
    strncpy  ((char *)buf + GNAME_OFFSET,   gname.c_str (),    GNAME_LENGTH);

    buf [TYPE_OFFSET] = type;

    // Compute checksum and put it in
    memset (buf + CHKSUM_OFFSET, ' ', CHKSUM_LENGTH);
    unsigned checksum = std::accumulate (buf, buf + HEADER_SIZE, 0u);
    ASSERT (checksum <= SMALL_MAX);
    putOctal (buf + CHKSUM_OFFSET, CHKSUM_LENGTH, checksum);

    // Write out the header and update block count
    m_output->xwrite (buf, sizeof (buf));
    m_blocks++;

    // Calculate how much padding to are needed at the end
    m_padding = (size + HEADER_SIZE-1) / HEADER_SIZE * HEADER_SIZE - size;
    m_remaining = size;

    // Update block count now, otherwise we'd have to keep track of
    // partial blocks written in write().  So just count how many
    // blocks it will take to write out m_remaining + m_padding bytes.
    ASSERT ((m_remaining + m_padding) % HEADER_SIZE == 0);
    m_blocks += (m_remaining + m_padding) % HEADER_SIZE;
}

/** Write @a length bytes from @a buffer into the current archive
    member.  */
IOSize
TarOutputStream::write (const void *from, IOSize n)
{
    // Can't be at the end already.
    ASSERT (! m_finished);

    ASSERT (m_output);
    ASSERT (m_current);

    if (m_buffer)
    {
	// Autobuffering -- stash the output away
	ASSERT (! m_remaining);
	ASSERT (! m_padding);
	return m_buffer->write (from, n);
    }
    else
    {
	// Outputting for real.
	ASSERT (IOOffset (n) <= m_remaining);
	IOSize did = m_output->xwrite (from, n);
	m_remaining -= did;
	return did;
    }
}

/** Close the current member and get ready to write the next one or to
    close the stream terminate.  */
void
TarOutputStream::closeMember (void)
{
    // Can't be at the end already.
    ASSERT (! m_finished);

    ASSERT (m_output);
    ASSERT (m_current);
    ASSERT (m_remaining == 0);

    if (m_buffer)
    {
	// Autobuffering. We are done, dump out the whole thing with
	// the header and all gathered data, and then zap the buffer.
	m_current->size (m_buffer->data ().size ());
	writeHeader ();
	m_output->xwrite (m_buffer->data ());
	m_remaining = 0;
	delete m_buffer;
	m_buffer = 0;
    }

    // Write out trailing padding to get us back to block boundary
    if (m_padding)
    {
	unsigned char buf [HEADER_SIZE];
	memset (buf, 0, sizeof (buf));
	m_output->xwrite (buf, m_padding);
	m_padding = 0;
    }

    // Get rid of the current member.
    delete m_current;
    m_current = 0;
}

/** Finish writing to the stream by padding it up to the default TAR
    record size (20 blocks).  (FIXME: only one null block if the
    output goes to a file?)  */
void
TarOutputStream::finish (void)
{
    if (m_finished)
	return;

    // Finish off the current member if one is still open.
    if (m_current)
	closeMember ();

    ASSERT (m_output);
    ASSERT (! m_current);
    ASSERT (! m_buffer);
    ASSERT (m_blocks);    // Cowardly refuse to write an empty archive.

    // Write the end-of-archive marker
    unsigned char buf [HEADER_SIZE];
    memset (buf, 0, sizeof (buf));
    m_output->xwrite (buf, sizeof (buf));
    m_blocks++;

    // Pad to the default record size (20 blocks).  The client can turn this
    // off, e.g. if it is known that the output goes to the file and the
    // trailing padding is useless.  This makes the archive nominally
    // non-conforming, but saves space and other tar programs do it anyway.
    for ( ; m_pad && m_blocks % 20; ++m_blocks)
	m_output->xwrite (buf, sizeof (buf));

    // Close context.
    m_remaining = m_padding = 0;
    m_output->finish ();
    m_finished = true;
}

} // namespace lat
