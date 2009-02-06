//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/ZipInputStream.h"
#include "classlib/zip/ZipMember.h"
#include "classlib/zip/ZError.h"
#include "classlib/zip/Decompressor.h"
#include "classlib/zip/Checksum.h"
#include "classlib/utils/DebugAids.h"
#include <algorithm>
#include <string>
#include <vector>
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

ZipInputStream::ZipInputStream (InputStream *input)
    : ZInputStream (input),
      m_current (0),
      m_compressed (0),
      m_uncompressed (0),
      m_magic (0),
      m_total (0),
      m_directory (0),
      m_dummy (false)
{}

/** Reset internal state for either reading the next archive member (!
    @a destroy) or completely (@a destroy).  */
void
ZipInputStream::resetState (bool destroy)
{
    if (destroy)
    {
	for (size_t i = 0; i < m_members.size (); ++i)
	    delete m_members [i];
	m_members.clear ();

	m_directory = 0;
	m_total     = 0;
    }
    else
    {
	// Reset the decompressor and checksum, but only if we are
	// going to be reused.
	m_decompressor->reset ();	// FIXME: need base method
	m_checksum->reset ();		// FIXME: need base method

	// If m_dummy is set, the base class #unread() the dummy byte
	// we provided in #fill() so we need to get rid of it.
	if (m_dummy)
	    VERIFY (m_input->read () == 0);
    }

    m_current       = 0;
    m_compressed    = 0;
    m_uncompressed  = 0;
    m_magic         = 0;
    m_dummy	    = false;
}

/** Close the archive.  */
ZipInputStream::~ZipInputStream (void)
{ resetState (true); }

/** Return the number of bytes left to be read from the current
    archive member.

    @return The number of bytes that should be left of the current
            member (uncompressed).  No check is made to see if
            anything is actually available upstream.  */
IOSize
ZipInputStream::available (void)
{
    // Initialise first member if necessary
    if (m_magic == END_HEADER_MAGIC || (! m_current && ! nextMember ()))
	// End of archive
	return 0;
    else
	// Return remaining size
	return IOSized (m_uncompressed);
}

/** Read @a length bytes into @a buffer from the current archive
    member.  */
IOSize
ZipInputStream::read (void *into, IOSize n)
{
    // At the moment we support multiple post-EOF reads, clamping on
    // the EOF.  To reuse the stream after fully reading an archive
    // (e.g. to read end-to-end archives), reset m_magic to 0.

    // Initialise first member if necessary
    if (m_magic == END_HEADER_MAGIC || (! m_current && ! nextMember ()))
	// End of archive
	return 0;

    switch (m_current->method ())
    {
    case DEFLATED:
	// Let the base class read compressed data upto end of this member.
	n = ZInputStream::read (into, std::min (n, IOSized (m_uncompressed)));
	m_uncompressed -= n;
	return n;

    case STORED:
	// Just read data straight off from upstream.
	n = m_input->read (into, std::min (n, IOSized (m_uncompressed)));
	m_compressed -= n;
	m_uncompressed -= n;
	m_total += n;
	return n;

    default:
	// Shouldn't happen, protected in nextMember().
	ASSERT (false);
	throw ZError (Z_STREAM_ERROR);
    }
}

/** @reimp  */
IOSize
ZipInputStream::fill (void *buffer, IOSize length)
{
    // Never read beyond the (compressed) data stream of the current
    // archive member.  If the decompressor wants more input than we
    // are willing to provide, the archive is corrupted.  The client
    // can recover by finding the next valid member with sync().
    //
    // However we do need to make a deviation to this.  ZIP archive
    // data is in unwrapped ZLIB data streams with the result that
    // ZDecompressor requires an extra dummy byte after the data
    // stream in order to complete decompression and return
    // end-of-stream indication.  (The alternative would be to read
    // only as much uncompressed data as we know there to be (in
    // #read()), but we want to catch corrupted stream here.)  So
    // arrange for that dummy byte to be provided.

    if (! (length = std::min (IOSized (m_compressed), length)))
    {
	if (! m_dummy)
	{
	    m_dummy = true;
	    *(char *) buffer = 0;
	    return 1;
	}
	else
	    throw ZError (Z_DATA_ERROR);
    }

    length = m_input->read (buffer, length);
    m_compressed -= length;
    m_total += length;
    return length;
}

