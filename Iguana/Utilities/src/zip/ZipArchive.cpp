//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/ZipArchive.h"
#include "classlib/zip/ZipMember.h"
#include "classlib/zip/ZError.h"
#include "classlib/zip/ZInputStream.h"
#include "classlib/zip/ZOutputStream.h"
#include "classlib/zip/ZDecompressor.h"
#include "classlib/zip/ZCompressor.h"
#include "classlib/zip/CheckedOutputStream.h"
#include "classlib/zip/CRC32.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iotools/StorageInputStream.h"
#include "classlib/iotools/StorageOutputStream.h"
#include "classlib/iotools/SubStorage.h"
#include "classlib/utils/Time.h"
#include <algorithm>
#include <cstring>
#include <zlib.h> // FIXME: Why do we need Z_DATA_ERROR?

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

//////////////////////////////////////////////////////////////////////
class ZipArchive::MemberByName
{
    Filename m_name;
public:
    MemberByName (Filename name) : m_name (name) {}
    bool operator() (ZipMember *member) {
	return member->name () == m_name;
    }
};

//////////////////////////////////////////////////////////////////////
class ZipArchive::ZInput : public ZInputStream
{
    IOOffset m_size;	//< Real input data left
    bool     m_dummy;	//< Dummy byte for ZLIB to finish
public:
    ZInput (InputStream *input, ZipMember *m)
	: ZInputStream (input, new ZDecompressor),
	  m_size (m->size (ZipMember::COMPRESSED)),
	  m_dummy (false)
	{}

    virtual IOSize available (void) {
	// NB: We know the decompressor always decompresses all
	// available compressed input.  (FIXME: Do we really?)
	return m_size && m_input->available ();
    }

    // Don't mess with the underlying file
    virtual void close (void) {}

protected:
    virtual IOSize fill (void *buffer, IOSize length) {
	if (! (length = std::min (IOSized (m_size), length)))
        {
	    // See ZipInputStream for description of this gymnastics
	    if (! m_dummy)
	    {
		m_dummy = true;
		*(char *) buffer = 0;
		return 1;
	    }
	    else
		throw ZError (Z_DATA_ERROR);
        }

	length = ZInputStream::fill (buffer, length);
	m_size -= length;
	return length;
    }
};

//////////////////////////////////////////////////////////////////////
class ZipArchive::SInput : public FilterInputStream
{
    IOOffset m_size;	//< Real input data left
public:
    SInput (InputStream *input, ZipMember *m)
	: FilterInputStream (input),
	  m_size (m->size (ZipMember::UNCOMPRESSED))
	{}

    virtual IOSize available (void) {
	return m_size && m_input->available ();
    }

    virtual IOSize read (void *into, IOSize n) {
	n = FilterInputStream::read (into, std::min (n, IOSized (m_size)));
	m_size -= n;
	return n;
    }

    virtual IOSize skip (IOSize n) {
	n = FilterInputStream::skip (std::min (n, IOSized (m_size)));
	m_size -= n;
	return n;
    }

    // Don't mess with the underlying file
    virtual void close (void) {}
};

//////////////////////////////////////////////////////////////////////
class ZipArchive::ZOutput : public ZOutputStream
{
    ZipArchive	*m_archive;	//< The archive
    ZipMember	*m_member;	//< The member
    bool	m_finished;	//< Flag indicating finished() was called
public:
    ZOutput (OutputStream *output, ZipArchive *a, ZipMember *m)
	: ZOutputStream (output, new ZCompressor (m->level ())),
	  m_archive (a),
	  m_member (m),
	  m_finished (false)
	{}

    virtual void finish (void) {
	if (m_finished)
	    return;

	ZOutputStream::finish ();
	m_member->size (ZipMember::COMPRESSED, m_compressor->out ());
	m_member->size (ZipMember::UNCOMPRESSED, m_compressor->in ());
	m_member->crc32 (checksum ());
	m_archive->updateMember (m_member);
	m_finished = true;
    }

    // Finish but don't mess with the underlying file
    virtual void close (void) { finish (); }
};

