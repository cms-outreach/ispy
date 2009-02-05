#ifndef CLASSLIB_STDIO_STORAGE_H
# define CLASSLIB_STDIO_STORAGE_H

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

class StdioStorage : public Storage
{
public:
    StdioStorage (void);
    StdioStorage (const char *name, const char *mode);
    StdioStorage (const std::string &name, const char *mode);
    StdioStorage (FILE *stream);
    ~StdioStorage (void);

    // open (..., OpenMode?)
    virtual void	open (const char *name, const char *mode);
    virtual void	open (const std::string &name, const char *mode);

    virtual bool	hasError (void) const;
    virtual void	clearError (void);

    // setbuf / setvbuf?

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
    FILE		*m_stream;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_STDIO_STORAGE_H