/// Skip @a n bytes in the current archive member.
IOSize
ZipInputStream::skip (IOSize n)
{
    // Initialise first member if necessary
    if (m_magic == END_HEADER_MAGIC || (! m_current && ! nextMember ()))
	// End of archive
	return 0;

    switch (m_current->method ())
    {
    case DEFLATED:
	// Let the base class read compressed data upto end of this member.
	n = ZInputStream::skip (std::min (n, IOSized (m_uncompressed)));
	m_uncompressed -= n;
	return n;

    case STORED:
	// Just read data straight off from upstream.
	n = m_input->skip (std::min (n, IOSized (m_uncompressed)));
	m_compressed -= n;
	m_uncompressed -= n;
	m_total += n;
	return n;

    default:
	// Shouldn't happen, protected in nextMember().
	ASSERT (false);
	throw ZError (Z_STREAM_ERROR);
    }
}

/** Close the stream and release resources used by it.  */
void
ZipInputStream::close (void)
{
    resetState (true);
    ZInputStream::close ();
}

/** Reset the stream.

    Removes all previously seen members and resets the upstream input.  */
void
ZipInputStream::reset (void)
{
    resetState (true);
    ZInputStream::reset ();
}

/** Read off the next header magic code.  The stream must already be
    positioned in the right place.  */
unsigned
ZipInputStream::readHeader (void)
{
    unsigned char buf [ZIP_MAGIC_LEN];
    if (! extract (m_input, buf, sizeof (buf), sizeof (buf)))
	throw ZError (Z_DATA_ERROR);

    return get (buf, ZIP_MAGIC_LEN);
}

/** Look for the next valid header and return its magic code.

    Searches for the next header after a corrupted input stream has
    been detected.  Reads from the input until one of the magic header
    codes is found (an entry, directory or end).

    @return The magic code for the header that was found, or zero.  if
	    we reached the end of stream without finding anything.
	    The magic code will have been read off the stream, but not
	    the rest of the header.  The caller should make sure the
	    header is indeed valid -- we may have found a rogue
	    header.  */
unsigned
ZipInputStream::findHeader (void)
{
    // This is dreadfully inefficient, but it is only error recovery.
    // Pass me a buffering input or use ZipArchive this speed matters.

    // Keep looking for characters compatible with the magic values.
    // When a character in the middle fails to match, re-examine it:
    // it might start a new match.  A successfully matched character
    // cannot start a new match and can be discarded.  On EOF return
    // zero to indicate no match was found.
    int ch = m_input->read ();
    ++m_total;
    while (true)
    {
	if (ch == -1)
	    return 0;
	else if (ch == 0x50 && (ch = m_input->read ()) != 0x4b)
	    continue;
	else if ((ch = m_input->read ()) == 0x01)
        {
	    if ((ch = m_input->read ()) == 0x02)
		return DIR_HEADER_MAGIC;
	    else
		continue;
        }
	else if (ch == 0x03)
        {
	    if ((ch = m_input->read ()) == 0x04)
		return ENTRY_HEADER_MAGIC;
	    else
		continue;
        }
	else if (ch == 0x05)
        {
	    if ((ch = m_input->read ()) == 0x06)
		return END_HEADER_MAGIC;
	    else
		continue;
        }

	ch = m_input->read ();
	m_total++;
    }
}

/** Read the header of the next archive member and get ready to read
    its data content.  */
ZipMember *
ZipInputStream::nextMember (void)
{
    // This is a non-seekable stream, we cannot seek to the central
    // directory at the end of archive (cf. ZipArchive).  We assume
    // that the stream begins with an entry header followed by member
    // contents, next entry header, ... until the central directory.
    // Once in the central directory, we pick up the fields from it
    // and fill back the ZipMembers we have already seen (m_members).

    // We don't support multi-disk archives.  The archive must contain
    // fully all the three parts: local entries, central directory and
    // the final end entry.  No prefix is allowed.  ZipArchive permits
    // a SFX header in the archive; we don't except through the client
    // catching the ZError and calling sync().

    // If a member is already open, finish reading it
    if (m_current)
	closeMember ();

    // Must be looking at the header and have closed previous member.
    // Decompressor must have fully consumed input: our fill() never
    // provides the decompressor more input than the member header
    // told there was.

    ASSERT (m_input);
    ASSERT (! m_directory);
    ASSERT (! m_current);
    ASSERT (! m_compressed);
    ASSERT (! m_uncompressed);
    ASSERT (! m_magic);
    ASSERT (! m_last);
    ASSERT (! m_dummy);

    // Read off the next header.  It must be either an entry or a
    // directory.  The header is not compressed, the data may be.
    unsigned char buf [DIR_HEADER_SIZE];
    switch (readHeader ())
    {
    case ENTRY_HEADER_MAGIC:
	return readMemberHeader (buf, sizeof (buf));

    case DIR_HEADER_MAGIC:
	readDirectory (buf, sizeof (buf));
	return 0;

    default:
	throw ZError (Z_DATA_ERROR);
    }
}

