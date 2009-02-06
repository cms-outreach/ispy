//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/ZipOutputStream.h"
#include "classlib/zip/ZipMember.h"
#include "classlib/zip/ZError.h"
#include "classlib/zip/ZCompressor.h"
#include "classlib/zip/Checksum.h"
#include "classlib/iotools/MemoryStorage.h"
#include "classlib/iotools/StorageOutputStream.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/DebugAids.h"
#include <cstring>
#include <string>
#include <zlib.h>

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

// FIXME: We have too much of knowledge about base class goings on.
// Clean reuse of ZOutputStream for multiple outputs needs to be
// thought through.
//
// At the moment we swap into the downstream m_output our "capture
// buffer" (m_buffer/m_bufout) and hold the actual output in m_bufout.
// When a member is closed, we dump m_buffer downstream and swap the
// original back into its place in m_output.  When the next member
// begins, they get swapped around again.  This is ok.
//
// The problem is in reusing the ZOutputStream and its compressor for
// multiple members -- there should be a cleaner way to capture the
// end results and reset the compressor to reuse the stream.  At the
// moment we know that `finish()' leaves the compressor and checksum
// around without resetting them so that we can capture the stats.  We
// use the stats to write out the header and the buffered output (see
// above), and then restart the compression via base class `restart()'
// method.  (We also use the base class m_checksum, knowing that it is
// a CRC32 installed by the ZOutputStream, when outputting in "STORED"
// mode, even though we are not using the base class for the output.)

ZipOutputStream::ZipOutputStream (OutputStream *output)
    : ZOutputStream (output),
      m_method (DEFLATED),
      m_level (3),
      m_total (0),

      m_current (0),
      m_buffer (new MemoryStorage),
      m_bufout (new StorageOutputStream (m_buffer)),
      m_usize (0),

      m_finished (false)
{}

/** Close the archive.  */
ZipOutputStream::~ZipOutputStream (void)
{
    for (size_t i = 0; i < m_members.size (); ++i)
	delete m_members [i];
    m_members.clear ();
    delete m_buffer;
    delete m_bufout;
}

/** Set archive comment.  */
void
ZipOutputStream::comment (const std::string &text)
{
    ASSERT (text.length () <= MAX_VAR_LEN);
    m_comment = text;
}

/** Set default compression method.  */
void
ZipOutputStream::method (Method m)
{
    ASSERT (m == DEFLATED || m == STORED);
    m_method = m;
}

/** Set default ZLIB compression level.  */
void
ZipOutputStream::level (unsigned n)
{
    ASSERT (n >= BEST_SPEED && n <= BEST_COMPRESSION);
    m_level = n;
}

/** Get ready to write the next archive member.

    The member's data is buffered internally; the member header cannot
    be written until the compression results are known.  Thus nothing
    will actually be written out downstream until closeMember(), which
    writes out both the header and entire compressed contents.
    ZipArchive implements a more efficient output method.

    If the member's compression method or level are not defined, the
    stream's default values (see method() and level()) are applied.
    If the member has no time stamp, current time is used.

    This class takes ownership of @a member; the client should no
    longer modify it.  Fields pertaining to the statistics of the
    member (its position, size and checksum) will be updated on
    closeMember().  */
