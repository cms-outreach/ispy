//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/CPIOOutputStream.h"
#include "classlib/zip/CPIOMember.h"
#include "classlib/zip/CPIOError.h"
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

CPIOOutputStream::CPIOOutputStream (OutputStream *output, int format /* = ODC_MAGIC */)
    : FilterOutputStream (output),
      m_format (format),
      m_current (0),
      m_buffer (0),
      m_remaining (0),
      m_checksum (0),
      m_finished (false)
{ ASSERT (format == ODC_MAGIC || format == SVR4_MAGIC || format == SVR4C_MAGIC); }

/** Close the archive.  */
CPIOOutputStream::~CPIOOutputStream (void)
{ delete m_current; delete m_buffer; }

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
CPIOOutputStream::nextMember (CPIOMember *member, bool autobuffer /* = false */)
{
    // Can't be at the end already.
    ASSERT (! m_finished);

    // Close previous member if it is still open.
    if (m_current)
	closeMember ();

    // Must be at the end of previous member.
    ASSERT (! m_current);
    ASSERT (! m_buffer);
    ASSERT (m_remaining == 0);
    ASSERT (! m_checksum);

    ASSERT (member);
    m_current = member;

    // Checksumming automatically turns on autobuffering
    autobuffer |= (m_format == SVR4C_MAGIC);

    // If automatic buffering is requested, postpone everything until
    // we see the end of data stream.  Otherwise write out the header;
    // we are at the right position and know everything we need.
    if (autobuffer)
	m_buffer = new MemoryStorage;
    else
	writeHeader ();
}

/** Dump out an archive member header for the current member.  */
void
CPIOOutputStream::writeHeader (void)
{
    switch (m_format)
    {
    case ODC_MAGIC:
	writePosixHeader ();
	return;

    case SVR4_MAGIC:
    case SVR4C_MAGIC:
	writeSvrHeader ();
	return;

    default:
	ASSERT (false);
    }
}

/** Write POSIX.1-style archive header for this member. */
void
CPIOOutputStream::writePosixHeader (void)
{
    unsigned char buf [ODC_HEADER_LEN];
    memset (buf, 0, sizeof (buf));

    // Get the member values (FIXME: symlink handling?)
    std::string		name  (m_current->name ());
    int			dev   = m_current->dev ();
    int			ino   = m_current->ino ();
    int			mode  = m_current->mode ();
    int			uid   = m_current->uid ();
    int			gid   = m_current->gid ();
    int			nlink = m_current->nlink ();
    int			rdev  = m_current->rdev ();
    unsigned		mtime = m_current->mtime ();
    IOOffset		size  = m_current->size ();
    IOSize		namelen = name.size ()+1;

    // POSIX archives have no padding so nothing special here.

    // Check that no value is larger than field allocated to it
    // and things look otherwise valid.
    if (! (   (mode  >= 0 && mode  <= SMALL_MAX)
	   && (dev   >= 0 && dev   <= SMALL_MAX)
	   && (ino   >= 0 && ino   <= SMALL_MAX)
	   && (uid   >= 0 && uid   <= SMALL_MAX)
	   && (gid   >= 0 && gid   <= SMALL_MAX)
	   && (nlink >= 0 && nlink <= SMALL_MAX)
	   && (rdev  >= 0 && rdev  <= SMALL_MAX)
	   && (       name.size () <= LARGE_MAX)
	   && (              mtime <= LARGE_MAX)
	   && (              size  >= 0)
	   && (              size  <= LARGE_MAX)))
	throw CPIOError ();

    // Put in the field values
    putOctal (buf +	ODC_MAGIC_OFFSET,    ODC_MAGIC_LENGTH,    ODC_MAGIC, false);
    putOctal (buf +	ODC_DEV_OFFSET,      ODC_DEV_LENGTH,      dev, false);
    putOctal (buf +	ODC_INO_OFFSET,      ODC_INO_LENGTH,      ino, false);
    putOctal (buf +	ODC_MODE_OFFSET,     ODC_MODE_LENGTH,     mode, false);
    putOctal (buf +	ODC_UID_OFFSET,      ODC_UID_LENGTH,      uid, false);
    putOctal (buf +	ODC_GID_OFFSET,      ODC_GID_LENGTH,      gid, false);
    putOctal (buf +	ODC_NLINK_OFFSET,    ODC_NLINK_LENGTH,    nlink, false);
    putOctal (buf +	ODC_RDEV_OFFSET,     ODC_RDEV_LENGTH,     rdev, false);
    putOctal (buf +	ODC_MTIME_OFFSET,    ODC_MTIME_LENGTH,    mtime, false);
    putOctal (buf +	ODC_NAMELEN_OFFSET,  ODC_NAMELEN_LENGTH,  namelen, false);
    putOctal (buf +	ODC_SIZE_OFFSET,     ODC_SIZE_LENGTH,     IOSized (size), false);

    // Write out the header and the file name.
    m_output->xwrite (buf, sizeof (buf));
    m_output->xwrite (name.c_str (), name.size ()+1);

    // Remember how much data the file has
    m_remaining = size;
}

