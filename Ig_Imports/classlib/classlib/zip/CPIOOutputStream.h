#ifndef CLASSLIB_CPIO_OUTPUT_STREAM_H
# define CLASSLIB_CPIO_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/CPIOConstants.h"
# include "classlib/iotools/FilterOutputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class CPIOMember;
class MemoryStorage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// FIXME: this won't work with real tape devices, since we only pretend
// to write up to the record size.  we don't actually ever do, we just
// pad up to the right size at the end.

/** Write a CPIO archive straight into an output stream.

    This stream does not work on non-blocking output.  */

class CPIOOutputStream : public FilterOutputStream, public CPIOConstants
{
public:
    CPIOOutputStream (OutputStream *output, int format = ODC_MAGIC);
    ~CPIOOutputStream (void);
    // no copy constructor
    // no assignment operator

    using FilterOutputStream::write;
    virtual void	nextMember (CPIOMember *member, bool autobuffer=false);
    virtual IOSize	write (const void *from, IOSize n);
    virtual void	closeMember (void);
    virtual void	finish (void);

protected:
    virtual void	writeHeader (void);

private:
    void		writePosixHeader (void);
    void		writeSvrHeader (void);

    int			m_format;	//< Archive format (one of magic values)
    CPIOMember		*m_current;	//< Current member we are writing
    MemoryStorage	*m_buffer;	//< Automatic buffer if requested
    IOOffset		m_remaining;	//< Bytes still to come for this member
    unsigned		m_checksum;	//< Checksum of file contents (only for autobuffer)
    bool		m_finished;	//< Indicates when #finished() was done
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_CPIO_OUTPUT_STREAM_H
