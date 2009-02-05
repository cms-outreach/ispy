#ifndef CLASSLIB_ARCHIVE_H
# define CLASSLIB_ARCHIVE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/ZConstants.h"
# include "classlib/iobase/IOBuffer.h"
# include <vector>
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Storage;
class Filename;
class ZipMember;
class InputStream;
class OutputStream;
class MemoryStorage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Read and write members in a ZIP archive.

    A ZIP archive consists of files compressed with the popular ZLIB
    compression algorithm.  The archive format is used among others by
    PKZip and Info-ZIP.  The compression algorithm is also used by
    GZIP and the PNG graphics standard.  The format of the archives is
    explained briefly below.  This class provides an interface to read
    and write such archives.

    A ZIP archive contains a prefix, series of archive members
    (files), and a central directory.  In theory the archive could
    span multiple disks (or files) with the central directory of the
    whole archive on the last disk, but this package does not support
    such multi-part archives.  The prefix is only used in
    self-extracting executable archive files.

    The member are stored in the archive sequentially, each with a
    local header followed by the compressed data; the local header
    describes the member, including its file name and compressed and
    real sizes.  The central directory includes the member details
    again, plus allows an extra member comment to be added.  The last
    member in the central directory is an end marker that can contain
    a comment for the whole archive.  Both the local header and the
    central directory can also carry extra member-specific data; the
    data in the local and global parts can be different.

    Once the archive has been opened, the client can query the members
    and read their contents by asking the archive for an input stream.
    The members can be accessed in any order, however only one member
    at a time should be read from.  Trying to read data from more than
    one member at a time will confuse the internal file state.
    (FIXME: attempting to seek the underlying file storage behind the
    scenes will have similar impact.)

    Archive writes are possible only in constrained sequential write
    of all members in one go.  First create a @ZipArchive in write
    mode.  Next create a new @ZipMember, fully describing it in the
    constructor.  Then retrieve the compressing output stream from the
    archive and write all the member file data to the stream.  Once
    complete, close the stream, and repeat the whole process for
    subsequent files.  Finally, the close or at least flush the
    archive itself.  The writing is not thread safe, nor is it
    permitted to read from already-written members while writing.

    The @ZipArchive owns all the @ZipMember objects it returns as
    well as those added to it for write.

    @sa ZipInputStream, ZipOutputStream for simple stream-oriented
    reading and writing of ZIP archives.  */
class ZipArchive : public ZConstants
{
public:
    typedef std::vector<ZipMember *>::const_iterator Iterator;

    // FIXME: file can be an inmemory file, e.g. compiled-in string!
    ZipArchive (Storage *file);
    ZipArchive (Filename name, unsigned mode);
    virtual ~ZipArchive (void);

    // FIXME: version made, required
    std::string		comment (void) const;
    void		comment (const std::string &text);

    size_t		size (void) const;
    Iterator		begin (void) const;
    Iterator		end (void) const;
    Iterator		find (Filename name) const; // FIXME: case?
    ZipMember *		member (Filename name) const;  // FIXME: case?

    // FIXME: (CheckedInputStream ->) InflaterInputSream -> bounded
    // FileStorageInputStream
    InputStream *	input (ZipMember *member);
    OutputStream *	output (ZipMember *member);
    Storage *		openStored (ZipMember *member);

    // NB: on file close, finish writing last file, write out
    // zip member headers again, write out end header.
    void		flush (void);
    void		close (void);

    // Other operations
    bool		verify (void);

protected:
    virtual Storage *	openStored (Storage *master,
				    IOOffset position,
				    IOOffset length);

private:
    class MemberByName;
    class ZInput;
    class SInput;
    class ZOutput;
    class SOutput;
    friend class ZInput;
    friend class SInput;
    friend class ZOutput;
    friend class SOutput;

    void		openArchive (void);
    IOOffset		findEndHeader (void);
    void		readEndHeader (IOOffset pos);
    void		readDirectory (void);
    ZipMember *		readMember (void);

    void		closeArchive (void);
    void		writeDirectory (void);
    
    bool		verifyMember (void);
    void		updateMember (ZipMember *member);

    Storage		*m_file;	//< The archive
    Method		m_method;	//< Default compression method
    unsigned		m_level;	//< Default compression level
    std::string		m_comment;	//< Archive comment
    IOOffset		m_prefixSize;	//< Size of the archive prefix (SFX)
    IOOffset		m_dirPos;	//< Central directory position
    IOOffset		m_dirSize;	//< Central directory size
    IOOffset		m_dirOffset;	//< Central directory offset (from
					//  the beginning of the archive,
					//  which could be multi-file)
    unsigned		m_mode;		/**< Current open mode.  Always has one
					     of File::ModeRead, File::ModeWrite
					     set (exactly one of them), except
					     that internally the whole mode is
					     set to zero to indicate closed
					     archive.  (Flags other than those
					     two can be set by the client.) */
    std::vector<ZipMember *> m_members;	//< Members in this archive
    OutputStream	*m_current;	//< Stream for current member output

    // undefined, no semantics
    ZipArchive (const ZipArchive &);
    const ZipArchive &operator= (const ZipArchive &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_ARCHIVE_H
