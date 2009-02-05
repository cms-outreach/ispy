#ifndef CLASSLIB_IO_CHANNEL_INPUT_STREAM_H
# define CLASSLIB_IO_CHANNEL_INPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/InputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IOChannel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IOChannelInputStream : public InputStream
{
public:
    IOChannelInputStream (IOChannel *input);
    ~IOChannelInputStream (void);

    using InputStream::read;
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	readv (IOBuffer *into, IOSize buffers);
    virtual void	close (void);

protected:
    IOChannel		*m_input;	//< Actual input.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_IO_CHANNEL_INPUT_STREAM_H
