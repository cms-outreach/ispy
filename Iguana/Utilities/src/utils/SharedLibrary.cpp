//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/SharedLibrary.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/SystemError.h"
#include "classlib/sysapi/SharedLibrary.h"
#include <cstring>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>

#ifndef SHLIB_UNSUPPORTED
# define  SHLIB_UNSUPPORTED \
	throw SharedLibraryError ("", "unsupported operation")
#endif

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

#ifdef _WIN32
static BOOL CALLBACK
enumModules (LPSTR name, ULONG base_address, PVOID context)
{
    IMAGEHLP_MODULE			 moduleinfo;
    const SharedLibrary::InfoHandler	*handler
	= static_cast<SharedLibrary::InfoHandler *> (context);

    memset (&moduleinfo, 0, sizeof (moduleinfo));
    moduleinfo.SizeOfStruct = sizeof (moduleinfo);

    SharedLibrary::LibraryInfo info;

    if (SymGetModuleInfo (GetCurrentProcess (), base_address, &moduleinfo))
    {
	info.m_filename   = moduleinfo.LoadedImageName;
	info.m_text_start = moduleinfo.BaseOfImage;
	info.m_text_end   = moduleinfo.BaseOfImage + moduleinfo.ImageSize;
	info.m_data_start = 0;
	info.m_data_end   = 0;
	info.m_bss_start  = 0;
	info.m_bss_end    = 0;
    }
    else
    {
	info.m_filename   = name;
	info.m_text_start = base_address;
	info.m_text_end   = 0;
	info.m_data_start = 0;
	info.m_data_end   = 0;
	info.m_bss_start  = 0;
	info.m_bss_end    = 0;
    }
    (*handler) (info);
    return TRUE;
}
#endif

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

std::string
SharedLibrary::path (void)
{
    const char *pathvar = PATH;
    const char *path = pathvar ? getenv (pathvar) : 0;
    return path ? path : "";
}

void
SharedLibrary::path (const std::string &path)
{
    /* Do not free `var'; most implementations of `putenv' use the
       string without copying it.  On systems where `putenv' copies,
       you'll see leaks from this routine.  It would be possible to
       check for this, but only by killing cross-compilation.

       NB: `HAVE_COPYING_PUTENV' will never be set as we are not
       checking for it :-)  */

    const char *pathvar = PATH;
    if (pathvar) {
	char *var = (char *) malloc (strlen(pathvar) + 1 + path.length () + 1);
	sprintf (var, "%s=%s", pathvar, path.c_str ());
	putenv (var);
#if HAVE_COPYING_PUTENV
	free (var);
#endif
    }
}

/** Return a shared library name that follows the system conventions
    for naming shared library.  @a name is the basic name of the
    shared library, without the name prefix ("lib" on unix) or the
    extension (".so", ".sl", ".dylib" or ".dll").  @a name must not
    have any directory components. */
std::string
SharedLibrary::libname (const std::string &name)
{
#ifdef _WIN32
    return name + ".dll";
#elif defined __hpux
    return "lib" + name + ".sl";
#elif defined __APPLE__ && defined __MACH__
    // FIXME: Darwin supposedly can only load bundles, not dylibs,
    // but the latter seems to work just fine.  Use the dylibs until
    // the issue can be settled.
    return "lib" + name + ".dylib";
#else
    return "lib" + name + ".so";
#endif
}

/** Transform 'extern "C"' symbol @a name into a name suitable for
    lookup in a shared library, e.g. with #data() or #function().
    Normally the latter two automatically perform the necessary
    mangling by calling this function, but the clients can also
    do the mangling themselves.  The @a name should be in the
    form it is spelled in C source code.  */
std::string
SharedLibrary::symname (const std::string &name)
{ return name; }

//////////////////////////////////////////////////////////////////////
/** Return a shared library object representing the application itself.
    The returned object is allocated with @c new.  The caller must
    release the object with either #release() or #abandon().  The
    method throws a #SharedLibraryError if the operation is not
    supported or some failure occurs.  */