//////////////////////////////////////////////////////////////////////
class ZipArchive::SOutput : public CheckedOutputStream
{
    ZipArchive	*m_archive;	//< The archive
    ZipMember	*m_member;	//< The member
    IOOffset	m_size;		//< Real input data left
    bool	m_finished;	//< Flag indicating finished() was called
public:
    SOutput (OutputStream *output, ZipArchive *a, ZipMember *m)
	: CheckedOutputStream (output, new CRC32),
	  m_archive (a),
	  m_member (m),
	  m_size (0),
	  m_finished (false)
	{}

    virtual IOSize write (const void *from, IOSize n) {
	n = CheckedOutputStream::write (from, n);
	m_size += n;
	return n;
    }

    virtual void finish (void) {
	if (m_finished)
	    return;

	CheckedOutputStream::finish ();
	m_member->size (ZipMember::COMPRESSED, m_size);
	m_member->size (ZipMember::UNCOMPRESSED, m_size);
	m_member->crc32 (checksum ()->value ());
	m_archive->updateMember (m_member);
	m_finished = true;
    }

    // Finish but don't mess with the underlying file
    virtual void close (void) { finish (); }
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ZipArchive::ZipArchive (Storage *file)
    : m_file (file),
      m_method (DEFLATED),
      m_level (3),
      m_prefixSize (0),
      m_dirPos (0),
      m_dirSize (0),
      m_dirOffset (0),
      m_mode (IOFlags::OpenRead),
      m_current (0)
{ openArchive (); }

ZipArchive::ZipArchive (Filename name, unsigned mode)
    : m_file (new File (name, mode)),
      m_method (DEFLATED),
      m_level (3),
      m_prefixSize (0),
      m_dirPos (0),
      m_dirSize (0),
      m_dirOffset (0),
      m_mode (mode),
      m_current (0)
{ ASSERT (m_mode & (IOFlags::OpenRead | IOFlags::OpenWrite)); openArchive (); }

ZipArchive::~ZipArchive (void)
{ closeArchive (); }

//////////////////////////////////////////////////////////////////////
std::string
ZipArchive::comment (void) const
{ ASSERT (m_file); return m_comment; }

void
ZipArchive::comment (const std::string &text)
{ ASSERT (m_file); ASSERT (m_mode & IOFlags::OpenWrite); m_comment = text; }

//////////////////////////////////////////////////////////////////////
size_t
ZipArchive::size (void) const
{ ASSERT (m_file); return m_members.size (); }

ZipArchive::Iterator
ZipArchive::begin (void) const
{ ASSERT (m_file); return m_members.begin (); }

ZipArchive::Iterator
ZipArchive::end (void) const
{ ASSERT (m_file); return m_members.end (); }

ZipArchive::Iterator
ZipArchive::find (Filename name) const
{
    ASSERT (m_file);
    return std::find_if (m_members.begin(), m_members.end(),
			 MemberByName (name));
}

ZipMember *
ZipArchive::member (Filename name) const
{
    Iterator pos = find (name);
    return pos == end () ? 0 : *pos;
}

//////////////////////////////////////////////////////////////////////
// Caller should delete the stream when done with it.
InputStream *
ZipArchive::input (ZipMember *member)
{
    ASSERT (m_file);
    ASSERT ((m_mode & (IOFlags::OpenRead | IOFlags::OpenWrite)) == IOFlags::OpenRead);
    ASSERT (member);
    ASSERT (std::find (m_members.begin (), m_members.end (), member)
	    != m_members.end ());

    // Seek to member position.
    m_file->position (member->position ());

    // Read the entry header and verify its consistency.
    // FIXME: share code with ZipInputStream?
    // FIXME: memory leaks here on throw, delete input!
    typedef std::vector<unsigned char> databuf;
    InputStream		*input = new StorageInputStream (m_file);
    unsigned char	buf [ENTRY_HEADER_SIZE];

    if (! extract (input, buf, sizeof (buf), sizeof (buf)))
	throw ZError (Z_DATA_ERROR);

    unsigned	version	= get (buf + ENTRY_VREQD_OFF,  	 ENTRY_VREQD_LEN);
    // unsigned	flags	= get (buf + ENTRY_FLAG_OFF,   	 ENTRY_FLAG_LEN);
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
	// FIXME: mismatch? || flags   != member->flags ()
	|| method  != member->method ()
	|| time    != Time::toDosDate (member->time ())
	|| crc32   != member->crc32 ()
	|| csize   < 0
	|| usize   < 0
	|| csize   != member->size (ZipMember::COMPRESSED)
	|| usize   != member->size (ZipMember::UNCOMPRESSED)
	|| namelen != member->name ().size ()
	|| ! extract (input, &name [0], namelen, namelen)
	|| name    != member->name ()
	|| ! extract (input, &extra [0], extlen, extlen))
	throw ZError (Z_DATA_ERROR);

