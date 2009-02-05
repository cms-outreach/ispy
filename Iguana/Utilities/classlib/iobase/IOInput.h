#ifndef CLASSLIB_IO_INPUT_H
# define CLASSLIB_IO_INPUT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/IOBuffer.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Abstract base class for stream-oriented input sources. */
class IOInput
{
public:
    virtual ~IOInput (void);
    // implicit constructor
    // implicit copy constructor
    // implicit assignment operator

    int			read (void);
    IOSize		read (IOBuffer into);
    virtual IOSize	read (void *into, IOSize n) = 0;
    virtual IOSize	readv (IOBuffer *into, IOSize buffers);

    IOSize		xread (IOBuffer into);
    IOSize		xread (void *into, IOSize n);
    IOSize		xreadv (IOBuffer *into, IOSize buffers);

protected:
    // Trap migration errors, these are undefined.  Use x/readv() instead.
    IOSize		read (IOBuffer *into, IOSize buffers);
    IOSize		xread (IOBuffer *into, IOSize buffers);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_IO_INPUT_H
