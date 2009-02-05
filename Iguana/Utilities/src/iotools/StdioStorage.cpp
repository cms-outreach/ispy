//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/StdioStorage.h"
#include "classlib/iotools/StdioError.h"
#include "classlib/utils/DebugAids.h"
#include <cerrno>
#ifdef _WIN32
# include <io.h>
# define ftruncate _chsize
#else
# include <unistd.h>	// for ftruncate
#endif

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

StdioStorage::StdioStorage (const char *name, const char *mode)
    : m_stream (fopen (name, mode))
{ if (! m_stream) throw StdioError (errno); }

StdioStorage::StdioStorage (const std::string &name, const char *mode)
    : m_stream (fopen (name.c_str (), mode))
{ if (! m_stream) throw StdioError (errno); }

StdioStorage::StdioStorage (FILE *stream)
    : m_stream (stream)
{}

StdioStorage::~StdioStorage (void)
{
    // Ignore errors, we can't throw from a destructor
    if (m_stream)
	fclose (m_stream);
}

//////////////////////////////////////////////////////////////////////
void
StdioStorage::open (const std::string &name, const char *mode)
{ open (name.c_str (), mode); }

void
StdioStorage::open (const char *name, const char *mode)
{
    ASSERT (! m_stream);
    ASSERT (name && *name);
    ASSERT (mode);

    if (! (m_stream = fopen (name, mode)))
	throw StdioError (errno);
}

// FIXME: This doesn't work because eof() becomes true only when we've
// tried to read past the end: bool StdioStorage::eof (void) const {
// ASSERT (m_stream); return feof (m_stream) != 0; }

bool
StdioStorage::hasError (void) const
{
    ASSERT (m_stream);
    return ferror (m_stream) != 0;
}

void
StdioStorage::clearError (void)
{
    ASSERT (m_stream);
    clearerr (m_stream);
}

//////////////////////////////////////////////////////////////////////
IOSize
StdioStorage::read (void *into, IOSize n)
{
    ASSERT (m_stream);
    IOSize nread = fread (into, n, 1, m_stream);

    // If nread < n, there might have been an error.  Ignore it if
    // nread > 0, otherwise check if it is an end-of-file; if so,
    // return 0, otherwise throw an exception.
    if (nread > 0 || feof (m_stream))
	return nread;
    else
	throw StdioError ();
}

IOSize
StdioStorage::write (const void *from, IOSize n)
{
    ASSERT (m_stream);
    IOSize nwritten = fwrite (from, n, 1, m_stream);

    // If nwritten < n, there probably was an error.  Ignore it if
    // nwritten > 0.  Otherwise throw an exception.
    if (nwritten > 0)
	return nwritten;
    else
	throw StdioError ();
}

//////////////////////////////////////////////////////////////////////
IOOffset
StdioStorage::position (void) const
{
    // FIXME: LFS support?
    ASSERT (m_stream);
    return ftell (m_stream);
}

IOOffset
StdioStorage::position (IOOffset offset, Relative whence /* = SET */)
{
    // FIXME: LFS support?
    ASSERT (m_stream);
    if (fseek (m_stream, offset, (whence == SET ? SEEK_SET
				  : whence == CURRENT ? SEEK_CUR
				  : SEEK_END)))
	throw StdioError (errno);

    return ftell (m_stream);
}

void
StdioStorage::rewind (void)
{
    // Overridden because stdio's rewind also clears the error status.
    ASSERT (m_stream);
    ::rewind (m_stream);
}

//////////////////////////////////////////////////////////////////////
void
StdioStorage::resize (IOOffset size)
{
    // FIXME: No stdio version of these?  This is not portable, try to
    // figure out replacement from File...
    ASSERT (m_stream);
    int fd = fileno (m_stream);
    if (fd == -1)
	throw StdioError ();
    if (ftruncate (fd, size))
	throw StdioError (errno);
}

//////////////////////////////////////////////////////////////////////
void
StdioStorage::flush (void)
{
    ASSERT (m_stream);
    if (fflush (m_stream))
	throw StdioError (errno);
}

void
StdioStorage::close (void)
{
    ASSERT (m_stream);
    FILE *stream = m_stream;
    m_stream = 0;
    if (fclose (stream))
	throw StdioError (errno);
}

} // namespace lat