    // Provide input to this member.
    return (method == DEFLATED
	    ? static_cast <FilterInputStream *> (new ZInput (input, member))
	    : static_cast <FilterInputStream *> (new SInput (input, member)));
}

// Caller should close and delete the stream when done with it
OutputStream *
ZipArchive::output (ZipMember *member)
{
    // If a previous member is still incomplete, force it completed.
    // This leaves us in a right position to begin the next header.
    ASSERT (m_file);
    ASSERT ((m_mode & (IOFlags::OpenRead | IOFlags::OpenWrite)) == IOFlags::OpenWrite);
    ASSERT (member);
    ASSERT (std::find (m_members.begin (), m_members.end (), member)
	    == m_members.end ());

    if (m_current)
	m_current->finish ();

    ASSERT (! m_current);

    // Write the preliminary entry header.
    OutputStream	*output = new StorageOutputStream (m_file);
    unsigned char	buf [ENTRY_HEADER_SIZE];
    unsigned		time      = Time::toDosDate (member->time ());
    IOSize		lextlen   = 0;
    IOSize		gextlen   = 0;
    void		*extra   = 0;

    member->extra (ZipMember::CENTRAL, &extra, &gextlen);
    member->extra (ZipMember::LOCAL, &extra, &lextlen);

    // Check that we don't have too many members and that the member is valid.
    ASSERT (m_members.size () < MAX_VAR_LEN);
    ASSERT (member->name().size () && member->name().size () <= MAX_VAR_LEN);
    ASSERT (member->comment ().length () <= MAX_VAR_LEN);
    ASSERT (lextlen <= MAX_VAR_LEN);
    ASSERT (gextlen <= MAX_VAR_LEN);
    ASSERT (member->method () == DEFLATED || member->method () == STORED);

    // Make this the current member.  Set undefined parameters.
    m_members.push_back (member);
    if (member->method () == ZipMember::NONE)
	member->method (m_method);
    if (member->method () == DEFLATED && member->level () == 0)
	member->level (m_level);
    if (! member->time ())
	member->time (Time::current ().ns () / TimeConst::SEC_NSECS);

    // Update member data for central directory
    member->position (m_file->position ());

    // Put in the field values.  When the member output is completed,
    // updateHeader will update the CRC32 and size fields.
    put (buf + ENTRY_MAGIC_OFF,    ZIP_MAGIC_LEN,      ENTRY_HEADER_MAGIC);
    put (buf + ENTRY_VREQD_OFF,    ENTRY_VREQD_LEN,    ARCHIVE_VERSION);
    put (buf + ENTRY_FLAG_OFF,     ENTRY_FLAG_LEN,     member->flags ());
    put (buf + ENTRY_METHOD_OFF,   ENTRY_METHOD_LEN,   member->method ());
    put (buf + ENTRY_DATE_OFF,     ENTRY_DATE_LEN,     time);
    put (buf + ENTRY_CRC32_OFF,    ENTRY_CRC32_LEN,    0);
    put (buf + ENTRY_CSIZE_OFF,    ENTRY_CSIZE_LEN,    0);
    put (buf + ENTRY_USIZE_OFF,    ENTRY_USIZE_LEN,    0);
    put (buf + ENTRY_NAMELEN_OFF,  ENTRY_NAMELEN_LEN,  member->name ().size());
    put (buf + ENTRY_EXTRALEN_OFF, ENTRY_EXTRALEN_LEN, lextlen);

    // Write out the header and variable length fields.
    ASSERT (IOOffset (MAX_SIZE - sizeof(buf) - member->name().size() - lextlen)
	    >= m_file->position ());
    m_file->xwrite (buf, sizeof (buf));
    m_file->xwrite (member->name (), member->name ().size ());
    m_file->xwrite (extra, lextlen);

    // Return output stream to this member.
    if (member->method () == DEFLATED)
	m_current = new ZOutput (output, this, member);
    else
	m_current = new SOutput (output, this, member);

    return m_current;
}

