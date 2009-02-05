#ifndef CLASSLIB_IOSELECT_FD_MAP_H
# define CLASSLIB_IOSELECT_FD_MAP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "IOSelectMethod.h"
# ifndef _WIN32

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** #IOSelectMethod that provides mapping from file descriptors to
    client request index.  */
class IOSelectFdMap : public IOSelectMethod
{
protected:
    size_t		fds (void);
    void		fdmap (IOFD fd, size_t index);
    size_t		fdindex (IOFD fd);
    size_t		purge (IOFD fd);
    void		compact (void);

private:
    //< Mapping table from file descriptor to requests.
    std::vector<size_t> m_fd2index;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline size_t
IOSelectFdMap::fds (void)
{ return m_fd2index.size (); }

inline size_t
IOSelectFdMap::fdindex (IOFD fd)
{
    ASSERT (fd >= 0);
    ASSERT ((size_t) fd < m_fd2index.size ());
    return m_fd2index [fd];
}

} // namespace lat
# endif // !_WIN32
#endif // CLASSLIB_IOSELECT_FD_MAP_H
