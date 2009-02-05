//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/Log.h"
#include <limits.h>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#ifdef _WIN32
# include <io.h>
#else
# include <unistd.h>
#endif
#if HAVE_NEW
# include <new>
#elif HAVE_NEW_H
# include <new.h>
#endif

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>

# ifdef PATH_MAX
#  define LOG_PATH_MAX PATH_MAX+1
# else
#  define LOG_PATH_MAX 1024
# endif

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** File or FIFO logging device.

    @note #FileLog opens and closes the file every so often in order
          for the output to be really flushed to the disk.  The number
          of bytes to write between the flush points is defined by
          #FLUSH_INTERVAL.  This behaviour can generate heavy disk
          traffic when a lot of debugging information is produced.
          The benefit is that the output is written to the disk and
          visible to others sooner; this has some significance for
          instance on AFS file systems.  */

class FileLog : public logstream
{
public:
    enum { FLUSH_INTERVAL = 1024 };
    enum { NAME_LEN = LOG_PATH_MAX };

    FileLog (void);
    ~FileLog (void);

    logstream *		initialise (const char *args);

protected:
    virtual void	emit (const char *string, int length);

private:
    void		close (void);

    /// File descriptor for the log file, or -1.
    int			m_fd;

    /// Number of bytes written since the last flush.
    int			m_written;

    /// Log file name; needed to reopen the file.
    char		m_name [NAME_LEN];

    /// Access rights mask to use when opening the file.
    unsigned int	m_umask;
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

logstream *
logstream::createFileLogDevice (const char *argument)
{
    // make sure the log stays around beyond destructors
    static char		data[sizeof(FileLog)];
    static FileLog	*log = new (&data[0]) FileLog;
    return log->initialise (argument);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/** Initialise an empty log file.  */
FileLog::FileLog (void)
    : m_fd (-1),
      m_written (0),
      m_umask (0)
{ m_name [0] = '\0'; }

/** Direct the log output to file named @a name.  No path name
    expansion is performed on @a name.  The output file name can be
    specified as @c NAME[#TRUNC[#MODE]], where NAME is the actual file
    name, TRUNC can be @c `t' or @c `T' (for truncate existing one) or
    @c `a' or @c `A' (append to existing file), and MODE is a series
    of octal digits (file open umask).  @c `debug.log#T#600' for
    example specifies to open the file @c `debug.log', truncate it if
    it exists, and if not, to create it with umask 0600.  */
logstream *
FileLog::initialise (const char *args)
{
    const char		*name = args;
    const char		*end;
    bool		truncate = false;
    unsigned int	umask = 0600;
    
    if (! args || ! *args)
	args = "debug.log";

    while (*args && *args != '#')
	++args;
    end = args;

    // Parse trailer; this can be any combination of TRUNCATE or MODE
    // definitions separated by colons (can be repeated any number of
    // times).  TRUNCATE may be either `t'/`T' or `a'/`A', the former
    // meaning that the log file should truncated before use, the
    // latter meaning that it should be appended to.  MODE specifies
    // the umask mode in which the file should be opened.

    // If we encounter bad syntax, we'll just refuse to do anything at
    // all to err on the safe side.
    while (args[0] == '#' && args[1])
    {
	if (args[1] == 't' || args[1] == 'T')
	{
	    truncate = true;
	    args += 2;
	}
	else if (args[1] == 'a' || args[1] == 'A')
	{
	    truncate = false;
	    args += 2;
	}
	else if (isdigit ((unsigned char) args[1])
		 && args[1] != '8'
		 && args[1] != '9')
	{
	    unsigned int mask = 0;

	    do
		switch (*++args)
		{
		case '0': mask = (mask << 3) + 0u; break;
		case '1': mask = (mask << 3) + 1u; break;
		case '2': mask = (mask << 3) + 2u; break;
		case '3': mask = (mask << 3) + 3u; break;
		case '4': mask = (mask << 3) + 4u; break;
		case '5': mask = (mask << 3) + 5u; break;
		case '6': mask = (mask << 3) + 6u; break;
		case '7': mask = (mask << 3) + 7u; break;
		default: ASSERT (false);
		}
	    while (isdigit ((unsigned char) args[1])
		   && args[1] != '8'
		   && args[1] != '9');

	    umask = mask;

	    if (args[0] && args[0] != '#')
		return 0;
	}
    }

    ASSERT (end >= name);
    if (end == name || STDC::size_t (end - name) >= sizeof (m_name))
	return 0;

    strncpy (m_name, name, end - name);
    m_fd = ::open (m_name, O_WRONLY|O_APPEND|O_CREAT|(truncate ? 0 : O_TRUNC),
		   umask);

    if (m_fd == -1)
	return 0;
    else
	::write (m_fd, "\n---------- Log file opened ----------\n", 39);

    return this;
}

/** Flush and close the log. */
FileLog::~FileLog (void)
{
    flush ();
    if (m_fd != -1)
	close ();
    m_name[0] = 0;
}

/** Dump output from @a string for @a length bytes to the file.  */
void FileLog::emit (const char *string, int length)
{
    ASSERT (m_fd != -1);
    ::write (m_fd, string, length);


    // Close and reopen the file ever `FLUSH_INTERVAL' bytes.
    // Helps to keep everything synched to disk, especially on
    // AFS systems and if the program is prone to cause OS
    // crashes :-)
    m_written += length;
    if (m_written > FLUSH_INTERVAL)
    {
	::close (m_fd);
	m_fd = ::open (m_name, O_WRONLY|O_APPEND|O_CREAT, m_umask);
	m_written = 0;
    }
}

/** Close the log file.  */
void FileLog::close (void)
{
    ASSERT (m_fd != -1);
    write (m_fd, "\n---------- Log file closed ----------\n", 39);
    ::close (m_fd);

    m_fd = -1;
    m_written = 0;
}

} // namespace lat
