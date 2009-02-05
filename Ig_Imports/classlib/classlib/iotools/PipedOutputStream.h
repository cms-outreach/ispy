#ifndef CLASSLIB_PIPED_OUTPUT_STREAM_H
# define CLASSLIB_PIPED_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/OutputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class PipedInputStream;
class Pipe;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class PipedOutputStream : public OutputStream
{
public:
    PipedOutputStream (PipedInputStream *link = 0);
    ~PipedOutputStream (void);

    using OutputStream::write;
    virtual void	connect (PipedInputStream *to);
    virtual IOSize	write (const void *from, IOSize n);
    virtual void	close (void);

protected:
    friend class PipedInputStream;
    Pipe *		pipe (void) const;

    PipedInputStream	*m_link;
    Pipe		*m_pipe;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_PIPED_OUTPUT_STREAM_H