SharedLibrary *
SharedLibrary::self (void)
{
#if HAVE_DLOPEN || HAVE_LOAD
    // NB: Linux (at least RH 7.x) dynamic loader is severly broken
    // when it comes to reporting error messages.  The error messages
    // are frequently garbled or null.  If you see a crash in a call
    // to dlerror(), sorry, there's nothing we can do about that.
    // Our attempts have only produced even more undesirable crashes.
    // Waiting for a better version of the linux dynamic laoder.
    void *handle = ::dlopen (0, RTLD_LAZY);
    if (! handle)
    {
	const char *msg = ::dlerror ();
	msg = msg ? msg : "dynamic linker error message lost!";
	throw SharedLibraryError ("dlopen()", msg);
    }

    return new SharedLibrary (handle);
#elif HAVE_SHL_LOAD
    return new SharedLibrary (PROG_HANDLE);
#elif defined _WIN32
    return new SharedLibrary (::GetModuleHandle (0));
#else
    SHLIB_UNSUPPORTED;
#endif
}

/** Load a shared library and return an object representing it.  The
    returned object is allocated with @c new.  The caller must release
    the object with either #release() or #abandon().  The method throws
    a #SharedLibraryError if the operation is not supported or some
    failure occurs.  Please note that on several systems failure to
    properly load a library, e.g. due to missing symbols, is effectively
    fatal.  */
SharedLibrary *
SharedLibrary::load (const std::string &name)
{
    ASSERT (! name.empty ());

    void *handle = 0;

#if HAVE_DLOPEN || HAVE_LOAD
# ifndef RTLD_GLOBAL
#  define RTLD_GLOBAL 0
# endif
    // See comments in "self()" about crashes in dlerror().
    if (! (handle = ::dlopen (name.c_str (), RTLD_LAZY | RTLD_GLOBAL)))
    {
	const char *msg = ::dlerror ();
	msg = msg ? msg : "dynamic linker error message lost!";
	throw SharedLibraryError ("dlopen()", msg);
    }

#elif HAVE_SHL_LOAD
    if (! (handle = ::shl_load (name.c_str (), BIND_DEFERRED, 0L)))
	throw SharedLibraryError ("shl_load()", errno);

#elif defined _WIN32
    if (! (handle = ::LoadLibrary (name.c_str ())))
	throw SharedLibraryError ("LoadLibrary()", GetLastError ());
#else
    SHLIB_UNSUPPORTED;
#endif

    return new SharedLibrary (handle);
}

/** Iterate and provide information about all currently loaded
    shared libraries.  */