// Caller should delete the storage when done with it
// This is like read but allows clients to access stored
// (and only stored!) members as seekable storage objects.
Storage *
ZipArchive::openStored (ZipMember *member)
{
    ASSERT (m_file);
    ASSERT ((m_mode & (IOFlags::OpenRead | IOFlags::OpenWrite)) == IOFlags::OpenRead);
    ASSERT (member);
    ASSERT (std::find (m_members.begin (), m_members.end (), member)
	    != m_members.end ());

    // Seek to member position.
    m_file->position (member->position ());

    // Read the entry header and verify its consistency.
    // FIXME: share code with ZipInputStream?
    // FIXME: memory leaks here on throw, delete input!
    typedef std::vector<unsigned char> databuf;
    StorageInputStream	*input = new StorageInputStream (m_file);
    unsigned char	buf [ENTRY_HEADER_SIZE];

    if (! extract (input, buf, sizeof (buf), sizeof (buf)))
	throw ZError (Z_DATA_ERROR);

    unsigned	version	= get (buf + ENTRY_VREQD_OFF,  	 ENTRY_VREQD_LEN);
    // unsigned	flags	= get (buf + ENTRY_FLAG_OFF,   	 ENTRY_FLAG_LEN);
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
	// FIXME: mismatch? || flags   != member->flags ()
	|| method  != member->method ()
	|| time    != Time::toDosDate (member->time ())
	|| crc32   != member->crc32 ()
	|| csize   < 0
	|| usize   < 0
	|| csize   != member->size (ZipMember::COMPRESSED)
	|| usize   != member->size (ZipMember::UNCOMPRESSED)
	|| namelen != member->name ().size ()
	|| ! extract (input, &name [0], namelen, namelen)
	|| name    != member->name ()
	|| ! extract (input, &extra [0], extlen, extlen))
	throw ZError (Z_DATA_ERROR);

    // Provide input to this member.
    delete input;
    ASSERT (method == STORED);
    return openStored (m_file, m_file->position (), csize);
}

Storage *
ZipArchive::openStored (Storage *master, IOOffset position, IOOffset length)
{ return new SubStorage (master, position, length); }

//////////////////////////////////////////////////////////////////////
void
ZipArchive::flush (void)
{
    ASSERT (m_file);
    ASSERT ((m_mode & (IOFlags::OpenRead | IOFlags::OpenWrite)) == IOFlags::OpenWrite);

    if (m_current)
	// FIXME: finish or flush?
	m_current->finish ();
}

void
ZipArchive::close (void)
{ closeArchive (); }

//////////////////////////////////////////////////////////////////////
bool
ZipArchive::verify (void)
{
    // FIXME
    return true;
}

//////////////////////////////////////////////////////////////////////
void
ZipArchive::openArchive (void)
{
    if (m_mode & IOFlags::OpenRead)
    {
	readEndHeader (findEndHeader ());
	readDirectory ();
    }
}


IOOffset
ZipArchive::findEndHeader (void)
{
    static const IOOffset BUFSIZE = 1024;
    IOOffset		size = m_file->size ();
    IOOffset		limit = std::min (size, IOOffset (MAX_VAR_LEN));
    unsigned char	buf [BUFSIZE+4];

    // NB: This works correctly even if the signature straddles read
    // boundaries since we always read an overlapped area of four
    // bytes on the next read.
    for (IOOffset offset = 4; offset < limit; )
    {
	offset = std::min (offset + BUFSIZE, limit);

	IOOffset pos = size - offset;
	IOSize   n   = std::min (BUFSIZE+4, offset);
	if (m_file->read (buf, n, pos) != n)
	    return 0;

	for (int i = n - 4; i > 0; --i)
	    if (buf[i] == 0x50 && buf[i+1] == 0x4b
		&& buf[i+2] == 0x05 && buf[i+3] == 0x06)
		return pos + i;
    }

    return 0;
}

