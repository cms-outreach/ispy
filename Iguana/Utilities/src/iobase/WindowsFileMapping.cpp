//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/FileMapping.h"
#include "classlib/iobase/FileMappingError.h"
#include "classlib/iobase/File.h"
#include "classlib/utils/SystemInfo.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/FileMapping.h"
#ifdef _WIN32

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

void
FileMapping::initialise (void)
{
    // Setup inherited security attributes (FIXME: merge somewhere else)
    SECURITY_ATTRIBUTES	sa;
    DWORD		sysprot = 0;
    LARGE_INTEGER	limit;

    sa.nLength = sizeof (sa);
    sa.bInheritHandle = true;
    sa.lpSecurityDescriptor = 0;

    if (m_prot & ProtWrite)
	sysprot = PAGE_READWRITE;
    else
	sysprot = PAGE_READONLY;

    limit.QuadPart = m_limit;
    if (! (m_handle = CreateFileMapping (m_file ? m_file->fd () : IOFD_INVALID,
					 &sa, sysprot, limit.HighPart,
					 limit.LowPart, 0)))
	throw FileMappingError ("CreateFileMapping()", GetLastError ());
}

void *
FileMapping::create (IOOffset pos, IOSize length, int prot, int flags)
{
    ASSERT (pos >= 0);
    ASSERT (! (flags & MapFixed));
    ASSERT (! (flags & MapDenyWrite));
    ASSERT (! (flags & MapExecutable));
    ASSERT ((flags & (MapShared | MapPrivate)) == MapPrivate
	    || (flags & (MapShared | MapPrivate)) == MapShared);

    DWORD		sysprot = (prot & ProtWrite)
				  ? FILE_MAP_WRITE : FILE_MAP_READ;
    DWORD		granularity = SystemInfo::allocGranularity ();
    DWORD		bias = pos % granularity;
    LARGE_INTEGER	off;
    void		*address;

    off.QuadPart = (pos / granularity) * granularity;
    if (! (address = MapViewOfFile (m_handle, sysprot,
				    off.HighPart, off.LowPart,
				    length)))
	throw FileMappingError ("MapViewOfFile()", GetLastError ());

    View view = { (unsigned char *) address + bias, bias, length };
    m_views.push_back (view);
    return view.m_address;
}

void
FileMapping::sysunmap (View &view)
{
    if (! UnmapViewOfFile ((unsigned char *) view.m_address - view.m_bias))
	throw FileMappingError ("UnmapViewOfFile()", GetLastError ());
}

void
FileMapping::sysclose (void)
{
    CloseHandle (m_handle);
    m_handle = INVALID_HANDLE_VALUE;
}

void
FileMapping::flush (void *address, IOSize size)
{
    ASSERT (address);
    if (! FlushViewOfFile (address, size))
	throw FileMappingError ("FlushViewOfFile()", GetLastError ());
}

} // namespace lat
#endif // _WIN32
