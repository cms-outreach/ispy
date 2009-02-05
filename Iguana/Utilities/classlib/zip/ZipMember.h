#ifndef CLASSLIB_MEMBER_H
# define CLASSLIB_MEMBER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/ZConstants.h"
# include "classlib/iobase/Filename.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A ZIP archive member.

    A ZIP archive consists of files compressed with the popular ZLIB
    compression algorithm; this class records the information about a
    single archive member.  Please note that it is not possible to
    read or write the file data through this class, use the archive
    file or stream interface instead: @ZipInputStream,
    @ZipOutputStream or @ZipArchive.  */
class ZipMember : public ZConstants
{
public:
    // FIXME: Check that name is <256 characters.

    // FIXME: Check that extra data and comment is <65536 bytes.

    // FIXME: File format restricts file data length to <32bits.

    // FIXME: CRC-32 and data length (compressed and real) are valid
    // only on reading, or once the member output stream is closed on
    // write.

    // FIXME: Comment, global extra field and file attributes are
    // valid only when read from ZipArchive, or if they were given on
    // member creation for write, or once ZipInputStream has reached
    // the end of the archive (at which point already seen ZipMembers
    // will be completed with the central directory info).

    // FIXME: Not all the information is available in one place -- it
    // must be merged from central and local headers.  ZipArchive must
    // read both on archive open, as well as fetch all the comment and
    // extra data fields even if they are never accessed; otherwise
    // will end up in horrible mess of needing to seek around the
    // archive.  To be able to share this one class implementation
    // with both streams and archives, read it all.

    enum ExtraField { LOCAL = 0, CENTRAL = 1 };
    enum Attribute { INTERNAL = 0, EXTERNAL = 1 };
    enum SizeType { COMPRESSED = 0, UNCOMPRESSED = 1 };
    enum UnsetMethod { NONE = ~0U };

    ZipMember (Filename name);
    ZipMember (const ZipMember &x);
    ZipMember &operator= (const ZipMember &x);
    ~ZipMember (void);

    // FIXME: level = ZLIB level?
    //   ZLIB level:   ZIP level:      (flag & 6):  (= ziplevel << 2)
    //     3-7          0 (normal)         0
    //     8-9          1 (best)           2
    //     2            2 (fast)           4
    //     1            3 (extra fast)     6

    Filename		name (void) const;
    IOOffset		position (void) const;
    IOOffset		size (SizeType which) const;
    unsigned		crc32 (void) const;

    std::string		comment (void) const; // unavailble in stream reading
    Time		time (void) const;

    unsigned		method (void) const;
    unsigned		level (void) const;
    bool		isDirectory (void) const;
    unsigned		flags (void) const;

    unsigned		attributes (Attribute which = EXTERNAL) const;
    void		extra (ExtraField which, void **buffer,
			       IOSize *size) const;

    // Interface for writers.  Size and CRC-32 are filled when the
    // member output stream is closed or flushed, and updated back to
    // this member.  @ZipArchive seeks back to overwrite the local
    // header; @ZipOutputStream buffers compressed output until the
    // values are known and then streams the whole lot out.
    void		position (IOOffset value);
    void		size (SizeType which, IOOffset value);
    void		crc32 (unsigned value);

    void		comment (const std::string &value);
    void		time (Time value);

    void		method (Method value);
    void		level (unsigned value);
    void		isDirectory (bool value);

    void		attributes (Attribute which, unsigned value);
    void		extra (ExtraField which, void *buffer, IOSize size);

private:
    Filename		m_name;		//< Name of the archive member
    std::string		m_comment;	//< Comment field
    IOOffset		m_position;	//< Archive byte position
    void		*m_xlocal;	//< Extra file header data
    IOSize		m_xlocalLen;	//< Length of extra file header data
    void		*m_xglobal;	//< Extra directory data
    IOSize		m_xglobalLen;	//< Length of extra directory data
    Time		m_time;		//< Modification time
    IOOffset		m_csize;	//< Compressed size
    IOOffset		m_dsize;	//< Decompressed size
    unsigned		m_method;	//< Compression method
    unsigned		m_level;	//< Compression level
    unsigned		m_crc32;	//< CRC-32 for all decompressed data
    unsigned		m_attrInt;	//< Internal file attributes
    unsigned		m_attrExt;	//< External file attributes
    bool		m_directory;	//< Flag indicating this is a directory
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_MEMBER_H