ZipMember *
ZipInputStream::readMemberHeader (unsigned char *buf, IOSize length)
{
    // Another local directory entry, start a new member.  The
    // magic has already been read; read the rest of the header.
    ASSERT (length >= ENTRY_HEADER_SIZE);
    ASSERT (buf);

    IOOffset offset = m_total;

    if (! extract (m_input, buf + ZIP_MAGIC_LEN, length,
		   ENTRY_HEADER_SIZE - ZIP_MAGIC_LEN))
	throw ZError (Z_DATA_ERROR);
    m_total += ENTRY_HEADER_SIZE;

    // Extract the various values.
    typedef std::vector<unsigned char> databuf;
    unsigned	version	= get (buf + ENTRY_VREQD_OFF,  	 ENTRY_VREQD_LEN);
    unsigned	flags	= get (buf + ENTRY_FLAG_OFF,   	 ENTRY_FLAG_LEN);
    unsigned	method	= get (buf + ENTRY_METHOD_OFF, 	 ENTRY_METHOD_LEN);
    unsigned	time	= get (buf + ENTRY_DATE_OFF, 	 ENTRY_DATE_LEN);
    unsigned	crc32	= get (buf + ENTRY_CRC32_OFF,  	 ENTRY_CRC32_LEN);
    IOOffset	csize	= get (buf + ENTRY_CSIZE_OFF,    ENTRY_CSIZE_LEN);
    IOOffset	usize	= get (buf + ENTRY_USIZE_OFF,  	 ENTRY_USIZE_LEN);
    IOSize	namelen	= get (buf + ENTRY_NAMELEN_OFF,  ENTRY_NAMELEN_LEN);
    IOSize	extlen	= get (buf + ENTRY_EXTRALEN_OFF, ENTRY_EXTRALEN_LEN);
    std::string	name    (namelen, '\0');
    databuf	extra   (extlen,  '\0');

    // Check value sanity and the variable-length fields.
    if (version > ARCHIVE_VERSION
	|| flags & 8			// FIXME: what's this?
	|| (method != STORED && method != DEFLATED)

	|| ! extract (m_input, &name [0], namelen, namelen)
	|| ! extract (m_input, &extra [0], extlen, extlen))
	throw ZError (Z_DATA_ERROR);
    m_total += namelen + extlen;

    // Create a new archive member.
    m_members.push_back (m_current = new ZipMember (name));

    m_current->method  	(ZConstants::Method (method));
    m_current->level  	(method == STORED ? 0
			 : (flags & 6)/2 == 0 ? 3 // default (:N)
			 : (flags & 6)/2 == 1 ? 9 // best (:X)
			 : (flags & 6)/2 == 2 ? 2 // fast (:F)
			 : (flags & 6)/2 == 3 ? 1 // fastest (:F)
			 : 3 /* unreached */);
    m_current->size    	(ZipMember::UNCOMPRESSED, usize);
    m_current->size    	(ZipMember::COMPRESSED,   csize);
    m_current->crc32   	(crc32);
    m_current->time   	(Time::fromDosDate (time));
    m_current->extra   	(ZipMember::LOCAL, &extra[0], extlen);
    m_current->position (offset);

    // Remember stuff about this member.
    m_magic        	= ENTRY_HEADER_MAGIC;
    m_compressed   	= csize;
    m_uncompressed 	= usize;

    // Return the new member.
    return m_current;
}

