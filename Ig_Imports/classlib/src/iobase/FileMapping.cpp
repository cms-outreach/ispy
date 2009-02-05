//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/FileMapping.h"
#include "classlib/iobase/FileMappingError.h"
#include "classlib/iobase/File.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/FileMapping.h"

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

// FIXME: Mapping -> VirtualProtect?
// FIXME: Giving a name to CreateFileMapping -> Named shared memory region


/** @fn void FileMapping::initialise (void)

    Actually create a file mapping.  If #m_file is null, creates an
    anonymous mapping, otherwise a real one.  Members are to be
    initialised in the caller, please refer to the constructors.  */

/** @fn void *FileMapping::create (IOOffset pos, IOSize length, int prot, int flags)

    Initialise a file mapping view according to the parameters.  If
    #m_file is null, maps an anonymous region, the file and offset
    arguments are ignored and the region is initialised to zeroes.

    @note Windows cannot map files at arbitrary offsets but requires
    them to begin at "system memory allocation granularity" (usually
    around 64 kB, see #SystemInfo::allocGranularity()).  We hide this
    detail from the clients by rounding the requested offset down the
    last allocation boundary, setting #View::m_bias to the difference
    between the boundary and the requested file offset, and returning
    a pointer to the requested region.  When the memory is later
    unmapped, #View::m_bias helps us how to get back the original
    mapping address needed by the system.  However a side effect of
    this is that returned address is page-aligned only if the file
    offset is.  This is a lesser evil than burdening all clients to
    worry about rounding the mapping requests.  */

/** @fn void FileMapping::sysunmap (View &view)

    Actually release a mapping.  */

/** @fn void FileMapping::sysclose (void)

    Release system-specific file mapping data.  */

/** @fn void FileMapping::flush (void *address, IOSize size)

    Flush a portion of the mapped view of the file starting at @a
    address and extending for @a size bytes.  This instructs the
    operating system to write any dirty buffers to disk and if
    possible, to invalidate all other mapped views of the file so that
    they will be refreshed.

    On most systems changes to the file through mapped views and
    conventional I/O do not appear to each other in real time, but
    only when the views are explicitly synchronised.  (FIXME: Explain
    how different/same-inherited mapped views are synchronised.)  */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Create a new file mapping for @a file, with maximum extent @a
    maxSize and @a prot as the protection settings.

    The @a maxSize and @a prot matter only on Windows, on unixen they
    are ignored and only the arguments to #map() matter (for
    portability always specifly the values).  @a maxSize defines the
    maximum extent that will be requested in any subsequent #map()
    invocation on this mapping; zero makes the entire file available.
    @a prot must be a superset of protections that will be requested
    via #map() later on.

    The @a file points to the file to map from and must be specified
    (use the other constructor to create an anonymous mapping).  The
    protections requested in @a prot must be compatible with the way
    the file was opened: if @a prot specifies write access, the file
    must have been opened in write mode.

    @a file becomes property of this mapping and will be deleted by
    the destructor of this object.  (FIXME: Explain whether closing
    the file should be left to the mapping or can be done in client.)  */
FileMapping::FileMapping (File   *file,
			  IOSize maxSize /* = 0 */,
			  int    prot /* = ProtRead|ProtWrite */)
    : m_handle (0),
      m_file (file),
      m_limit (maxSize),
      m_prot (prot)
{ initialise (); }

/** Create an anonymous mapping of maximum extent @a maxSize and @a
    prot as the protection settings.

    The arguments matter only on Windows; on unixen they are ignored
    and only the arguments to #map() matter (for portability always
    specify the values).  @a maxSize defines the maximum extent that
    will be requested in any subsequent #map() invocation on this
    mapping.  @a prot must be a superset of protections that will be
    requested via #map() later on.

    On some platforms anonymous mappings are implemented by opening
    and mapping a special "zero-source" file.  This may result in the
    error conditions that opening a file may cause.  */
FileMapping::FileMapping (IOSize maxSize, int prot /* = ProtRead|ProtWrite */)
    : m_handle (0),
      m_file (0),
      m_limit (maxSize),
      m_prot (prot)
{ initialise (); } 

/** Release the file mapping.  If there are outstanding mapped file
    views associated to this mapping, they are all unmapped first.
    Also closes the and destroys the underlying #File object.  */
FileMapping::~FileMapping (void)
{ close (); }

