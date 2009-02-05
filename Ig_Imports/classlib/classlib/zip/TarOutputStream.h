#ifndef CLASSLIB_TAR_OUTPUT_STREAM_H
# define CLASSLIB_TAR_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/TarConstants.h"
# include "classlib/iotools/FilterOutputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class TarMember;
class MemoryStorage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Write a TAR archive straight into an output stream.

    Notes:
     - This stream may not be usable with real tape devices, or at
       least the caller must determine the tape record size before
       using the stream to output.  (The archive is rounded up to
       the default record size of 20 blocks.)

     - Sparse files are not supported.

    This stream does not work on non-blocking output.  */
class TarOutputStream : public FilterOutputStream, public TarConstants
{
public:
    TarOutputStream (OutputStream *output, bool pad = true);
    ~TarOutputStream (void);
    // no copy constructor
    // no assignment operator

    using FilterOutputStream::write;
    virtual void	setPadding (bool padTrailer);
    virtual void	nextMember (TarMember *member, bool autobuffer=false);
    virtual IOSize	write (const void *from, IOSize n);
    virtual void	closeMember (void);
    virtual void	finish (void);

protected:
    virtual void	writeHeader (void);

private:
    TarMember		*m_current;	//< Current member we are writing
    MemoryStorage	*m_buffer;	//< Automatic buffer if requested
    IOOffset		m_remaining;	//< Bytes still to come for this member
    IOOffset		m_padding;	//< Bytes to be pad after this member
    IOOffset		m_blocks;	/*< Number of blocks written
					    so far, for calculating
					    the amount of padding at
					    the end of the archive.  */
    bool		m_pad;		//< Indicates whether to pad at the end
    bool		m_finished;	//< Indicates when #finished() was done
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_TAR_OUTPUT_STREAM_H
