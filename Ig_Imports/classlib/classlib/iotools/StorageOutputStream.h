#ifndef CLASSLIB_STORAGE_OUTPUT_STREAM_H
# define CLASSLIB_STORAGE_OUTPUT_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iotools/OutputStream.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Storage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class StorageOutputStream : public OutputStream
{
public:
    StorageOutputStream (Storage *output);
    ~StorageOutputStream (void);

    using OutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual IOSize	writev (const IOBuffer *from, IOSize buffers);
    virtual void	close (void);

protected:
    Storage		*m_storage;	//< Actual output
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_STORAGE_OUTPUT_STREAM_H
