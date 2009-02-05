#ifndef CLASSLIB_BUFFER_OUTPUT_STREAM_H
# define CLASSLIB_BUFFER_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/FilterOutputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class BufferOutputStream : public FilterOutputStream
{
public:
    BufferOutputStream (OutputStream *output, IOSize bufsize = 0);
    ~BufferOutputStream (void);

    using FilterOutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual IOSize	flush (void);
    virtual void	finish (void);
    virtual void	close (void);

protected:
    unsigned char	*m_buffer;
    unsigned char	*m_top;
    unsigned char	*m_limit;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_BUFFER_OUTPUT_STREAM_H
