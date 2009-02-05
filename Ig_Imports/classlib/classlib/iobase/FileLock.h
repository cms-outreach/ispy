#ifndef CLASSLIB_FILE_LOCK_H
# define CLASSLIB_FILE_LOCK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/IOTypes.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class File;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class FileLock
{
public:
    FileLock (void);
    FileLock (File *file, IOOffset pos, IOOffset len,
	      bool write = true, bool wait = false);
    ~FileLock (void);

    void		lock (File *file,
			      IOOffset pos,
			      IOOffset length,
			      bool write = true,
			      bool wait = false);
    void		unlock (void);
    static bool		locked (File *file,
				IOOffset pos,
				IOOffset length,
				bool write = true);

private:
    static void		unlock (File *file, IOOffset pos, IOOffset length);

    File		*m_file;
    IOOffset		m_offset;
    IOOffset		m_length;

    // undefined, no semantics (yet?)
    FileLock (const FileLock &);
    FileLock &operator= (const FileLock &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_FILE_LOCK_H
