#ifndef CLASSLIB_TAR_INPUT_STREAM_H
# define CLASSLIB_TAR_INPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/TarConstants.h"
# include "classlib/iotools/FilterInputStream.h"
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class TarMember;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Read a TAR archive straight off an input stream.

    Notes:
     - The archive can contain multiple files with the same file name
       (from updates).

     - If a the same file (dev/inode pair) appears several times in
       the archive, the first time it appears as a regular file; the
       second and subsequent references appear as as a link.

     - The archive may include entries used by TAR to store info for
       itself (labels), but these are not supported.

     - Tar archives consist of a series of 512-byte blocks.  Each
       entry in the archive has a header record followed by file
       contents padded to block size.  The entire archive may be,
       but is not required to be padded to "record size", which is
       normally 20 blocks (= the blocking factor); the actual record
       size depends on tape media used or simply parameters to the
       program that created the archive.  Hence
       the archive may end with an arbitrary number of zero blocks.
       (Note: if archives have been concatenated, there may be zero
       blocks in between archives.)
       The reader detects the all-zeros record as the end of the archive
       marker and stops reading at that point.  (Lack of the all-zeroes
       end-of-archive marker is not considered an error; the last member
       is not required to be full block size either.) There may
       be more such zero blocks (= archive padded to tape record size)
       left in the input stream.  If the archive is embedded in
       another stream, the embedder needs to take this into account,
       for example by knowing the exact size and the skipping what
       this stream did not read, or by continuing to read as long
       as there are null blocks and then pushing the rest back.

     - This stream is not directly usable with real tape devices.  For
       one, someone has to determine the tape record size.  Secondly
       this stream does not make atomic writes of the tape record
       size.  (The archive @em is rounded up to the default record
       size of 20 blocks.)

     - Sparse files are not supported.

     - GNU tar's extra headers are not supported.

    This stream does not work on non-blocking input.  */
class TarInputStream : public FilterInputStream, public TarConstants
{
public:
    TarInputStream (InputStream *input);
    ~TarInputStream (void);
    // no copy constructor
    // no assignment operator

    using FilterInputStream::read;
    virtual TarMember *	nextMember (void);
    virtual IOSize	available (void);
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	skip (IOSize n);
    virtual void	closeMember (void);

    virtual void	close (void);
    virtual void	reset (void);

private:
    bool		validate (const void *buf, IOSize length);
    std::string		extractStr (const void *buf, IOSize max);

    TarMember		*m_current;	//< Current member
    IOOffset		m_remaining;	//< Bytes left of current member
    IOOffset		m_padding;	//< Bytes of padding after member
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_TAR_INPUT_STREAM_H