void
ZipInputStream::readDirectory (unsigned char *buf, IOSize length)
{
    ASSERT (length >= DIR_HEADER_SIZE);
    ASSERT (buf);

    unsigned n = DIR_HEADER_MAGIC;
    size_t i;

    // Record where the directory was found.
    m_directory = m_total;

    // Central directory entry; refill already seen members.
    for (i = 0; n == DIR_HEADER_MAGIC && i < m_members.size (); ++i)
    {
	// Create a few shortcuts.
	ZipMember *m      = m_members [i];
	IOOffset  usize   = m->size (ZipMember::UNCOMPRESSED);
	IOOffset  csize   = m->size (ZipMember::COMPRESSED);
	IOOffset  pos     = m->position ();
	IOSize    namelen = m->name ().size ();
	unsigned  time    = Time::toDosDate (m->time ());

	// Read the rest of the header.
	if (! extract (m_input, buf + ZIP_MAGIC_LEN, length,
		       DIR_HEADER_SIZE - ZIP_MAGIC_LEN))
	    throw ZError (Z_DATA_ERROR);
	m_total += DIR_HEADER_SIZE;

	// Check that we understand this stuff
	if (get   (buf + DIR_VREQD_OFF, DIR_VREQD_LEN) > ARCHIVE_VERSION

	    // Check that various fields match with the local entry
	    // FIXME: The flags don't always seem to match in real zip files.
	    // For example, for files compressed with maximum compression, the
	    // entry header says so, but the central directory says otherwise.
	    // || get(buf + DIR_FLAG_OFF,      DIR_FLAG_LEN)   	!= m->flags ()
	    || get(buf + DIR_METHOD_OFF,    DIR_METHOD_LEN) 	!= m->method ()
	    || get(buf + DIR_DATE_OFF,      DIR_DATE_LEN)   	!= time
    	       	  			    			
	    || get(buf + DIR_CRC32_OFF,     DIR_CRC32_LEN)  	!= m->crc32 ()

	    || csize < 0
	    || usize < 0
	    || IOOffset (IOSized (csize)) != csize
	    || IOOffset (IOSized (usize)) != usize
	    || get(buf + DIR_CSIZE_OFF,     DIR_CSIZE_LEN)  	!= IOSized (csize)
	    || get(buf + DIR_USIZE_OFF,     DIR_USIZE_LEN)  	!= IOSized (usize)
	       	  			    			 
	    || get(buf + DIR_NAMELEN_OFF,   DIR_NAMELEN_LEN)    != namelen
	       	  			    
	    || get(buf + DIR_DISK_START_OFF,DIR_DISK_START_LEN) != 0

	    || pos < 0
	    || IOOffset (IOSized (pos)) != pos
	    || get(buf + DIR_ENTRY_POS_OFF, DIR_ENTRY_POS_LEN)  != IOSized (pos))
	    throw ZError (Z_DATA_ERROR);

	// Read file name and check that it matches.
	n = get (buf + DIR_NAMELEN_OFF, DIR_NAMELEN_LEN);
	std::string name (n, '\0');
	if (! extract (m_input, &name [0], n, n) || name != m->name ())
	    throw ZError (Z_DATA_ERROR);
	m_total += n;

	// Read comment.
	n = get (buf + DIR_COMMENTLEN_OFF, DIR_COMMENTLEN_LEN);
	std::string comment (n, '\0');
	if (! extract (m_input, &comment [0], n, n))
	    throw ZError (Z_DATA_ERROR);
	m_total += n;

	// Read extra data.
	n = get (buf + DIR_EXTRALEN_OFF, DIR_EXTRALEN_LEN);
	std::vector<unsigned char> extra (n, '\0');
	if (! extract (m_input, &extra[0], n, n))
	    throw ZError (Z_DATA_ERROR);
	m_total += n;

	// Store the fields + internal and external file attributes
	m->comment    (comment);
	m->extra      (ZipMember::CENTRAL,  &extra[0], n);
	m->attributes (ZipMember::INTERNAL, get (buf + DIR_INT_ATTR_OFF,
						 DIR_INT_ATTR_LEN));
	m->attributes (ZipMember::EXTERNAL, get (buf + DIR_EXT_ATTR_OFF,
						 DIR_EXT_ATTR_LEN));

	// Done.  Read the next magic.
	if (! extract (m_input, buf, length, ZIP_MAGIC_LEN))
	    throw ZError (Z_DATA_ERROR);
	n = get (buf, ZIP_MAGIC_LEN);
    }

    // Should now see end of archive.
    if (n != END_HEADER_MAGIC || i != m_members.size ())
	throw ZError (Z_DATA_ERROR);

    readEndHeader (buf, length);
}

