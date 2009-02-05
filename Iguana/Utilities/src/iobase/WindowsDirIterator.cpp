//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/DirIterator.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/FileSystemError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/DirIterator.h"
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

/** System-specific version of #next().  */
bool
DirIterator::sysnext (void)
{
    if (! FindNextFile (m_data->m_dir, &m_data->m_dirent))
    {
	int error = GetLastError ();
	if (error == ERROR_NO_MORE_FILES)
	    m_data->m_last = true;
	else
	    throw FileSystemError ("FindNextFile()", error);

	return false;
    }
    else
	// skip `.' and `..' directory entries
	return (! m_data->m_dots
		&& m_data->m_dirent.cFileName[0] == '.'
		&& (m_data->m_dirent.cFileName[1] == 0
		    || (m_data->m_dirent.cFileName[1] == '.'
			&& m_data->m_dirent.cFileName[2] == 0)));
}

/** System-specific version of #initialise().  */
void
DirIterator::sysinitialise (const char *dirname)
{
    m_data->m_dir = INVALID_HANDLE_VALUE;
    memset (&m_data->m_dirent, 0, sizeof (m_data->m_dirent));

    if (dirname && dirname[0])
    {
	std::string pattern (dirname);
	if (pattern [pattern.size ()-1] != '\\')
	    pattern += '\\';
	pattern += '*';
	m_data->m_dir = FindFirstFile (pattern.c_str (), &m_data->m_dirent);
	if (m_data->m_dir == INVALID_HANDLE_VALUE)
	{
	    int error = GetLastError ();
	    if (error == ERROR_NO_MORE_FILES)
		m_data->m_last = true;
	    else
		throw FileSystemError ("FindFirstFile()", GetLastError ());
	}
	else if (! m_data->m_dots
		 && m_data->m_dirent.cFileName[0] == '.'
		 && (m_data->m_dirent.cFileName[1] == 0
		     || (m_data->m_dirent.cFileName[1] == '.'
			 && m_data->m_dirent.cFileName[2] == 0)))
	    next ();
    }
}

/** System-specific version of #close().  */
void
DirIterator::sysclose (bool throwable)
{
    HANDLE dir = m_data->m_dir;
    m_data->m_dir = INVALID_HANDLE_VALUE;
    if (dir != INVALID_HANDLE_VALUE && ! FindClose (dir) && throwable)
	throw FileSystemError ("FindClose()", GetLastError ());
}

/** System-specific version of #operator*().  */
std::string
DirIterator::sysderef (void) const
{ return m_data->m_dirent.cFileName; }

} // namespace lat
#endif // _WIN32
