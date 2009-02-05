//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/DirIterator.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iobase/FileSystemError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/DirIterator.h"
#ifndef _WIN32

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
    errno = 0;
    if (! (m_data->m_dirent = readdir (m_data->m_dir)) && errno)
	throw FileSystemError ("readdir()", errno);
    else if (! m_data->m_dirent)
    {
	// no more files
	m_data->m_last = true;
	return false;
    }
    else
	// skip `.' and `..' directory entries
	return (! m_data->m_dots
		&& m_data->m_dirent->d_name[0] == '.'
		&& (m_data->m_dirent->d_name[1] == 0
		    || (m_data->m_dirent->d_name[1] == '.'
			&& m_data->m_dirent->d_name[2] == 0)));
}

/** System-specific version of #initialise().  */
void
DirIterator::sysinitialise (const char *dirname)
{
    m_data->m_dir = 0;
    m_data->m_dirent = 0;

    if (dirname && dirname[0])
    {
	if (! (m_data->m_dir = opendir (dirname)))
	    throw FileSystemError ("opendir()", errno);
	next ();
    }
}

/** System-specific version of #close().  */
void
DirIterator::sysclose (bool throwable)
{
    DIR *dir = m_data->m_dir;
    m_data->m_dir = 0;

    if (dir && closedir (dir) == -1 && throwable)
	throw FileSystemError ("closedir()", errno);
}

/** System-specific version of #operator*().  */
std::string
DirIterator::sysderef (void) const
{ ASSERT (m_data->m_dirent); return m_data->m_dirent->d_name; }

} // namespace lat
#endif // !_WIN32