void
ZipArchive::readEndHeader (IOOffset pos)
{
    // Read the end header (including the archive comment) at the
    // current file position.  Check that it really was a single-disk
    // archive with all the entries as expected.  Most importantly,
    // figure out where the central directory begins.
    unsigned char buf [DIR_HEADER_SIZE];

    // Read and validate first the header magic.
    m_file->position (pos);
    if (! extract (m_file, buf, ZIP_MAGIC_LEN, ZIP_MAGIC_LEN)
	|| get (buf, ZIP_MAGIC_LEN) != END_HEADER_MAGIC)
	throw ZError (Z_DATA_ERROR);

    // Now read the rest of the header.
    if (! extract (m_file, buf + ZIP_MAGIC_LEN,
		   sizeof (buf) - ZIP_MAGIC_LEN,
		   END_HEADER_SIZE - ZIP_MAGIC_LEN))
	throw ZError (Z_DATA_ERROR);

    unsigned	disk	= get (buf + END_DISK_OFF,    	 END_DISK_LEN);
    unsigned	dirdisk	= get (buf + END_DIR_DISK_OFF, 	 END_DIR_DISK_LEN);
    unsigned	dhdrs	= get (buf + END_DISK_HDRS_OFF,  END_DISK_HDRS_LEN);
    unsigned	thdrs	= get (buf + END_TOTAL_HDRS_OFF, END_TOTAL_HDRS_LEN);
    IOOffset	diroff	= get (buf + END_DIR_OFFSET_OFF, END_DIR_OFFSET_LEN);
    IOOffset	dirsz	= get (buf + END_DIR_SIZE_OFF,	 END_DIR_SIZE_LEN);
    IOSize	commlen	= get (buf + END_COMMENTLEN_OFF, END_COMMENTLEN_LEN);
    std::string	comment (commlen, '\0');

    if (disk != 0 || dirdisk != 0 || dhdrs != thdrs
	|| diroff + dirsz != pos
	|| ! extract (m_file, &comment [0], commlen, commlen))
	throw ZError (Z_DATA_ERROR);

    m_comment = comment;

    m_dirOffset = m_dirPos = diroff;
    m_dirSize = dirsz;
}

