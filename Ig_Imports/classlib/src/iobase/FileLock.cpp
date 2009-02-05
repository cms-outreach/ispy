//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/FileLock.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/FileError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/FileLock.h"

// FIXME: doc linux open/O_EXCL docs about broken locking over NFS
//   -> use Filename locking
// FIXME: doc lost locks signals (NFS) cause fatal death: SIGLOST

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

/** @fn bool FileLock::locked (File *file, IOOffset pos, IOOffset length,
			       bool write = true)

    Check if any process currently holds a lock for the specified
    region (@a length may be negative).  If @a write, checks for
    exclusive (write) locks, otherwise checks only for read locks.  */

/** @fn void FileLock::lock (File *file, IOOffset pos, IOOffset length,
    			     bool write = true, bool wait = false)

    Lock the specified region of the @a file (@a length may be negative).
    If @a write, requests an exclusive (write) lock, otherwise a read
    lock.  If @a wait, waits until the region can be locked, otherwise
    fails if someone else already holds a conflicting lock.  If @a
    wait, the wait can be interrupted by a signal (e.g. an alarm)
    (unix only), and may fail if the system detects a dead-lock
    situation (which it may not, even if there is one). */

/** @fn void FileLock:unlock (File *file, IOOffset pos, IOOffset length)

    Unlock the previously locked region of the file.  */

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
FileLock::FileLock (void)
    : m_file (0),
      m_offset (0),
      m_length (0)
{}

FileLock::FileLock (File *file, IOOffset pos, IOOffset len,
		    bool write /* = true */, bool wait /* = false */)
    : m_file (0),
      m_offset (0),
      m_length (0)
{ lock (file, pos, len, write, wait); }

FileLock::~FileLock (void)
{ if (m_file) unlock (m_file, m_offset, m_length); }

/** Unlock the previously locked region of the file.  */
void
FileLock::unlock (void)
{
    ASSERT (m_file);
    unlock (m_file, m_offset, m_length);
    m_offset = m_length = 0;
    m_file = 0;
}

} // namespace lat
