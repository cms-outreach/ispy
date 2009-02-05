#ifndef CLASSLIB_NAMED_PIPE_SERVER_H
# define CLASSLIB_NAMED_PIPE_SERVER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/IOTypes.h"
# include "classlib/iobase/FileAcl.h"
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A named pipe.

    Named pipe is a named file system object, which when opened acts
    like a pipe channel (see #Pipe).  Unlike its anonymous counterpart
    #Pipe, which has two ends linked together, reading and writing, a
    named pipe can be opened by, written to and read from by any
    number of processes.  The file system name is merely a rendez-vous
    point, the data never passes through the file system.

    Writes of less than #FIXME bytes are atomic: they will not
    interleave with writes from other writers.  Reads may return any
    amount of data, including data from multiple writers, except that
    writes of less than #FIXME bytes are not interleaved (but can be
    merged on read).  When multiple readers read from the same pipe,
    no guarantees are made on who receives which data.

    Normally one would arrange to have one (server) open the pipe for
    read and one or more clients for writing.  If more than one writer
    uses the pipe, one normally exchanges packets of size #FIXME or
    less, with clear message boundaries (e.g. packets exactly of size
    #FIXME or single lines of plain text).  The server reads the data,
    splits it at message boundaries and the processes each message.
    To communicate back to the clients one can set up in each client
    another means communication, for example a named pipe that only
    that client listens to, and the name of which is passed to the
    server in each message.

    #NamedPipeServer is used only to create a named pipe; use #File to
    establish a connection.  The pipe may be opened for read or write
    (FIXME: or both? do all systems support this?).  The pipe must be
    open at both ends before communication is possible; the open
    normally blocks until the other end is opened.  If the pipe is
    opened in non-blocking mode (see #IOFlags::OpenNonBlock flag), the
    call will not block; this succeeds for reading only if nobody else
    has the pipe open for writing, and for writing it will fail unless
    the other end has already been opened.  In other words, the server
    must first open the pipe for read, and then the clients must open
    it write mode.

    Writes to an unconnected pipe throw #SystemError::PipeDisconnected
    error, or raise the #SIGPIPE signal (if the signal is not blocked).
    Reads from an unconnected pipe, e.g. when all clients have closed
    the connection, return 0 to indicate end of connection.

    Using named pipes portably is non-trivial despite encapsulation of
    the system interface provided by this and related clases.  The
    semantics are sufficiently different on UNIX and Windows that the
    client code needs to be careful to work correctly.  On UNIX, named
    pipes are real file system objects which can outlive the processes
    that create them.  On Windows a named pipe exists only while a
    process holds a handle to it (= an instance of this class), and
    they are not real file system objects -- they are merely named as
    if they were files with a special convention that is incompatible
    with UNIX file system conventions: Windows pipes must be named
    \\HOST\pipe\NAME where "HOST" can be "." for local pipes and NAME
    is the name of the pipe (with no slashes allowed).

    FIXME: IOSelector support?  */
class NamedPipeServer
{
    // FIXME: WIN32 allows multiple instances of a pipe?
    // FIXME: WIN32 pipes exist only while process that created them exists
    // FIXME: connect, create, open, listen as separate services?
    // FIXME: block SIGPIPE -> EPIPE -> throw? (write only; read -> eof)
public:
    NamedPipeServer (void);
    NamedPipeServer (const char *name, FileAcl access = 0666);
    NamedPipeServer (const std::string &name, FileAcl access = 0666);
    virtual ~NamedPipeServer (void);

    virtual void	create (const char *name, FileAcl access = 0666);
    virtual void	create (const std::string &name, FileAcl access = 0666);
    virtual void	close (void);
    virtual void	detach (void);

private:
    void		sysinit (const char *name, FileAcl access);
    void		sysclose (void);
    void		sysdetach (void);

    IOFD		m_handle;	//< Handle for Windows.
    std::string		m_name;		//< Name of the pipe.

    // undefined, no semantics
    NamedPipeServer (const NamedPipeServer &);
    NamedPipeServer &operator= (const NamedPipeServer &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_NAMED_PIPE_SERVER_H