void
ZipInputStream::readEndHeader (unsigned char *buf, IOSize length)
{
    // Read the rest of the end header (including the archive comment)
    // so end-to-end trailing readers will work.  Check that it really
    // was a single-disk archive with all the entries as expected.
    ASSERT (length >= END_HEADER_SIZE);
    ASSERT (buf);

    if (! extract (m_input, buf + ZIP_MAGIC_LEN, length,
		   END_HEADER_SIZE - ZIP_MAGIC_LEN))
	throw ZError (Z_DATA_ERROR);

    size_t n = m_members.size ();
    if (get    (buf + END_DISK_OFF,       END_DISK_LEN)       != 0
	|| get (buf + END_DIR_DISK_OFF,   END_DIR_DISK_LEN)   != 0
	|| get (buf + END_DISK_HDRS_OFF,  END_DISK_HDRS_LEN)  != n
	|| get (buf + END_TOTAL_HDRS_OFF, END_TOTAL_HDRS_LEN) != n

	|| m_total < m_directory
	|| m_directory < 0
	|| IOOffset (IOSized (m_total)) != m_total
	|| IOOffset (IOSized (m_directory)) != m_directory
	|| get (buf + END_DIR_OFFSET_OFF, END_DIR_OFFSET_LEN) != IOSized (m_directory)
	|| get (buf + END_DIR_SIZE_OFF,   END_DIR_SIZE_LEN)   != IOSized (m_total -
									  m_directory))
	throw ZError (Z_DATA_ERROR);

    m_total += END_HEADER_SIZE;

    n = get (buf + END_COMMENTLEN_OFF, END_COMMENTLEN_LEN);
    std::string comment (n, '\0');
    if (! extract (m_input, &comment [0], n, n))
	throw ZError (Z_DATA_ERROR);
    m_total += n;
    m_comment = comment;

    // Indicate end of archive.
    m_magic = END_HEADER_MAGIC;
}

/** Skip to the end of the current member and position the stream to
    be ready to read the next archive member.  */
void
ZipInputStream::closeMember (void)
{
    ASSERT (! m_dummy || (! m_compressed && ! m_uncompressed));
    ASSERT (IOOffset (IOSized (m_compressed)) == m_compressed);
    ASSERT (IOOffset (IOSized (m_uncompressed)) == m_uncompressed);
    VERIFY (IOOffset (m_input->skip (m_compressed)) == m_compressed);
    m_total += m_compressed;
    resetState (false);
}

/** Synchronise stream after data error.

    @return The next archive member if one is found (equivalent to
	    nextMember()).  If the central directory or the archive
	    end marker is found, returns zero to indicate end of input
	    stream; the directory and the end comment will have been
	    read in that case.

    @throw  ZError if no readable header is found; in that case the
            input stream will have been read to the end.  */
ZipMember *
ZipInputStream::sync (void)
{
    // FIXME: Allow maximum read limit to bound how far to look for?
    // Should we take into account how much data is supposed to be
    // left of the current member (if any)?

    unsigned char buf [DIR_HEADER_SIZE];
    unsigned	  n;

    // Reset state to something sane in case we get resynced because
    // of a corrupted member data stream.  Unset m_dummy to avoid
    // interference with somewhat unknown state.
    m_dummy = false;
    resetState (false);

    // Keep looking until we can find something recognisable and can
    // actually make sense of the header that follows.  Just finding
    // the magic code doesn't prove the header is valid, so if doing
    // the header fails, just assume it was a bad guess.
    //
    // Note though that reading the directory may result in an error
    // as the number of entries read from the archive and those found
    // in the directory may well be different if the corrupted region
    // included entries in the middle or part of the directory.  In
    // that case we will eat the error, and re-process the end header.
    //
    // At the end we read off the archive comment, which probably
    // isn't much use to rescue, but oh well...
    //
    // We indicate error only if we can find nothing; we will have
    // read to the end of input stream.
    while (true)
    {
	try
	{
	    if ((n = findHeader ()) == ENTRY_HEADER_MAGIC)
		return readMemberHeader (buf, sizeof (buf));
	    else if (n == DIR_HEADER_MAGIC)
	    {
		readDirectory (buf, sizeof (buf));
		return  0;
	    }
	    else if (n == END_HEADER_MAGIC)
	    {
		readEndHeader (buf, sizeof (buf));
		return 0;
	    }
	}
	catch (ZError &)
	{
	    // Keep looking.
	    continue;
	}

	// Found nothing.
	ASSERT (n == 0);
	throw ZError (Z_DATA_ERROR);
    }
}

} // namespace lat
