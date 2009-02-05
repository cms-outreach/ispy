#ifndef CLASSLIB_PIPED_INPUT_STREAM_H
# define CLASSLIB_PIPED_INPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/InputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class PipedOutputStream;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class PipedInputStream : public InputStream
{
    friend class PipedOutputStream;
public:
    PipedInputStream (PipedOutputStream *link = 0);
    ~PipedInputStream (void);

    virtual void	connect (PipedOutputStream *to);

    using InputStream::read;
    virtual IOSize	read (void *into, IOSize n);
    virtual void	close (void);

protected:
    PipedOutputStream	*m_link;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_PIPED_INPUT_STREAM_H
