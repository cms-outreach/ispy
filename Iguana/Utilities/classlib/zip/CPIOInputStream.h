#ifndef CLASSLIB_CPIO_INPUT_STREAM_H
# define CLASSLIB_CPIO_INPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/CPIOConstants.h"
# include "classlib/iotools/FilterInputStream.h"
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class CPIOMember;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Read a CPIO archive straight off an input stream.

    This stream does not work on non-blocking input.  */

class CPIOInputStream : public FilterInputStream, public CPIOConstants
{
public:
    CPIOInputStream (InputStream *input);
    ~CPIOInputStream (void);
    // no copy constructor
    // no assignment operator

    using FilterInputStream::read;
    virtual CPIOMember *nextMember (void);
    virtual IOSize	available (void);
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	skip (IOSize n);
    virtual void	closeMember (void);

    virtual void	close (void);
    virtual void	reset (void);

private:
    CPIOMember *	readPosixHeader (void);
    CPIOMember *	readSvrHeader (bool check);

    bool		checkHex   (const void *buf, IOSize length);
    bool		checkOctal (const void *buf, IOSize length);

    int			m_format;	//< Archive format (one of magic values)
    CPIOMember		*m_current;	//< Current member
    IOOffset		m_remaining;	//< Bytes left of current member
    unsigned		m_checksum;	//< Checksum for the file contents.
    unsigned		m_sum;		//< Checksum for data read so far.
    bool		m_check;	//< Flag indicating #m_checksum is valid.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_CPIO_INPUT_STREAM_H
