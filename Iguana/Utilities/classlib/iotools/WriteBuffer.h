#ifndef CLASSLIB_WRITE_BUFFER_H
# define CLASSLIB_WRITE_BUFFER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/sysapi/IOTypes.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Object for buffering writes for output streaming.

    Clients should use #put() for buffered writes to the store.  If
    #capacity() returns zero to indicate the buffer is full, clients
    should write the section #dirty() .. #current () to the store.
    #current() - #dirty() is the number of bytes to write.  Once the
    data has been written, the client should call #taken() to move
    the buffer pointers.  */
class WriteBuffer
{
public:
    enum { DEFAULT_SIZE = 4096 }; //< Default buffer size.
    WriteBuffer (IOSize bufsize = DEFAULT_SIZE);
    WriteBuffer (const WriteBuffer &x);
    WriteBuffer &operator= (const WriteBuffer &x);
    ~WriteBuffer (void);

    IOSize		size (void) const;
    IOSize		capacity (void) const;

    unsigned char *	dirty (void) const;
    unsigned char *	buffer (void) const;
    unsigned char *	current (void) const;
    unsigned char *	limit (void) const;

    void		reset (void);

    void		bump (IOSize n);
    void		put (const void *data, IOSize n);
    void		taken (IOSize n);

private:
    unsigned char	*m_buffer;
    unsigned char	*m_current;
    unsigned char	*m_limit;
    unsigned char	*m_dirty;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_WRITE_BUFFER_H
