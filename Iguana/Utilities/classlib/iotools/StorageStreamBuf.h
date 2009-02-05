#ifndef CLASSLIB_STORAGE_STREAM_BUF_H
# define CLASSLIB_STORAGE_STREAM_BUF_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/CustomStreamBuf.h"
# include "classlib/iotools/ReadWriteBuffer.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Storage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class StorageStreamBuf : public CustomStreamBuf
{
public:
    StorageStreamBuf (Storage *data = 0);
    ~StorageStreamBuf (void);

    virtual Storage *	storage (void);
    virtual void	attach (Storage *to);
    virtual bool	attached (void) const;
    virtual void	detach (void);
    virtual bool	close (void);

    virtual size_type	showmanyc (void);
    virtual int_type	overflow (int_type c = CLASSLIB_STREAM_EOF);
    virtual int_type	underflow (void);
    virtual int_type	uflow (void);
    virtual int_type	pbackfail (int_type c = CLASSLIB_STREAM_EOF);
    virtual int_type	sync (void);
    
    virtual size_type	xsputn (const char_type *data, size_type length);
    virtual size_type	xsgetn (char_type *data, size_type length);

    virtual pos_type	seekoff (offset_type off, seekdir dir, openmode which);
    virtual pos_type	seekpos (pos_type pos, openmode which);

private:
    bool		syncwrite (void);
    size_type		writable (void);
    size_type		refill (void);

    Storage		*m_data;
    ReadWriteBuffer	m_buffer;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_STORAGE_STREAM_BUF_H
