#ifndef CLASSLIB_SUB_PROCESS_H
# define CLASSLIB_SUB_PROCESS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IOChannel;
class Pipe;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A subprocess.

    This class provides a convenient interface for running commands in
    subprocesses with possibly redirected input and output.  One can
    run either simple subcommands, or entire subprocess pipelines.
    The entire pipeline (henceforth "subprocess chain") can have its
    input or output redirected to or from a pipe, or any #IOChannel
    (e.g. a temporary file).

    The command to run is specified as an @c argz vector: a list of C
    strings with a null pointer to terminate the list, identical to
    the program's @c argv list.  The first element of @c argz is the
    name of the command to execute, the subsequent elements are the
    arguments to it.  All the information in @c argz is always passed
    without modification or interpretation to the child process.  By
    default the command name specifies the pathname of the file to be
    executed.  Using #Search changes this so that if the command does
    not include a path separator (slash (/) on unixen), the method
    duplicates the actions of shell in searching for an executable
    file in the directories listed in the $PATH shell environment
    variable.

    #Argz provides a way to build @c argz lists dynamically.  It also
    provides utilties for converting between @c argz lists and strings
    to parse.  One can force shell to parse of arguments by executing
    the platform equivalent of @c argz = { "/bin/sh", "-c", command }.
    This is not portable and more importantly, ridden with all kinds
    of security risks if the contents of "command" are not strictly
    controlled to be only safe characters.

    By default the subprocesses execute concurrently with the parent
    process.  Use #wait() to suspend the parent until the child exits
    and to find out its exit status.  If the #Synchronous is used when
    creating the subprocess, a #wait() is executed immediately before
    the method returns.

    To construct a subprocess chain, construct multiple SubProcess
    objects and chain them together.  The #First and #Last flags must
    be used by the client to indicate which portion of the chain is
    being constructed.  In addition, the #Read flag may be used when
    constructing the last child to indicate that this will be the end
    of the chain and the parent expects to read from it.  The #Write
    flag may be used at the beginning of the chain to indicate that
    the parent expects to write to the chain (the caller must provide
    the #Pipe for this).  If there are more than one process in the
    chain, intermediate pipes are automatically created; when the next
    subprocess is created, it attaches itself to the other end of the
    pipe.

    The different @c flags combinations overlap.  The purpose is to
    help clients express their needs in the most convenient terms.  To
    avoid confusion in more complicated subprocess chain constructions
    it is simplest to think about each #SubProcess as an individual
    redirection candidate.  All subprocesses can have their input or
    output redirected from or to a pipe; for the first process in the
    chain input redirection from a pipe implies #Write, for the last
    output redirection to a pipe implies #Read, and for all but last
    output redirection means another subprocess will attach to the
    pipe.  The implementation does not distinguish the pipe output
    redirections situations in any way, so different argument and call
    combinations can produce exactly the same result.  The first and
    last processes may also have their input and output, respectively,
    redirected to a #IOChannel, or inherit the parent's standard input
    or output.  These are treated differently only in that the pipes
    are automatically closed in the parent once used in a child
    whereas bare #IOChannel objects are not.

    Attempting to simultanously both write to and read from a process
    causes deadlocks and therefore @a flags is not allowed to contain
    both #Read and #Write.  Instead, if you read from the chain,
    redirect its input from a temporary file, or if you write to it,
    redirect its output to one; see #TempFile::file() for a way
    to create temporary files.  The deadlocks arise because pipes can
    buffer only a limited amount of data: once the buffer is full the
    caller will block until someone reads data off the other end.
    Indiscrete use of pipes to simultaneously read and write results
    in a deadlock where writes in both processes are blocked waiting
    on the other to read -- which neither is able to as they are
    blocked in their own write.  It does not matter how many pipes are
    used, nor in fact whether subprocesses are involved: as long as
    the communication channel has limited buffering capacity and it
    blocks the writer when the buffer becomes full, the processes will
    deadlock sooner or later.  The only workaround is to use
    non-blocking pipes and to explicitly buffer data at each end.

    (FIXME: envz? close descriptors, clean up safe env?) */
class SubProcess
{
    // FIXME: block SIGPIPE -> EPIPE -> throw? (write only; read -> eof)
    // FIXME: Allow redirect of unpiped file descriptors (e.g. output to
    //   child -> allow redirect of stderr, stdout)
    // FIXME: set options on handle inheritance
    // FIXME: set options on env inheritance
    // FIXME: wait with timeout?
public:
    // FIXME: Merge with/continue from IOFlags?
    enum Flags {
	Read		= 1,		//< Make a read subprocess chain
	Write		= 2,		//< Make a write subprocess chain
	First		= 4,		//< First subprocess in a chain
	Last		= 8,		//< Last subprocess in a chain
	One		= First | Last,	//< Single-subprocess chain
	Synchronous	= 16,		//< Run synchronously
	Search		= 32,		//< Search path for command
	NoCloseInput	= 64,		//< Don't close input pipe in parent
	NoCloseOutput	= 128,		//< Don't close output pipe in parent
	NoCloseError    = 256		//< Don't close error pipe in parent
	// CleanEnv	= 512		//< Clean up safe environment
	// CloseFds	= 1024		//< Close all "other" file descriptors
    };

    SubProcess (void);
    SubProcess (const char **argz, unsigned flags = One);
    SubProcess (const char **argz,
		unsigned flags,
		Pipe *pipe,
		IOChannel *other = 0,
		IOChannel *error = 0);
    SubProcess (const char **argz,
		unsigned flags,
		SubProcess *input,
		IOChannel *output = 0,
		IOChannel *error = 0);
    SubProcess (const char **argz,
		unsigned flags,
		IOChannel *input,
		IOChannel *output = 0,
		IOChannel *error = 0);
    virtual ~SubProcess (void);

    virtual pid_t	run (const char **argz, unsigned flags = One);
    virtual pid_t	run (const char **argz,
			     unsigned flags,
			     Pipe *pipe,
			     IOChannel *other = 0,
			     IOChannel *error = 0);
    virtual pid_t	run (const char **argz,
			     unsigned flags,
			     SubProcess *input,
			     IOChannel *output = 0,
			     IOChannel *error = 0);
    virtual pid_t	run (const char **argz,
			     unsigned flags,
			     IOChannel *input,
			     IOChannel *output = 0,
			     IOChannel *error = 0);

    virtual IOChannel *	input (void) const;
    virtual IOChannel *	output (void) const;
    virtual IOChannel *	error (void) const;
    virtual Pipe *	pipe (void) const;
    virtual pid_t	pid (void) const;

    virtual void	detach (void);
    virtual void	terminate (void);
    virtual bool	done (void);
    virtual int		wait (void);

    static bool		exitNormal (int waitcode);
    static bool		exitBySignal (int waitcode);
    static int		exitStatus (int waitcode);
    static int		exitSignal (int waitcode);

private:
    pid_t		dorun (const char **argz, unsigned flags,
			       IOChannel *input, IOChannel *output,
			       IOChannel *error, IOChannel *cleanup);
    pid_t		sysrun (const char **argz, unsigned flags,
				IOChannel *input, IOChannel *output,
				IOChannel *error, IOChannel *cleanup);
    void		sysdetach (void);

    int			m_status;
    IOChannel		*m_input;
    IOChannel		*m_output;
    IOChannel		*m_error;
    Pipe		*m_pipe;
# ifdef _WIN32
    void		*m_sub;
    unsigned long	m_pid;
# else
    pid_t		m_sub;
# endif

    // undefined, no semantics
    SubProcess (const SubProcess &x);
    SubProcess &operator= (const SubProcess &x);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_SUB_PROCESS_H
