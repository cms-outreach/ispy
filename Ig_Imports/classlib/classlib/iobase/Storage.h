#ifndef CLASSLIB_STORAGE_H
# define CLASSLIB_STORAGE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/IOInput.h"
# include "classlib/iobase/IOOutput.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class Storage : public virtual IOInput, public virtual IOOutput
{
public:
    enum Relative { SET, CURRENT, END };

    Storage (void);
    virtual ~Storage (void);

    using IOInput::read;
    using IOOutput::write;

    virtual IOSize	read (void *into, IOSize n, IOOffset pos);
    IOSize		read (IOBuffer into, IOOffset pos);
    virtual IOSize	write (const void *from, IOSize n, IOOffset pos);
    IOSize		write (IOBuffer from, IOOffset pos);

    virtual bool	eof (void) const;
    virtual IOOffset	size (void) const;
    virtual IOOffset	position (void) const;
    virtual IOOffset	position (IOOffset offset, Relative whence = SET) = 0;
    virtual void	rewind (void);

    virtual void	resize (IOOffset size) = 0;

    virtual void	flush (void);
    virtual void	close (void);

    // FIXME: sticky errors vs. throw?
    //   ferror ()? clearerr ()?

    // FIXME: all derived classes should provide a mode where they
    // *don't* own the underlying storage?

private:
    // undefined, no semantics
    Storage (const Storage &);
    Storage &operator= (const Storage &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_STORAGE_H