void
ZipOutputStream::nextMember (ZipMember *member)
{
    ASSERT (member);

    // Can't be at the end already.
    ASSERT (! m_finished);

    // Terminate current member if one is active (FIXME: document!)
    if (m_current)
	closeMember ();

    // Must be at the end of previous member.
    ASSERT (dynamic_cast<StorageOutputStream *> (m_bufout));
    ASSERT (! m_current);
    ASSERT (! m_usize);

    // Check that we don't have too many members and that the member is valid.
    IOSize	extlen = 0;
    void	*extra = 0;
    member->extra (ZipMember::LOCAL, &extra, &extlen);

    ASSERT (m_members.size () < MAX_VAR_LEN);
    ASSERT (member->name ().size () && member->name ().size () <= MAX_VAR_LEN);
    ASSERT (member->comment ().length () <= MAX_VAR_LEN);
    ASSERT (extlen <= MAX_VAR_LEN);
    ASSERT (member->method () == DEFLATED || member->method () == STORED);

    // Make this the current member.  Set undefined parameters.
    m_members.push_back (m_current = member);
    if (m_current->method () == ZipMember::NONE)
	m_current->method (m_method);
    if (m_current->method () == DEFLATED && m_current->level () == 0)
	m_current->level (m_level);
    if (! m_current->time ())
	m_current->time (Time::current ().ns () / TimeConst::SEC_NSECS);

    // Get ready for output: put in our capture buffer and initialise
    // the compressor to the desired settings.
    std::swap (m_bufout, m_output);
    if (m_current->method () == DEFLATED)
    {
	ASSERT (dynamic_cast<ZCompressor *> (m_compressor));
	static_cast<ZCompressor *> (m_compressor)->level (m_current->level ());
    }
}

/** Write @a length bytes from @a buffer into the current archive
    member.  */
IOSize
ZipOutputStream::write (const void *from, IOSize n)
{
    // Can't be at the end already.
    ASSERT (! m_finished);

    // At this point m_bufout points to the downstream object we were
    // given in the constructor, m_output is our capture stream.  If
    // we are compressing, we use the base class to write, otherwise
    // we bypass it and write directly to m_output.  We track the
    // uncompressed size for storing it into the header later on.

    // Check that a member is open and that the client is not trying
    // to write more output than will fit into the header size fields.
    ASSERT (m_output);
    ASSERT (m_current);
    ASSERT (m_buffer);
    ASSERT (m_bufout);
    ASSERT (IOOffset (MAX_SIZE - n) >= m_usize);

    IOSize did;
    if (m_current->method () == DEFLATED)
	did = ZOutputStream::write (from, n);
    else
    {
	did = m_output->write (from, n);
	m_checksum->update (from, did);
    }

    m_usize += did;
    return did;
}

/** Close the current member and get ready to write the next one or to
    close the stream terminate.  */
void
ZipOutputStream::closeMember (void)
{
    // Can't be at the end already.
    ASSERT (! m_finished);

    ASSERT (dynamic_cast<StorageOutputStream *> (m_output));
    ASSERT (m_current);
    ASSERT (m_buffer);
    ASSERT (m_bufout);

    if (m_current->method () == DEFLATED)
	// Flush compressor.
	ZOutputStream::finish ();

    // Prepare the header.
    unsigned char buf [ENTRY_HEADER_SIZE];
    memset (buf, 0, sizeof (buf));

    // Get the member values.  (FIXME: time format?)
    std::string	name	 (m_current->name ());
    unsigned	time     = Time::toDosDate (m_current->time ());
    unsigned    method   = m_current->method ();
    unsigned    flags    = m_current->flags ();
    IOOffset	csize    = m_buffer->size ();
    IOOffset	usize    = m_usize;
    unsigned    crc32    = checksum ();
    IOSize	extlen   = 0;
    void	*extra   = 0;

    m_current->extra (ZipMember::LOCAL, &extra, &extlen);

    // Check that no value is larger than field allocated to it and
    // things look otherwise valid.  These were already checked in
    // nextMember(), just make sure they are still valid.
    ASSERT (method == DEFLATED || method == STORED);
    ASSERT (name.size () && name.size () <= MAX_VAR_LEN);
    ASSERT (extlen <= MAX_VAR_LEN);
    ASSERT (method != STORED || csize == usize);
    ASSERT (method != DEFLATED || m_compressor->out () == csize);
    ASSERT (IOOffset (IOSized (csize)) == csize);
    ASSERT (IOOffset (IOSized (usize)) == usize);

    // Update member data for central directory
    m_current->position (m_total);
    m_current->crc32 	(crc32);
    m_current->size 	(ZipMember::COMPRESSED,   csize);
    m_current->size 	(ZipMember::UNCOMPRESSED, usize);

    // Put in the field values
    put (buf + ENTRY_MAGIC_OFF,    ZIP_MAGIC_LEN,      ENTRY_HEADER_MAGIC);
    put (buf + ENTRY_VREQD_OFF,    ENTRY_VREQD_LEN,    ARCHIVE_VERSION);
    put (buf + ENTRY_FLAG_OFF,     ENTRY_FLAG_LEN,     flags);
    put (buf + ENTRY_METHOD_OFF,   ENTRY_METHOD_LEN,   method);
    put (buf + ENTRY_DATE_OFF,     ENTRY_DATE_LEN,     time);
    put (buf + ENTRY_CRC32_OFF,    ENTRY_CRC32_LEN,    crc32);
    put (buf + ENTRY_CSIZE_OFF,    ENTRY_CSIZE_LEN,    IOSized (csize));
    put (buf + ENTRY_USIZE_OFF,    ENTRY_USIZE_LEN,    IOSized (usize));
    put (buf + ENTRY_NAMELEN_OFF,  ENTRY_NAMELEN_LEN,  name.size ());
    put (buf + ENTRY_EXTRALEN_OFF, ENTRY_EXTRALEN_LEN, extlen);

    // Write out the header and variable length fields
    ASSERT (IOOffset (MAX_SIZE - sizeof (buf) - name.size () - extlen) >= m_total);
    m_bufout->xwrite (buf, sizeof (buf));
    m_bufout->xwrite (name.data (), name.size ());
    m_bufout->xwrite (extra, extlen);
    m_total += sizeof (buf) + name.size () + extlen;

    // Write out the data
    ASSERT (IOOffset (MAX_SIZE - csize) >= m_total);
    m_bufout->xwrite (m_buffer->data ());
    m_total += csize;

    // Reset state for the next member
    ZOutputStream::restart (); // FIXME: in nextMember()?
    m_buffer->resize (0);
    std::swap (m_output, m_bufout);
    m_current = 0;
    m_usize = 0;
}

