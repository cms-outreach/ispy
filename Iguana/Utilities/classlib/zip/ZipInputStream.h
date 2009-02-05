#ifndef CLASSLIB_ZIP_INPUT_STREAM_H
# define CLASSLIB_ZIP_INPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/ZInputStream.h"
# include "classlib/zip/ZConstants.h"
# include <vector>
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class ZipMember;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// Reads a zip archive straight off an input stream; central directory
// features are not available in the produced members (file comment,
// global external data) until the whole archive has been read (at
// which points the already seen members will be filled in with the
// global data).
//
// Handle stored and compressed files!
class ZipInputStream : public ZInputStream, public ZConstants
{
public:
    ZipInputStream (InputStream *input);
    ~ZipInputStream (void);

    using ZInputStream::read;

    virtual ZipMember *	nextMember (void);
    virtual IOSize	available (void);
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	skip (IOSize n);
    virtual void	closeMember (void);

    virtual void	close (void);

    virtual void	reset (void);

    virtual ZipMember *	sync (void);

protected:
    virtual IOSize	fill (void *buffer, IOSize length);

private:
    void		resetState (bool destroy);
    unsigned		readHeader (void);
    unsigned		findHeader (void);
    ZipMember *		readMemberHeader (unsigned char *buf, IOSize length);
    void		readDirectory (unsigned char *buf, IOSize length);
    void		readEndHeader (unsigned char *buf, IOSize length);

    typedef std::vector<ZipMember *> MemberList;
    MemberList		m_members;	//< Members read and to be updated
    ZipMember		*m_current;	//< The member that is being read
    IOOffset		m_compressed;	//< Compressed data left to read
    IOOffset		m_uncompressed;	//< Uncompressed data left to read
    unsigned		m_magic;	//< Header magic for current state
    std::string		m_comment;	//< Archive comment
    IOOffset		m_total;	//< Total bytes read so far
    IOOffset		m_directory;	//< Position of directory from start
    bool		m_dummy;	//< Indicates when dummy end byte read
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_ZIP_INPUT_STREAM_H
