#ifndef CLASSLIB_PIPE_CMD_H
# define CLASSLIB_PIPE_CMD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/IOChannel.h"
# include "classlib/iobase/IOFlags.h"
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Pipe;
class SubProcess;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A pipe connected to read from or write to a subprocess.  This
    is just a simplifying wrapper around #SubProcess.  */
class PipeCmd : public IOChannel
{
public:
# ifdef CLASSLIB_DEPRECATED // Backwards (1.3.x) compatible flags
    static const int READ = IOFlags::OpenRead;
    static const int WRITE = IOFlags::OpenWrite;
# endif

    PipeCmd (void);
    PipeCmd (const char **argz, unsigned mode);
    PipeCmd (const std::string &command, unsigned mode);
    ~PipeCmd (void);
    // implicit copy constructor
    // implicit assignment operator

    virtual void	open (const char **argz, unsigned mode);
    virtual void	open (const std::string &command, unsigned mode);
    virtual int		wait (void);

private:
    Pipe		*m_pipe;
    SubProcess		*m_sub;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_PIPE_CMD_H