//////////////////////////////////////////////////////////////////////
/** Map the whole file represented by @c this into memory.

    The mapping preferences are set according to @a flags.  The memory
    region is protected according to @a prot.  The latter should be a
    subset of the flags specified in the constructor and of those used
    to open the underlying file: the file should have been opened with
    #IOFlags::OpenWrite if mapping with #ProtWrite.  The protections
    actually granted may be wider than those requested in @a prot.
    For example, most hardware architectures are not capable of giving
    write access without also granting read access.

    If this variant is applied to an anonymous mapping, it allocates
    new memory block with the size equivalent to the maximum size
    given to the constructor.

    Mapped views are always rounded up to complete system memory pages
    (see #SystemInfo::pagesize()): the address will be page-aligned
    and the length a multiple of page size.  Mapping a file in write
    mode may cause the file to be extended to a multiple of page size
    in length.

    Note that when memory mapping a plain text file for reading, it is
    not safe to treat the returned buffer as a C string as it may not
    be null-terminated: if the file size happens to an exact multiple
    of the system page size, the file contents will not be followed by
    a zero byte.

    @return An #IOBuffer describing the mapped file view.  */
IOBuffer
FileMapping::map (int prot, int flags)
{
    IOSize length = m_file ? IOSized (m_file->size ()) : m_limit;
    ASSERT (length);
    return IOBuffer (create (0, length, prot, flags), length);
}

/** Map the whole file represented by @c this into memory.

    The mapping preferences will be #MapPrivate and the protections
    those given to the #FileMapping constructor; otherwise behaviour
    is equal to #map(int,int).

    If this variant is applied to an anonymous mapping, it allocates a
    new memory block with the size equivalent to the maximum size
    given for the constructor.

    Mapped views are always rounded up to complete system memory pages
    (see #SystemInfo::pagesize()): the address will be page-aligned
    and the length a multiple of page size.  Mapping a file in write
    mode may cause the file to be extended to a multiple of page size
    in length.

    Note that when memory mapping a plain text file for reading, it is
    not safe to treat the returned buffer as a C string as it may not
    be null-terminated: if the file size happens to an exact multiple
    of the system page size, the file contents will not be followed by
    a zero byte.

    @return An #IOBuffer describing the mapped file view.  */
IOBuffer
FileMapping::map (void)
{ return map (m_prot, MapPrivate); }

/** Map a section of the file represented by @c this into memory.
    The mapping begins at @a offset for @a length bytes.

    The mapping preferences are set according to @a flags.  The memory
    region is protected according to @a prot.  The latter should be a
    subset of the flags specified in the constructor and of those used
    to open the underlying file: the file should have been opened with
    #IOFlags::OpenWrite if mapping with #ProtWrite.  The protections
    actually granted may be wider than those requested in @a prot.
    For example, most hardware architectures are not capable of giving
    write access without also granting read access.

    If @c this represents an anonymous mapping, i.e. has no file, @a
    offset is ignored, @a length must be non-zero, and the requested
    memory starts out zero-filled.  A new memory block is allocated
    for every anonymous mapping call.

    Mapped views are always rounded up to complete system memory pages
    (see #SystemInfo::pagesize()): the address will be page-aligned
    and the length a multiple of page size (however note the caveats
    below on Windows).  Mapping a file in write mode may cause the
    file to be extended to a multiple of page size in length.

    Note that Windows is not capable of mapping files at arbitrary
    offsets.  This class works around the limitations in a way that
    most clients should not need to care about.  This means however
    that the returned addresses are not always page-aligned.  The
    address will be page-aligned on Windows only if @a offset is a
    multiple of the page size, but not otherwise.

    Note that when memory mapping a plain text file for reading, it is
    not safe to treat the returned buffer as a C string as it may not
    be null-terminated: if the file size happens to an exact multiple
    of the system page size, the file contents will not be followed by
    a zero byte.

    @return An #IOBuffer describing the mapped file view.  */
IOBuffer
FileMapping::map (IOOffset offset, IOSize length, int prot, int flags)
{
    ASSERT (m_file || length);
    return IOBuffer (create (offset, length, prot, flags), length);
}

//////////////////////////////////////////////////////////////////////
/** Release the mapping that resulted in @a address.  @a address must
    match the result of a previous #map() exactly -- it cannot be an
    interior pointer.  The file view specified in that previous call
    to #map() is unmapped.  */
void
FileMapping::unmap (void *address)
{
    std::list<View>::iterator pos = m_views.begin ();
    while (pos != m_views.end () && pos->m_address != address)
	++pos;

    ASSERT (pos != m_views.end ());
    sysunmap (*pos);
    m_views.erase (pos);
}

/** Release all mappings that have been made from this object.  */
void
FileMapping::unmap (void)
{
    while (! m_views.empty ())
	unmap (m_views.front ().m_address);
}

/** Release the file mapping.  If there are outstanding mapped file
    views associated to this mapping, they are all unmapped first.
    Also closes the and destroys the underlying #File object.  */
void
FileMapping::close (void)
{
    unmap ();
    sysclose ();
    delete m_file;
    m_file = 0;
}

} // namespace lat