void
ZipArchive::readDirectory (void)
{
    // Read members.
    unsigned char buf [DIR_HEADER_SIZE];
    unsigned	  n = DIR_HEADER_MAGIC;
    unsigned	  i;

    // Read and validate first the header magic.
    m_file->position (m_dirPos);
    if (! extract (m_file, buf, ZIP_MAGIC_LEN, ZIP_MAGIC_LEN)
	|| (n = get (buf, ZIP_MAGIC_LEN)) != DIR_HEADER_MAGIC)
	throw ZError (Z_DATA_ERROR);

    // Now read the full directory.
    for (i = 0; n == DIR_HEADER_MAGIC; ++i)
    {
	// Read the rest of the header.
	if (! extract (m_file, buf + ZIP_MAGIC_LEN,
		       sizeof (buf) - ZIP_MAGIC_LEN,
		       DIR_HEADER_SIZE - ZIP_MAGIC_LEN))
	    throw ZError (Z_DATA_ERROR);

	typedef std::vector<unsigned char> databuf;
	unsigned	version	= get (buf + DIR_VREQD_OFF,    	 DIR_VREQD_LEN);
	unsigned	flags	= get (buf + DIR_FLAG_OFF,     	 DIR_FLAG_LEN);
	unsigned	method	= get (buf + DIR_METHOD_OFF,   	 DIR_METHOD_LEN);
	unsigned	time	= get (buf + DIR_DATE_OFF,     	 DIR_DATE_LEN);
	unsigned	crc32	= get (buf + DIR_CRC32_OFF,    	 DIR_CRC32_LEN);
	IOOffset	csize	= get (buf + DIR_CSIZE_OFF,    	 DIR_CSIZE_LEN);
	IOOffset	usize	= get (buf + DIR_USIZE_OFF,    	 DIR_USIZE_LEN);
	IOSize		namelen	= get (buf + DIR_NAMELEN_OFF,  	 DIR_NAMELEN_LEN);
	IOSize		extlen	= get (buf + DIR_EXTRALEN_OFF,   DIR_EXTRALEN_LEN);
	IOSize		commlen	= get (buf + DIR_COMMENTLEN_OFF, DIR_COMMENTLEN_LEN);
	unsigned	disk    = get (buf + DIR_DISK_START_OFF, DIR_DISK_START_LEN);
	unsigned	iattr   = get (buf + DIR_INT_ATTR_OFF,   DIR_INT_ATTR_LEN);
	unsigned	xattr   = get (buf + DIR_EXT_ATTR_OFF,   DIR_EXT_ATTR_LEN);
	IOOffset	offset  = get (buf + DIR_ENTRY_POS_OFF,  DIR_ENTRY_POS_LEN);
	std::string	name    (namelen, '\0');
	std::string	comment (commlen, '\0');
	databuf		extra   (extlen,  '\0');

	// Check value sanity and the variable-length fields.
	if (get (buf + DIR_MAGIC_OFF, ZIP_MAGIC_LEN) != DIR_HEADER_MAGIC
	    || version > ARCHIVE_VERSION
	    || flags & 8			// FIXME: what's this?
	    || (method != STORED && method != DEFLATED)
	    || disk != 0
	    || csize < 0
	    || usize < 0
	    || IOOffset (IOSized (csize)) != csize
	    || IOOffset (IOSized (usize)) != usize

	    || ! extract (m_file, &name [0], namelen, namelen)
	    || ! extract (m_file, &extra [0], extlen, extlen)
	    || ! extract (m_file, &comment [0], commlen, commlen))
	    throw ZError (Z_DATA_ERROR);

	// Create a new archive member and store the fields.
	ZipMember *m = new ZipMember (name);
	m_members.push_back (m);

	m->method     (ZConstants::Method (method));
	m->level      (method == STORED ? 0
	 	       : (flags & 6)/2 == 0 ? 3	// default (:N)
	 	       : (flags & 6)/2 == 1 ? 9 // best (:X)
	 	       : (flags & 6)/2 == 2 ? 2 // fast (:F)
	 	       : (flags & 6)/2 == 3 ? 1 // fastest (:F)
	 	       : 3 /* unreached */);
	m->size       (ZipMember::UNCOMPRESSED, usize);
	m->size       (ZipMember::COMPRESSED,   csize);
	m->crc32      (crc32);
	m->time       (Time::fromDosDate (time));
	m->extra      (ZipMember::CENTRAL, &extra[0], extlen);
	m->comment    (comment);
	m->attributes (ZipMember::INTERNAL, iattr);
	m->attributes (ZipMember::EXTERNAL, xattr);
	m->position   (offset);

	// Done.  Read the next magic.
	if (! extract (m_file, buf, sizeof (buf), ZIP_MAGIC_LEN))
	    throw ZError (Z_DATA_ERROR);
	n = get (buf, ZIP_MAGIC_LEN);
    }

    // Should now see end of archive.
    if (n != END_HEADER_MAGIC
	|| m_file->position () - m_dirPos != m_dirSize + ZIP_MAGIC_LEN)
	throw ZError (Z_DATA_ERROR);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
ZipArchive::closeArchive (void)
{
    if (m_mode & IOFlags::OpenWrite)
	writeDirectory ();

    if (m_mode)
	m_file->close ();

    m_mode = 0;
    // FIXME: m_file = 0;
}

void
ZipArchive::writeDirectory (void)
{
    // Cowardly refuse to write an empty archive.
    ASSERT (! m_members.empty ());

    // m_file->seek (0, Storage::END);
    IOOffset		diroffset = m_file->position ();
    unsigned char	buf [DIR_HEADER_SIZE];

    ASSERT (diroffset <= IOOffset (MAX_SIZE));

    // Write the central directory.
    for (size_t i = 0; i < m_members.size (); ++i)
    {
	ZipMember *m = m_members [i];

	// Prepare the header.
	memset (buf, 0, sizeof (buf));

	// Get the member values.  (FIXME: time format?)
	std::string	name	 (m->name ());
	std::string	comment	 (m->comment ());
	unsigned	time     = Time::toDosDate (m->time ());
	unsigned	method   = m->method ();
	unsigned	flags    = m->flags ();
	IOOffset	csize    = m->size (ZipMember::COMPRESSED);
	IOOffset	usize    = m->size (ZipMember::UNCOMPRESSED);
	unsigned	crc32    = m->crc32 ();
	unsigned	intattr  = m->attributes (ZipMember::INTERNAL);
	unsigned	extattr  = m->attributes (ZipMember::EXTERNAL);
	IOOffset	offset   = m->position();
	IOSize		extlen   = 0;
	void		*extra   = 0;

	ASSERT (IOOffset (IOSized (csize)) == csize);
	ASSERT (IOOffset (IOSized (usize)) == usize);

	m->extra (ZipMember::CENTRAL, &extra, &extlen);

	// Check that no value is larger than field allocated to it
	// and things look otherwise valid.
	ASSERT (name.size () && name.size () <= MAX_VAR_LEN);
	ASSERT (comment.length () <= MAX_VAR_LEN);
	ASSERT (extlen <= MAX_VAR_LEN);

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
			  - extlen - comment.length ())
		>= m_file->position ());
	m_file->xwrite (buf, DIR_HEADER_SIZE);
	m_file->xwrite (name.data (), name.size ());
	m_file->xwrite (extra, extlen);
	m_file->xwrite (comment.data (), comment.length ());
    }

    // Write the end-of-archive marker.
    ASSERT (m_comment.length () <= MAX_VAR_LEN);
    memset (buf, 0, sizeof (buf));

    ASSERT (m_file->position () >= 0);
    ASSERT (m_file->position () >= diroffset);
    ASSERT (IOOffset (IOSized (m_file->position ())) == m_file->position ());
    ASSERT (IOOffset (IOSized (diroffset)) == diroffset);
    put (buf + END_MAGIC_OFF,      ZIP_MAGIC_LEN,      END_HEADER_MAGIC);
    put (buf + END_DISK_OFF,       END_DISK_LEN,       0);
    put (buf + END_DIR_DISK_OFF,   END_DIR_DISK_LEN,   0);
    put (buf + END_DISK_HDRS_OFF,  END_DISK_HDRS_LEN,  m_members.size ());
    put (buf + END_TOTAL_HDRS_OFF, END_TOTAL_HDRS_LEN, m_members.size ());
    put (buf + END_DIR_SIZE_OFF,   END_DIR_SIZE_LEN,   IOSized (m_file->position ()
								- diroffset));
    put (buf + END_DIR_OFFSET_OFF, END_DIR_OFFSET_LEN, IOSized (diroffset));
    put (buf + END_COMMENTLEN_OFF, END_COMMENTLEN_LEN, m_comment.length ());

    ASSERT (IOOffset (MAX_SIZE - sizeof (buf) - m_comment.length ())
	    >= m_file->position ());
    m_file->xwrite (buf, END_HEADER_SIZE);
    m_file->xwrite (m_comment.data (), m_comment.length ());
}

