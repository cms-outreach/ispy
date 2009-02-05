#ifndef CLASSLIB_READ_BUFFER_H
# define CLASSLIB_READ_BUFFER_H

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

/** Object for buffering reads for input streaming.

    Clients should use #get() for buffered reads from the store.  If
    #left() returns zero to indicate the buffer is exhausted, clients
    should copy at most #capacity() bytes from the external source into
    #end().  They should then call #filled() to tell the buffer how
    much was added, the method updates the other pointers. It is
    possible to add more data even if #left() does not return zero
    provided #capacity() returns non-zero.

    The buffer also maintains a push-back area.  Bytes already read
    from the buffer can be "unread", or new data can be pushed back
    to be accessed on the next #get().  Arbitrary amounts of data
    can be pushed back.  */
class ReadBuffer
{
public:
    enum { DEFAULT_SIZE = 4096 };	//< Default buffer size.
    ReadBuffer (IOSize bufsize = DEFAULT_SIZE);
    ReadBuffer (const ReadBuffer &x);
    ReadBuffer &operator= (const ReadBuffer &x);
    ~ReadBuffer (void);

    IOSize		left (void) const;
    IOSize		capacity (void) const;

    unsigned char *	pback (void) const;
    unsigned char *	buffer (void) const;
    unsigned char *	current (void) const;
    unsigned char *	end (void) const;
    unsigned char *	limit (void) const;

    void		reset (void);

    void		filled (IOSize n);
    void		skip (IOSize n);
    void		get (void *data, IOSize n);
    void		unget (const void *data, IOSize n);

private:
    unsigned char	*m_buffer;
    unsigned char	*m_current;
    unsigned char	*m_end;
    unsigned char	*m_limit;
    unsigned char	*m_pback;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_READ_BUFFER_H