/** Write SVR4-style archive header for this member. */
void
CPIOOutputStream::writeSvrHeader (void)
{
    unsigned char buf [SVR_HEADER_LEN];
    memset (buf, 0, sizeof (buf));

    // Get the member values (FIXME: symlink handling?)
    std::string		name      (m_current->name ());
    int			ino       = m_current->ino ();
    int			mode      = m_current->mode ();
    int			uid       = m_current->uid ();
    int			gid       = m_current->gid ();
    int			nlink     = m_current->nlink ();
    unsigned		mtime     = m_current->mtime ();
    IOOffset		size      = m_current->size ();
    int			devmajor  = m_current->devmajor ();
    int			devminor  = m_current->devminor ();
    int			rdevmajor = m_current->rdevmajor ();
    int			rdevminor = m_current->rdevminor ();
    IOSize		namelen   = name.size ()+1;
    unsigned		checksum  = m_checksum;

    // SVR archives are padded to four bytes: adjust name length.
    name += std::string ((4 - ((SVR_HEADER_LEN + namelen) % 4)) % 4, '\0');

    // Some things don't get checksums.  (FIXME: Make constants public.)
    enum { CP_IFLNK = 0120000 };
    if (m_format != SVR4C_MAGIC || (mode & CP_IFLNK))
	checksum = 0;

    // Check that no value is larger than field allocated to it
    // and things look otherwise valid.
    if (! (   (mode      >= 0 && mode      <= LARGE_MAX)
	   && (ino       >= 0 && ino       <= LARGE_MAX)
	   && (uid       >= 0 && uid       <= LARGE_MAX)
	   && (gid       >= 0 && gid       <= LARGE_MAX)
	   && (nlink     >= 0 && nlink     <= LARGE_MAX)
	   && (                  mtime     <= LARGE_MAX)
	   && (			 size	   >= 0)
	   && (                  size      <= LARGE_MAX)
	   && (devmajor  >= 0 && devmajor  <= LARGE_MAX)
	   && (devminor  >= 0 && devminor  <= LARGE_MAX)
	   && (rdevmajor >= 0 && rdevmajor <= LARGE_MAX)
	   && (rdevminor >= 0 && rdevminor <= LARGE_MAX)
	   && (                  namelen   <= SVR_NAMELEN_MAX)))
	throw CPIOError ();

    // Put in the field values.
    putOctal (buf + SVR_MAGIC_OFFSET,   SVR_MAGIC_LENGTH,     m_format, false);
    putHex (buf + SVR_INO_OFFSET,       SVR_INO_LENGTH,       ino);
    putHex (buf + SVR_MODE_OFFSET,      SVR_MODE_LENGTH,      mode);
    putHex (buf + SVR_UID_OFFSET,       SVR_UID_LENGTH,       uid);
    putHex (buf + SVR_GID_OFFSET,       SVR_GID_LENGTH,       gid);
    putHex (buf + SVR_NLINK_OFFSET,     SVR_NLINK_LENGTH,     nlink);
    putHex (buf + SVR_MTIME_OFFSET,     SVR_MTIME_LENGTH,     mtime);
    putHex (buf + SVR_SIZE_OFFSET,      SVR_SIZE_LENGTH,      IOSized (size));
    putHex (buf + SVR_DEVMAJOR_OFFSET,  SVR_DEVMAJOR_LENGTH,  devmajor);
    putHex (buf + SVR_DEVMINOR_OFFSET,  SVR_DEVMINOR_LENGTH,  devminor);
    putHex (buf + SVR_RDEVMAJOR_OFFSET, SVR_RDEVMAJOR_LENGTH, rdevmajor);
    putHex (buf + SVR_RDEVMINOR_OFFSET, SVR_RDEVMINOR_LENGTH, rdevminor);
    putHex (buf + SVR_NAMELEN_OFFSET,   SVR_NAMELEN_LENGTH,   namelen);
    putHex (buf + SVR_CHECKSUM_OFFSET,  SVR_CHECKSUM_LENGTH,  checksum);

    // Write out the header and the file name.
    m_output->xwrite (buf, sizeof (buf));
    m_output->xwrite (name.c_str (), name.size ()+1);

    // Remember how much data the file has
    m_remaining = size;
}