void
SharedLibrary::loaded (const InfoHandler &handler)
{
    // Dynamic linker characteristics:
    //   AIX, Windows, SVR4 (DG/UX, DRS/NX, DYNIX/ptx, Linux, SINIX,
    //   Solaris, UnixWare, {Free,Open,Net}BSD if __ELF__), BSD,
    //   HP-UX, IRIX, Tru64

    // Object file formats:
    //   XCOFF (AIX), ELF32/64 (DG/UX, DRS/NX, DYNIX/ptx, IRIX, SINIX,
    //   Solaris, UnixWare, {Free,Open,Net}BSD: if __ELF__), a.out
    //   ({Free,Open,Net}BSD if ! __ELF__, SunOS), BFD (Cygwin, HP-UX,
    //   Linux, LynxOS, Tru64, Windows if GCC), PE (Windows), COFF (?)

#if HAVE_SHL_LOAD					// hp-ux
    shl_descriptor desc;

    for (int index = -1; shl_get_r (index, &desc) == 0; ++index)
    {
	LibraryInfo info;
	info.m_filename   = desc.filename;
	info.m_text_start = desc.tstart;
	info.m_text_end   = desc.tend;
	info.m_data_start = desc.dstart;
	info.m_data_end   = desc.dend;
	info.m_bss_start  = 0;
	info.m_bss_end    = 0;

	handler (info);
    }

#elif HAVE_LINK_H					// bsd/svr4/elf
# if !HAVE_LINK_MAP_L_MAP_START
#  define l_map_start l_addr
#  define l_map_end   l_addr
# endif
# if !HAVE_PROGRAM_INVOCATION_NAME
    static const char *program_invocation_name = "(unknown program name)";
# endif
# if HAVE_R_DEBUG					// linux/glibc
    link_map *p = _r_debug.r_map;
# else
    // Dynamic linker root:
    //   BSD (SunOS):
    //     #include <sys/types.h>
    //     #include <link.h>
    //     extern struct link_dynamic _DYNAMIC;
    //     link_dynamic *d = &_DYNAMIC;
    //     if ((d->ld_version > 1) && (d->ld_version <= 3) && (d->ld_un.ld_1 != 0))
    //       --> link_map *l = d->ld_un.ld_1->ld_loaded
    //          l->lm_name, l->lm_addr, l->lm_next
    //
    //   BSD ({Free,Open,Net}BSD):
    //     #include <sys/types.h>
    //     #include <link.h>
    //     extern struct _dynamic _DYNAMIC
    //     _dynamic *d = &_DYNAMIC;
    //     if ((d->version == LD_VERSION_BSD) && d->d_un.d_sdt != 0))
    //       --> so_map *l = d->d_un.d_sdt->sdt_loaded
    //           l->som_path, l->som_addr, l->som_next
    //
    //   SVR4 (DG/UX, DRS/NX, DYNIX/ptx, SINIX, UnixWare)
    //     ElfW(Dyn) _DYNAMIC[]  // Linux
    //     void _DYNAMIC (void)  // weak, really is data, but not
    //                           // all compilers allow weak data
    //
    //   Solaris:
    //     dlinfo (self, RTLD_DI_LINKMAP, &p);

    extern ElfW(Dyn) _DYNAMIC []; // #pragma weak?
    link_map *p = 0;
    for (ElfW(Dyn) *dyn = _DYNAMIC; dyn->d_tag != DT_NULL; ++dyn)
	if (dyn->d_tag == DT_DEBUG && dyn->d_un.d_ptr)
	    // linux: p = ((r_debug *) dyn->d_un_d.ptr)->r_map;
	    p = (link_map *) *((unsigned long *) dyn->d_un.d_ptr + 1);
# endif

    if (! p)
       throw SharedLibraryError ("loaded", "no shared library load map");

    // Get executable name; linux has a symlink in /proc/self/exe.
    // Linux path names are arbitrarily long, so we just have create
    // some random-sized buffer.  We allocate this on stack to avoid
    // dynamic memory allocation.  If this is a problem, report a bug.
    struct stat	sbuf;
    char	exe [4096];

    memset (exe, 0, sizeof (exe));
    if (::stat ("/proc/self/exe", &sbuf) == 0)
	::readlink ("/proc/self/exe", exe, sizeof (exe)-1);
    else
	STDC::strncpy (exe, program_invocation_name, sizeof (exe)-1);

    // Get shared libraries
    for ( ; p; p = p->l_next)
    {
	LibraryInfo info;

	/* FIXME: Does this work with prelinked shared libraries?
	   From a mail to GCC mailing list ("fde-glibc.c bug"):

	     There is a bug in gcc/config/ia64/fde-glibc.c:
	     ret = find_fde_for_dso ((Elf64_Addr)pc, (Elf64_Ehdr *)map->l_addr,
	     							   ^^^^^^^^^^^
				     segment_base, gp);

	     this will work only as long as the shared library in
	     question has first PT_LOAD segment's p_vaddr == 0.
	     E.g. with ELF prelinking this is almost never true
	     though, so what you really want is map->l_map_start
	     (map->l_addr will be almost always 0) or even better
	     map->l_phdr/map->l_phnum pair.  */

	// FIXME: use the map address (= ElfW(Ehdr)) to scan over
	// the different ElfW(Phdr)s to find the various sections.
	info.m_filename   = (p->l_name && p->l_name[0] ? p->l_name : exe);
	info.m_text_start = p->l_addr ? p->l_addr : p->l_map_start;
	info.m_text_end   = p->l_addr ? p->l_addr : p->l_map_end;
	info.m_data_start = 0;
	info.m_data_end   = 0;
	info.m_bss_start  = 0;
	info.m_bss_end    = 0;

	handler (info);
    }

#elif HAVE_SGIDEFS_H					// irix
    /* From rld(1) man page:

       rld keeps a doubly linked list of structures and crt1.o
       contains a pointer to the head of the list of obj structures
       called __rld_obj_head. In an o32 executable, this points to a
       linked list of objList structures (/usr/include/obj_list.h),
       each of which has a `data' element which is a pointer to a
       `struct obj' (/usr/include/obj.h) (even though the field is not
       declared as a pointer). In an n32 executable, __rld_obj_head
       points to a linked list of Elf32_Obj_Info structures
       (/usr/include/objlist.h).  In a 64-bit executable,
       __rld_obj_head points to a linked list of Elf64_Obj_Info
       structures (/usr/include/objlist.h).  The `oi_magic' element of
       each Elf32_Obj_Info or Elf64_Obj_Info is all-bits-on
       (0xffffffff) to make it easier to determine which list type is
       in use a 32-bit executable.  */

    // To get more details by reading the ELF files:
    // http://reality.sgi.com/davea/software.html
    extern ElfW(Obj_Info)	*__rld_obj_head;
    ElfW(Obj_Info)		*p = __rld_obj_head;

    for ( ; p; p = (ElfW(Obj_Info) *) p->oi_next)
    {
	LibraryInfo info;

# if defined _MIPS_SIM_ABI32 && _MIPS_SIM == _MIPS_SIM_ABI32
	info.m_filename   = (const char *) p->o_path;
	info.m_text_start = p->o_praw;  // base address: o_base_address
	info.m_text_end   = p->o_praw;
# elif (defined _MIPS_SIM_NABI32 && _MIPS_SIM == _MIPS_SIM_NABI32) \
    || (defined _MIPS_SIM_ABI64  && _MIPS_SIM == _MIPS_SIM_ABI64)
	info.m_filename   = (const char *) p->oi_pathname;
	info.m_text_start = p->oi_ehdr; // base address: oi_orig_ehdr
	info.m_text_end   = p->oi_ehdr;
# else
#  error "Unsupported ABI: not o32, n32 or 64"
# endif
	info.m_data_start = 0;
	info.m_data_end   = 0;
	info.m_bss_start  = 0;
	info.m_bss_end    = 0;

	handler (info);
    }

#elif HAVE_LOADER_H && HAVE_LDR_NEXT_MODULE_DECL	// tru64
    ldr_process_t	proc = ldr_my_process ();
    ldr_module_t	mod  = LDR_NULL_MODULE;
    int			ret  = ldr_next_module (proc, &mod);

    for (; ret == 0 && mod != LDR_NULL_MODULE; ret = ldr_next_module (proc, &mod))
    {
	ldr_module_info_t	info;
	size_t			size = 0;
	LibraryInfo		libinfo;

	if (ldr_inq_module(proc, mod, &info, sizeof(info), &size) < 0)
	    throw SharedLibraryError ("ldr_inq_module()", errno);

	libinfo.m_filename   = info.lmi_name;
	libinfo.m_text_start = 0;
	libinfo.m_text_end   = 0;
	libinfo.m_data_start = 0;
	libinfo.m_data_end   = 0;
	libinfo.m_bss_start  = 0;
	libinfo.m_bss_end    = 0;

	for (int i = 0; i < info.lmi_nregion; ++i)
	{
	    ldr_region_info_t	rinfo;
	    unsigned long	low;
	    unsigned long	high;

	    if (ldr_inq_region(proc, mod, i, &rinfo, sizeof(rinfo), &size) < 0)
	        throw SharedLibraryError ("ldr_inq_region()", errno);

	    low  = (unsigned long) rinfo.lri_mapaddr;
	    high = ((unsigned long) rinfo.lri_mapaddr) + rinfo.lri_size;

	    if (!strcmp(rinfo.lri_name, ".text")) {
		libinfo.m_text_start = low;
		libinfo.m_text_end   = high;
	    } else if (!strcmp(rinfo.lri_name, ".data")) {
		libinfo.m_data_start = low;
		libinfo.m_data_end   = high;
	    } else if (!strcmp(rinfo.lri_name, ".bss")) {
		libinfo.m_bss_start  = low;
		libinfo.m_bss_end    = high;
	    }
	}

	handler (libinfo);
    }

    if (ret < 0)
	throw SharedLibraryError ("ldr_next_module()", errno);

#elif HAVE_LOAD && HAVE_LOAD_DECL			// aix
    int		size = 16;
    void	*buffer = new ld_info [size];
    int		error = ::loadquery (L_GETINFO, buffer, size);
    int		offset = 0;

    while (error == -1 && errno == ENOMEM)
    {
	delete [] (ld_info *) buffer;
	buffer = new ld_info [size *= 2];
	error = ::loadquery (L_GETINFO, buffer, size);
    }

    if (error == -1)
	throw SharedLibraryError ("loadquery()", errno);

    while (true)
    {
	LibraryInfo	info;
	ld_info		*ld = (ld_info *) ((char *) buffer + offset);
	const char	*path = ld->ldinfo_filename;
	const char	*member = path + strlen (path) + 1;
	std::string	filename;	// FIXME: Use alloca instead?

	filename = path;
	if (*member)
	{
	    filename += '(';
	    filename += member;
	    filename += ')';
	}

	info.m_filename   = filename.c_str ();
	info.m_text_start = (unsigned long) ld->ldinfo_textorg;
	info.m_text_end   = info.m_text_start + ld->ldinfo_textsize;
	info.m_data_start = (unsigned long) ld->ldinfo_dataorg;
	info.m_data_end   = info.m_data_start + ld->ldinfo_datasize;
	info.m_bss_start  = 0;
	info.m_bss_end    = 0;

	handler (info);

	if (ld->ldinfo_next)
	    offset += ld->ldinfo_next;
	else
	    break;
    }

    delete [] (ld_info *) buffer;

#elif HAVE_MACH_O_DYLD_H				// darwin
    unsigned long images = _dyld_image_count ();
    for (unsigned long i = 0; i < images; ++i)
    {
	const mach_header *hdr = _dyld_get_image_header (i);
	unsigned long	 slide = _dyld_get_image_vmaddr_slide (i);
	unsigned int	size;
	char		*sect;
	LibraryInfo	info;

	info.m_filename   = _dyld_get_image_name (i);

	sect = getsectdatafromheader (hdr, SEG_TEXT, SECT_TEXT, &size);
	info.m_text_start = sect ? (unsigned long) sect + slide : 0;
	info.m_text_end   = sect ? (unsigned long) sect + slide + size : 0;
	sect = getsectdatafromheader (hdr, SEG_DATA, SECT_DATA, &size);
	info.m_data_start = sect ? (unsigned long) sect + slide : 0;
	info.m_data_end   = sect ? (unsigned long) sect + slide + size : 0;
	sect = getsectdatafromheader (hdr, SEG_DATA, SECT_BSS, &size);
	info.m_bss_start  = sect ? (unsigned long) sect + slide : 0;
	info.m_bss_end    = sect ? (unsigned long) sect + slide + size : 0;

	handler (info);
    }

#elif defined _WIN32					// windows
    if (! SymInitialize (GetCurrentProcess (), NULL, TRUE)
	|| ! SymEnumerateModules (GetCurrentProcess (), &enumModules, (void *) &handler)
	|| ! SymCleanup (GetCurrentProcess ()))
	throw SharedLibraryError ("SymEnumerateModules()", GetLastError());
#else
    SHLIB_UNSUPPORTED;
#endif
}