/** Update the entry header for member after its data has been
    fully written.  */
void
ZipArchive::updateMember (ZipMember *member)
{
    ASSERT (m_file);
    ASSERT ((m_mode & (IOFlags::OpenRead | IOFlags::OpenWrite)) == IOFlags::OpenWrite);
    ASSERT (m_current);
    ASSERT (member);
    ASSERT (std::find (m_members.begin (), m_members.end (), member)
	    == m_members.end () - 1);

    // Buffer the member values we will update.  Then update header
    // already in the file, and seek back to where we are now.  This
    // leaves the output ready for the next member or the central
    // directory. (We know the three updated fields are consequtive.)
    unsigned char	buf [ENTRY_HEADER_SIZE];
    IOOffset		csize = member->size (ZipMember::COMPRESSED);
    IOOffset		usize = member->size (ZipMember::UNCOMPRESSED);
    unsigned		crc32 = member->crc32 ();

    ASSERT (member->method () != STORED || csize == usize);
    ASSERT (IOOffset (IOSized (csize)) == csize);
    ASSERT (IOOffset (IOSized (usize)) == usize);
    put (buf + ENTRY_CRC32_OFF, ENTRY_CRC32_LEN, crc32);
    put (buf + ENTRY_CSIZE_OFF, ENTRY_CSIZE_LEN, IOSized (csize));
    put (buf + ENTRY_USIZE_OFF, ENTRY_USIZE_LEN, IOSized (usize));

    m_file->write (buf + ENTRY_CRC32_OFF,
		   ENTRY_CRC32_LEN+ENTRY_CSIZE_LEN+ENTRY_USIZE_LEN,
		   member->position () + ENTRY_CRC32_OFF);

    // Zero our current pointer but don't delete the object -- the
    // client is suppose to do so (FIXME: optimise management!)
    m_current = 0;
}

} // namespace lat