/** Write @a length bytes from @a buffer into the current archive
    member.  */
IOSize
CPIOOutputStream::write (const void *from, IOSize n)
{
    // Can't be at the end already.
    ASSERT (! m_finished);

    ASSERT (m_output);
    ASSERT (m_current);

    if (m_buffer)
    {
	// Autobuffering -- stash the output away
	ASSERT (! m_remaining);
	IOSize did = m_buffer->write (from, n);
        m_checksum = std::accumulate ((unsigned char *) from,
				      (unsigned char *) from + did,
				      m_checksum);
	return did;
    }
    else
    {
	// Outputting for real.
	ASSERT (signed(n) <= m_remaining);
	ASSERT (m_format != SVR4C_MAGIC);
	IOSize did = m_output->xwrite (from, n);
	m_remaining -= did;
	return did;
    }
}

/** Close the current member and get ready to write the next one or to
    close the stream terminate.  */
void
CPIOOutputStream::closeMember (void)
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
	m_checksum = 0;
	delete m_buffer;
	m_buffer = 0;
    }

    // SVR4 archives are padded to four bytes (including file data)
    if (m_format == SVR4_MAGIC || m_format == SVR4C_MAGIC)
    {
	char paddata [4] = { 0, 0, 0, 0 };
	unsigned padding = (4 - (m_current->size () % 4)) % 4;
	m_output->xwrite (paddata, padding);
    }

    // Get rid of the current member.
    delete m_current;
    m_current = 0;
}

/** Finish writing to the stream the trailer header. */
void
CPIOOutputStream::finish (void)
{
    if (m_finished)
	return;

    // Finish off the current member if one is still open.
    if (m_current)
	closeMember ();

    ASSERT (m_output);
    ASSERT (! m_current);
    ASSERT (! m_buffer);
    ASSERT (! m_checksum);

    // Write the end-of-archive marker
    if (m_format == ODC_MAGIC)
    {
	// Prepare the head, write it out and then the file name.
	unsigned char	buf [ODC_HEADER_LEN];
	std::string	name (TRAILER);

	memset (buf, '0', sizeof (buf));
	putOctal (buf + ODC_MAGIC_OFFSET,    ODC_MAGIC_LENGTH,    ODC_MAGIC, false);
	putOctal (buf + ODC_NLINK_OFFSET,    ODC_NLINK_LENGTH,    1, false);
	putOctal (buf + ODC_NAMELEN_OFFSET,  ODC_NAMELEN_LENGTH,  name.size ()+1, false);
	m_output->xwrite (buf, sizeof (buf));
	m_output->xwrite (name.c_str (), name.size ()+1);
    }
    else if (m_format == SVR4_MAGIC || m_format == SVR4C_MAGIC)
    {
	// Prepare the head, write it out and then the file name.
	unsigned char buf [SVR_HEADER_LEN + TRAILER_LEN + SVR_TRAILER_PAD];
	memset (buf, '0', sizeof (buf));
	putOctal (buf + SVR_MAGIC_OFFSET, SVR_MAGIC_LENGTH,   m_format, false);
	putHex (buf + SVR_NLINK_OFFSET,   SVR_NLINK_LENGTH,   1);
	putHex (buf + SVR_NAMELEN_OFFSET, SVR_NAMELEN_LENGTH, TRAILER_LEN);
	memcpy (buf + SVR_HEADER_LEN,     TRAILER,            TRAILER_LEN);
	memset (buf + SVR_HEADER_LEN + TRAILER_LEN, 0, SVR_TRAILER_PAD);
	m_output->xwrite (buf, sizeof (buf));
    }
    else
	ASSERT (false);

    // Close context.
    m_remaining = 0;
    m_output->finish ();
    m_finished = true;
}

} // namespace lat