/** Protected constructor for initialising a library object.  The real
    initialisation happens in #load() or #self().   */
SharedLibrary::SharedLibrary (void *handle)
    : m_handle (handle)
{ ASSERT (m_handle); }

/** Protected destructor for cleaning up a library object.  The real
    destruction happens in #release() or #abadon().   */
SharedLibrary::~SharedLibrary (void)
{ ASSERT (! m_handle); }

/** Release a shared library.  This unloads any library the object
    currently refers to, then deletes @c this.  Note that releasing
    the library does not guarantee that it will actually be unloaded.
    If there are outstanding references to the library, explicit or
    implicit, the library will remain in memory.  */
void
SharedLibrary::release (void)
{
    ASSERT (m_handle);

#if HAVE_DLOPEN || HAVE_LOAD
    ::dlclose (m_handle);
#elif HAVE_SHL_LOAD
    ::shl_unload ((shl_t) m_handle);
#elif defined _WIN32
    ::FreeLibrary ((HINSTANCE) m_handle);
#else
    // cannot get here---`load' and `self' should take care of it.
    ASSERT (false);
#endif

    m_handle = 0;
    delete this;
}

/** Abandon a library.  This simply destroys the shared library
    object (@c this) without releasing the underlying dynamic
    object.  */
void
SharedLibrary::abandon (void)
{
    ASSERT (m_handle);
    m_handle = 0;
    delete this;
}

