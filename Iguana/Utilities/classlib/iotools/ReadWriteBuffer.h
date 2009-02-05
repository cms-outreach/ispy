#ifndef CLASSLIB_READ_WRITE_BUFFER_H
# define CLASSLIB_READ_WRITE_BUFFER_H

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

/** Buffer for reads and writes to a seekable backing store.

    This class makes it easier to buffer input and output to seekable
    stores such as #Storage objects.  The object maintains a reference
    location and a single combined buffer for both reads and writes.
    The buffer can be thought of as a cache window of data surrounding
    the reference location, the file position of the underlying store.
    The "real" file position is the current buffer position, which is
    usually at an offset from the store's file position: negative after
    buffered reads, positive after buffered writes.

    Clients should use #get() for buffered reads from the store.  If
    #left() returns zero to indicate the buffer is exhausted, clients
    should copy at most #capacity() bytes from the external source into
    #end().  They should then call #filled() to tell the buffer how
    much was added, the method updates the other pointers and moves
    the reference location to the end of the newly added data.  This
    matches normal reads: the store file pointer remains at the end
    of what has just been read.  The "real" file pointer is the one
    maintained by the buffer at the beginning of the data.  It is
    possible to add more data even if #left() does not return zero
    provided #capacity() returns non-zero and is not #dirty().

    Clients should use #put() for buffered writes to the store.  If
    #capacity() returns zero to indicate the buffer is full, clients
    should write the section #dirty() .. #current () to the store.
    #offset() indicates the distance from the reference location at
    which the write should occur, #current() - #dirty() is the number
    of bytes to write.  Once the data has been written, the client
    should call #taken() to move the pointers and the reference
    location to the end of the just written data.  This matches
    normal writes; the store file pointer remains at the end of what
    was just written.

    Writing to the buffer causes it to discard data already buffered
    for reads with #filled().  More specifically, #left() will return
    zero after a #put() as long as there is dirty data in the buffer;
    all of it must be extracted with #taken() before read can proceed.
    Thus, if #left() returns zero, clients should notice the buffer is
    #dirty() and synchronise with the external store before trying to
    read more input.  This behaviour implies that some already data
    may be lost if the underlying input is unseekable (stream-like).
    Use separate read and write buffers for streaming.

    The buffer also maintains a push-back area.  Bytes already read
    from the buffer can be "unread", or new data can be pushed back
    to be accessed on the next #get().  Arbitrary amounts of data
    can be pushed back.  That data is however considered "virtual"
    as it may not correspond to the data in the backing store. Thus
    a #put() discards all pushed-back data, and then writes at the
    last "real" position (remembered by #pback()).  */
class ReadWriteBuffer
{
public:
    enum { DEFAULT_SIZE = 4096 };  //< Default buffer size

    ReadWriteBuffer (IOSize busize = DEFAULT_SIZE);
    ReadWriteBuffer (const ReadWriteBuffer &x);
    ReadWriteBuffer &operator= (const ReadWriteBuffer &x);
    ~ReadWriteBuffer (void);

    IOSize		left (void) const;
    IOSize		capacity (void) const;
    ptrdiff_t    	offset (void) const; 
    //int /* ptrdiff_t */	offset (void) const; // FIXME!

    unsigned char *	pback (void) const;
    unsigned char *	dirty (void) const;
    unsigned char *	buffer (void) const;
    unsigned char *	current (void) const;
    unsigned char *	end (void) const;
    unsigned char *	limit (void) const;
    unsigned char *	reference (void) const;

    void		reset (void);

    void		bump (IOSize n);
    void		put (const void *data, IOSize n);
    void		taken (IOSize n);

    void		filled (IOSize n);
    void		skip (IOSize n);
    void		get (void *data, IOSize n);
    void		unget (const void *data, IOSize n);

private:
    unsigned char	*m_buffer;	//< Start of allocated buffer.
    unsigned char	*m_current;	//< Current read/write position.
    unsigned char	*m_end;		//< End of buffered data.
    unsigned char	*m_limit;	//< One past end of allocated buffer.
    unsigned char	*m_reference;	//< External file pointer location.
    unsigned char	*m_pback;	//< Null or records end of push-back area.
    unsigned char	*m_dirty;	//< Null or records start of dirty data.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_READ_WRITE_BUFFER_H
