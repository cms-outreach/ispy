#ifndef CLASSLIB_SUB_STORAGE_H
# define CLASSLIB_SUB_STORAGE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/iobase/Storage.h"
# include <string>
# include <cstdio>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A fragment of another #Storage object.

    Allows reads and writes only within the specified byte range.
    Seeks are allowed beyond the given range, but reads and writes
    outside the allowed range fail: reads return nothing, and writes
    refuse to write more than is allowed.
    
    Maintains its own file pointer independent of the underlying base
    storage, so multiple SubStorage objects on a single Storage neither
    interfere nor collaborate.

    Closing a SubStorage is a no-op; close the underlying Storage
    instead.  Resizing requests are ignored.  Deleting a #SubStorage
    will not delete the base storage.  */
class SubStorage : public Storage
{
public:
    // FIXME: bool expandable?
    SubStorage (Storage *base, IOOffset from, IOOffset len);
    ~SubStorage (void);

    using Storage::read;
    using Storage::write;

    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	write (const void *from, IOSize n);

    virtual IOOffset	position (void) const;
    virtual IOOffset	position (IOOffset offset, Relative whence = SET);
    virtual void	rewind (void);

    virtual void	resize (IOOffset size);

    virtual void	flush (void);
    virtual void	close (void);

protected:
    Storage		*m_base;
    IOOffset		m_bias;
    IOOffset		m_length;
    IOOffset		m_position;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_SUB_STORAGE_H