/** Locate and return a reference to a data symbol called @a name.
    If no such symbol exists, returns a null pointer.  If @a mangle
    is the default @c true, the symbol is mangled to the platform
    convention, typically prepending an underscore if required.  The
    mangling does not refer to C++ name mangling, but to the mangling
    required to convert C identifiers to run-time symbol names; see
    #symname() for details.  */
SharedLibrary::Data
SharedLibrary::data (const std::string &name, bool mangle /* = true */) const
{
    ASSERT (! name.empty ());
    ASSERT (m_handle);
    std::string	mangled = mangle ? symname (name) : name;
    Data	symbol = 0;

#if HAVE_DLOPEN || HAVE_LOAD
    // See comments in "self()" about crashes in dlerror().
    const char *error = 0;
    symbol = ::dlsym (m_handle, mangled.c_str ());
    if (! symbol && (error = ::dlerror ()) != 0)
	throw SharedLibraryError ("dlsym()", error);

#elif HAVE_SHL_LOAD
    shl_t handle = (shl_t) m_handle;
    if (::shl_findsym (&handle, mangled.c_str (), TYPE_DATA, &symbol) != 0)
	throw SharedLibraryError ("shl_findsym()", errno);
    ASSERT (handle == (shl_t) m_handle);

#elif defined _WIN32
    if (! (symbol = (Data)::GetProcAddress((HINSTANCE)m_handle, mangled.c_str())))
	throw SharedLibraryError ("GetProcAddress()", GetLastError ());
#else
    // cannot get here---`load' and `self' should take care of it.
    ASSERT (false);
#endif
    return symbol;
}

