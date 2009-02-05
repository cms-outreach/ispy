#ifndef CLASSLIB_BUFFER_INPUT_STREAM_H
# define CLASSLIB_BUFFER_INPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/FilterInputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class BufferInputStream : public FilterInputStream
{
public:
    BufferInputStream (InputStream *input, IOSize bufsize = 0);
    ~BufferInputStream (void);

    using FilterInputStream::read;
    virtual IOSize	available (void);
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	skip (IOSize n);
    virtual void	close (void);
    virtual void	reset (void);

protected:
    unsigned char	*m_buffer;
    unsigned char	*m_bottom;
    unsigned char	*m_top;
    unsigned char	*m_limit;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_BUFFER_INPUT_STREAM_H
