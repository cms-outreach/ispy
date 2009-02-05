#ifndef CLASSLIB_IO_CHANNEL_OUTPUT_STREAM_H
# define CLASSLIB_IO_CHANNEL_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/OutputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IOChannel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IOChannelOutputStream : public OutputStream
{
public:
    IOChannelOutputStream (IOChannel *output);
    ~IOChannelOutputStream (void);

    using OutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual IOSize	writev (const IOBuffer *from, IOSize buffers);
    virtual void	close (void);

protected:
    IOChannel		*m_output;	//< Actual output
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_IO_CHANNEL_OUTPUT_STREAM_H
