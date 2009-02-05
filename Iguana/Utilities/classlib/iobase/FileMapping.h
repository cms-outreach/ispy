#ifndef CLASSLIB_FILE_MAPPING_H
# define CLASSLIB_FILE_MAPPING_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/Storage.h"
# include <list>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class File;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Memory-mapped files.  */
class FileMapping
{
    // FIXME: tell page size -- limited to that!
    // FIXME: some systems allow to unmap multiple mappings in one go
    // (even with unmapped section in between), or unmap sections of a
    // file; other systems don't; which interface?
    // FIXME: mremap?
    // FIXME: fixed address?

public:
    // Protection mode bits.
    static const int ProtExec	   = 1;	 //< Execute permission.
    static const int ProtRead	   = 2;	 //< Read permission.
    static const int ProtWrite	   = 4;	 //< Write permission.
    static const int ProtNone	   = 8;	 //< No permission.

    // Flag bits.
    static const int MapShared	   = 1;	 /*< Share the mapping with
					     other processes.  Changes
					     to the region will be
					     written back to the file.  */
    static const int MapPrivate	   = 2;	 /*< Private copy-on-write
					     mapping.  Changes to the
					     region will not be
					     written back to the but
					     copied on write to the
					     process' address space.  */
    static const int MapFixed	   = 4;	 /*< Map to a fixed address.  */
    static const int MapDenyWrite  = 8;	 /*< Deny writes.  Mapping
					     will fail if the file is
					     open for writing.  */
    static const int MapExecutable = 16; /*< Map as an executable? */

    // FIXME: Do spawned processes inherit memory mappings?
    explicit
    FileMapping (File *file, IOSize maxSize = 0, int prot = ProtRead);
    explicit
    FileMapping (IOSize maxSize, int prot = ProtRead | ProtWrite);
    virtual ~FileMapping (void);

    // Mapping views.
    virtual IOBuffer	map (void);
    virtual IOBuffer	map (int prot, int flags);
    virtual IOBuffer	map (IOOffset offset, IOSize length,
			     int prot, int flags);

    // Unmapping views.
    virtual void	unmap (void *address);
    virtual void	unmap (void);
    virtual void	close (void);

    // Flushing views.
    virtual void	flush (void *address, IOSize length);

private:
    struct View
    {
	void		*m_address;
	IOSize		m_bias;
	IOSize		m_size;
    };

    void		initialise (void);
    void *		create (IOOffset pos, IOSize length,
				int prot, int flags);
    void		sysunmap (View &view);
    void		sysclose (void);

    void		*m_handle;
    File		*m_file;
    IOSize		m_limit;
    int			m_prot;
    std::list<View>	m_views;

    // undefined, no semantics
    FileMapping (const FileMapping &);
    FileMapping &operator= (const FileMapping &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_FILE_MAPPING_H
