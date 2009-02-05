#ifndef CLASSLIB_STD_STORAGE_H
# define CLASSLIB_STD_STORAGE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/iobase/Storage.h"
# include <string>
# include <iostream>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class StdStorage : public Storage
{
public:
    StdStorage (void);
# if HAVE_NEW_IOSTREAMS
    StdStorage (const char *name, std::ios_base::openmode mode
		= std::ios_base::in | std::ios_base::out);
    StdStorage (const std::string &name, std::ios_base::openmode mode
		= std::ios_base::in | std::ios_base::out);
# else
    StdStorage (const char *name, std::ios::openmode mode
		= std::ios::in | std::ios::out);
    StdStorage (const std::string &name, std::ios::openmode mode
		= std::ios::in | std::ios::out);
# endif
    StdStorage (std::iostream *stream);
    ~StdStorage (void);

# if HAVE_NEW_IOSTREAMS
    virtual void	open (const char *name, std::ios_base::openmode mode
			      = std::ios_base::in | std::ios_base::out);
    virtual void	open (const std::string &name, std::ios_base::openmode mode
			      = std::ios_base::in | std::ios_base::out);
# else
    virtual void	open (const char *name, std::ios::openmode mode
			      = std::ios::in | std::ios::out);
    virtual void	open (const std::string &name, std::ios::openmode mode
			      = std::ios::in | std::ios::out);
# endif

    virtual bool	hasError (void) const;
    virtual void	clearError (void);

    using Storage::read;
    using Storage::write;

    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	write (const void *from, IOSize n);

    virtual IOOffset	position (void) const;
    virtual IOOffset	position (IOOffset offset, Relative whence = SET);
    virtual void	resize (IOOffset size);

    virtual void	flush (void);
    virtual void	close (void);

protected:
    std::iostream	*m_stream;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_STD_STORAGE_H