/** Locate and return a reference to a function symbol called @a name.
    If no such symbol exists, returns a null pointer.  If @a mangle
    is the default @c true, the symbol is mangled to the platform
    convention, typically prepending an underscore if required.  The
    mangling does not refer to C++ name mangling, but to the mangling
    required to convert C identifiers to run-time symbol names; see
    #symname() for details.  */
SharedLibrary::Function
SharedLibrary::function (const std::string &name, bool mangle /* = true */) const
{
    ASSERT (! name.empty ());
    ASSERT (m_handle);
    std::string	mangled = mangle ? symname (name) : name;
    Function	symbol = 0;

#if HAVE_DLOPEN || HAVE_LOAD
    // See comments in "self()" about crashes in dlerror().
    const char *error = 0;
    union { Function func; Data data; } sym;
    sym.data = ::dlsym (m_handle, mangled.c_str ());
    if (! sym.data && (error = ::dlerror ()) != 0)
	throw SharedLibraryError ("dlsym()", error);
    symbol = sym.func;

#elif HAVE_SHL_LOAD
    shl_t handle = (shl_t) m_handle;
    if (::shl_findsym (&handle, mangled.c_str (), TYPE_PROCEDURE, &symbol) != 0)
	throw SharedLibraryError ("shl_findsym()", errno);
    ASSERT (handle == (shl_t) m_handle);

#elif defined _WIN32
    if (! (symbol = (Function) ::GetProcAddress ((HINSTANCE) m_handle,
						 mangled.c_str ())))
	throw SharedLibraryError ("GetProcAddress()", GetLastError ());
#else
    // cannot get here---`load' and `self' should take care of it.
    ASSERT (false);
#endif
    return symbol;
}

} // namespace lat