/** Finish writing the archive by producing the directory. */
void
ZipOutputStream::finish (void)
{
    if (m_finished)
	return;

    ASSERT (m_output);
    if (m_current)
	closeMember ();

    ASSERT (dynamic_cast<StorageOutputStream *> (m_bufout));
    ASSERT (! m_current);
    ASSERT (! m_usize);
    // Cowardly refuse to write an empty archive.
    ASSERT (! m_members.empty ());
    ASSERT (m_total);

    IOOffset		diroffset = m_total;
    unsigned char	buf [DIR_HEADER_SIZE];

    ASSERT (diroffset <= IOOffset (MAX_SIZE));

    // Write the central directory.
    for (size_t i = 0; i < m_members.size (); ++i)
    {
	// Prepare the header.
	memset (buf, 0, sizeof (buf));
	
	m_current = m_members [i];

	// Get the member values.  (FIXME: time format?)
	std::string	name	 (m_current->name ());
	std::string	comment	 (m_current->comment ());
	unsigned	time     = Time::toDosDate (m_current->time ());
	unsigned        method   = m_current->method ();
	unsigned        flags    = m_current->flags ();
	IOOffset	csize    = m_current->size (ZipMember::COMPRESSED);
	IOOffset	usize    = m_current->size (ZipMember::UNCOMPRESSED);
	unsigned        crc32    = m_current->crc32 ();
	unsigned	intattr  = m_current->attributes (ZipMember::INTERNAL);
	unsigned	extattr  = m_current->attributes (ZipMember::EXTERNAL);
	IOOffset	offset   = m_current->position();
	IOSize		extlen   = 0;
	void		*extra   = 0;

	m_current->extra (ZipMember::CENTRAL, &extra, &extlen);

	// Check that no value is larger than field allocated to it
	// and things look otherwise valid.
	ASSERT (name.size () && name.size () <= MAX_VAR_LEN);
	ASSERT (comment.length () <= MAX_VAR_LEN);
	ASSERT (extlen <= MAX_VAR_LEN);
	ASSERT (IOOffset (IOSized (csize)) == csize);
	ASSERT (IOOffset (IOSized (usize)) == usize);

	// Put in the field values
	put (buf + DIR_MAGIC_OFF,      ZIP_MAGIC_LEN,      DIR_HEADER_MAGIC);
	put (buf + DIR_VMADE_OFF,      DIR_VMADE_LEN,      ARCHIVE_VERSION);
	put (buf + DIR_VREQD_OFF,      DIR_VREQD_LEN,      ARCHIVE_VERSION);
	put (buf + DIR_FLAG_OFF,       DIR_FLAG_LEN,       flags);
	put (buf + DIR_METHOD_OFF,     DIR_METHOD_LEN,     method);
	put (buf + DIR_DATE_OFF,       DIR_DATE_LEN,       time);
	put (buf + DIR_CRC32_OFF,      DIR_CRC32_LEN,      crc32);
	put (buf + DIR_CSIZE_OFF,      DIR_CSIZE_LEN,      IOSized (csize));
	put (buf + DIR_USIZE_OFF,      DIR_USIZE_LEN,      IOSized (usize));
	put (buf + DIR_NAMELEN_OFF,    DIR_NAMELEN_LEN,    name.size ());
	put (buf + DIR_EXTRALEN_OFF,   DIR_EXTRALEN_LEN,   extlen);
	put (buf + DIR_COMMENTLEN_OFF, DIR_COMMENTLEN_LEN, comment.length ());
	put (buf + DIR_DISK_START_OFF, DIR_DISK_START_LEN, 0);
	put (buf + DIR_INT_ATTR_OFF,   DIR_INT_ATTR_LEN,   intattr);
	put (buf + DIR_EXT_ATTR_OFF,   DIR_EXT_ATTR_LEN,   extattr);
	put (buf + DIR_ENTRY_POS_OFF,  DIR_ENTRY_POS_LEN,  offset);

	// Write out the header and variable length fields.
	ASSERT (IOOffset (MAX_SIZE - sizeof (buf) - name.size ()
			  - extlen - comment.length ()) >= m_total);
	m_output->xwrite (buf, DIR_HEADER_SIZE);
	m_output->xwrite (name.data (), name.size ());
	m_output->xwrite (extra, extlen);
	m_output->xwrite (comment.data (), comment.length ());
	m_total += sizeof (buf) + name.size () + extlen + comment.length ();
    }

    // Write the end-of-archive marker.
    ASSERT (m_comment.length () <= MAX_VAR_LEN);
    memset (buf, 0, sizeof (buf));

    ASSERT (m_total >= 0);
    ASSERT (m_total >= diroffset);
    ASSERT (IOOffset (IOSized (m_total)) == m_total);
    ASSERT (IOOffset (IOSized (diroffset)) == diroffset);
    put (buf + END_MAGIC_OFF,      ZIP_MAGIC_LEN,      END_HEADER_MAGIC);
    put (buf + END_DISK_OFF,       END_DISK_LEN,       0);
    put (buf + END_DIR_DISK_OFF,   END_DIR_DISK_LEN,   0);
    put (buf + END_DISK_HDRS_OFF,  END_DISK_HDRS_LEN,  m_members.size ());
    put (buf + END_TOTAL_HDRS_OFF, END_TOTAL_HDRS_LEN, m_members.size ());
    put (buf + END_DIR_SIZE_OFF,   END_DIR_SIZE_LEN,   IOSized (m_total - diroffset));
    put (buf + END_DIR_OFFSET_OFF, END_DIR_OFFSET_LEN, IOSized (diroffset));
    put (buf + END_COMMENTLEN_OFF, END_COMMENTLEN_LEN, m_comment.length ());

    ASSERT (IOOffset (MAX_SIZE - sizeof (buf) - m_comment.length ()) >= m_total);
    m_output->xwrite (buf, END_HEADER_SIZE);
    m_output->xwrite (m_comment.data (), m_comment.length ());
    m_total += sizeof (buf) + m_comment.length ();

    // Finish downstream.
    m_output->finish ();
    m_finished = true;
}


/** Finish writing the archive and close it. */
void
ZipOutputStream::close (void)
{
    // FIXME: there was a reason for us to override this here...
    finish ();
    ZOutputStream::close ();
}

} // namespace lat
