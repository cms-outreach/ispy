//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/SubProcess.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/iobase/Pipe.h"

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

SubProcess::SubProcess (void)
    : m_status (-1),
      m_input (0),
      m_output (0),
      m_error (0),
      m_pipe (0),
      m_sub (0)
{}

SubProcess::SubProcess (const char **argz, unsigned flags /* = One */)
    : m_status (-1),
      m_input (0),
      m_output (0),
      m_error (0),
      m_pipe (0),
      m_sub (0)
{ run (argz, flags); }

SubProcess::SubProcess (const char **argz,
			unsigned flags,
			Pipe *pipe,
			IOChannel *other /* = 0 */,
			IOChannel *error /* = 0 */)
    : m_status (-1),
      m_input (0),
      m_output (0),
      m_error (0),
      m_pipe (0),
      m_sub (0)
{ run (argz, flags, pipe, other, error); }

SubProcess::SubProcess (const char **argz,
			unsigned flags,
			SubProcess *input,
			IOChannel *output /* = 0 */,
			IOChannel *error /* = 0 */)
    : m_status (-1),
      m_input (0),
      m_output (0),
      m_error (0),
      m_pipe (0),
      m_sub (0)
{ run (argz, flags, input, output, error); }

SubProcess::SubProcess (const char **argz,
			unsigned flags,
			IOChannel *input,
			IOChannel *output /* = 0 */,
			IOChannel *error /* = 0 */)
    : m_status (-1),
      m_input (0),
      m_output (0),
      m_error (0),
      m_pipe (0),
      m_sub (0)
{ run (argz, flags, input, output, error); }

SubProcess::~SubProcess (void)
{ detach (); }

/** Run a command in subprocess.

    The execution of the command specified in @a argz is further
    controlled by @a flags.  No input redirection can be established
    by this method, so it can only create the first subprocess in a
    chain.  The child will inherit the parent's standard input.  Thus
    @a flags must include #First and must not include #Write.  If @a
    flags does not include #Last or includes #Read, the process will
    allocate an output pipe() and redirect the output to the pipe's
    sink; the sink will be closed in the parent process.  If @a flags
    includes #Last but not #Read, the subprocess will inherit the
    parent's standard output.  The subprocess always inherits the
    parent's standard error. */
pid_t
SubProcess::run (const char **argz, unsigned flags /* = One */)
{
    // Some things can't be done with this method.
    ASSERT (! (flags & Write));
    ASSERT (flags & First);

    // Pass it on to the real work horse
    return dorun (argz, flags, 0, 0, 0, 0);
}

/** Run a command in subprocess.

    The execution of the command specified in @a argz is further
    controlled by @a flags.  The subprocess may either read from or
    write to @a pipe according to @a flags; the remaining input or
    output can optionionally be redirected to or from @a other.  The
    @a flags must include either #Read or #Write, but not both.

    If @a flags includes #Read, the subprocess will be read from
    through @a pipe and the standard output of the subprocess will be
    redirected to @c pipe->sink().  The sink is closed in the parent.
    The parent is assumed to read from the subprocess chain via @c
    pipe->source(), or to connect it another subprocess.  If @a output
    is non-null, it will become the standard input of the subprocess,
    otherwise the child will inherit the parent's standard input. The
    @a output is not closed in the parent.  Basically using #Read
    generally implies a single subprocess in the chain unless @a
    output is used to redirect the input from another pipe's source;
    however in this case the parent must close the source itself once
    the subprocess has been created.  Using the other overloaded
    methods provide a far more convenient way to achieve this.

    If @a flags includes #Write, the standard input of the subprocess
    is redirected from @c pipe->source().  The source is closed in the
    parent.  The parent is assumed write to the subprocess chain via
    @c pipe->sink(), or that the pipe is the output end of a previous
    subprocess.  If @a other is non-null, it will become the standard
    output of the subprocess; @a other will not be closed in the
    parent.  If @a flags includes #Last and @a other is null, the
    child will inherit the parent's standard output.

    Using this method with #Write flag and the output pipe of the
    previous #SubProcess in the chain is identical to calling the
    #run() with a #SubProcess argument.

    The @a flags may include #First and #Last as long as they do not
    conflict with the above conventions.
    
    If @a error is non-null, it is used for standard error output
    from the subprocess.  Otherwise the subprocess inherits the
    parent's standard error. */
pid_t
SubProcess::run (const char **argz,
		 unsigned flags,
		 Pipe *pipe,
		 IOChannel *other /* = 0 */,
		 IOChannel *error /* = 0 */)
{
    // Require user to tell what they want.  Relying on #First and
    // #Last is too confusing.
    ASSERT ((flags & (Read | Write)) == Read
	    || (flags & (Read | Write)) == Write);

    // Decide what to do with the args.
    if (flags & Read)
	return dorun (argz, flags | NoCloseInput,
		      other, pipe->sink (), error,
		      pipe->source ());
    else if (flags & Write)
    {
	if (other)
	    flags |= NoCloseOutput;

	return dorun (argz, flags,
		      pipe->source (), other, error,
		      pipe->sink ());
    }
    else
	// Misbehave rather than run a unpredictable command.
	// Debugging builds will have caught the error already.
	return 0;
}

/** Run a command in subprocess.

    The execution of the command specified in @a argz is further
    controlled by @a flags.  The subprocess will redirect its input
    from @a input.  Its output is redirected based on @a output and @a
    flags.

    If @a input is non-null, the subprocess input is redirected from
    the @c input->pipe()->source(), which is then closed in the parent
    (the sink side was connected to @a input's standard output and
    closed in the parent when @a input was created).  In this case @a
    flags may not contain #First or #Write.  Using non-null @a input
    is identical to passing in @c input->pipe() with a #Write flag to
    the overloaded method taking a #Pipe argument.

    If @a input is null a new subprocess chain is started; #First is
    implied.  The subprocess will inherit the parent's standard input.
    It is not possible to specify #Write or to otherwise redirect the
    input since there is no way specify the redirection source.  Use
    the other overloaded methods to achieve this.  The output is
    redirected as described below.

    One of #Read or @a output may be given but not both.  If @a output
    is non-null, the subprocess output is redirected to it.  Thus the
    subprocess chain is terminated, implying #Last; @a output will not
    be closed in the parent.  If @a flags includes #Read or does not
    include #Last, an output pipe is allocated and the subprocess
    output is redirected to the sink of the pipe.  The sink is closed
    in the parent.  The parent is assumed to read the output from the
    my @c pipe()->source(), or to chain another process to this one.

    If this is the last subprocess of the chain and @a output is null
    the subprocess will inherit the parent's standard output.  Giving
    #Read and not giving #Last (directly or implied) is equivalent:
    in both cases the output will be created and redirected to.
    
    If @a error is non-null, it is used for standard error output
    from the subprocess.  Otherwise the subprocess inherits the
    parent's standard error. */
pid_t
SubProcess::run (const char **argz,
		 unsigned flags,
		 SubProcess *input,
		 IOChannel *output /* = 0 */,
		 IOChannel *error /* = 0 */)
{
    ASSERT (! output || ! (flags & Read));
    ASSERT (! (flags & Read) || ! output);

    if (output)
	flags |= NoCloseOutput;

    if (input)
    {
	ASSERT (! (flags & First));
	ASSERT (! (flags & Write));
	ASSERT (input->pipe ());
	ASSERT (input->pipe ()->source ());
	ASSERT (input->pipe ()->source ()->fd () != IOFD_INVALID);
	return dorun (argz, flags, input->pipe ()->source (), output, error, 0);
    }
    else
    {
	ASSERT (! (flags & Write));
	return dorun (argz, flags, 0, output, error, 0);
    }
}

/** Run a command in subprocess.

    The name of the command to execute is the first element of @a
    argz, the subsequent elements are the arguments to be passed to
    it.  The @a argz list is terminated with a null pointer, i.e. it
    is built exactly like a program's @c argv list.  #Argz provides a
    convenient way to build such lists dynamically.

    The execution of the command specified in @a argz is further
    controlled by @a flags.  The subprocess will redirect its input
    from @a input and output to @a output.

    If @a input is non-null, the subprocess input is redirected from
    it.  @a input is closed in parent only if @a flags includes #Write
    (to indicate an input pipe).  Use this to not close intermediate
    pipes in the parent of a subprocess chain; retaining them open is
    rarely desirable.  The @a flags may include #First; it is ignored.

    If @a input is null a new subprocess chain is started; #First is
    implied.  The subprocess will inherit the parent's standard input.
    It is not possible to specify #Write or to otherwise redirect the
    input since there is no way specify the redirection source.  Use
    the other overloaded methods to achieve this.

    If @a output is non-null, the subprocess output is redirected to
    it.  @a output is closed in parent only if @a flags includes #Read
    (to indicate an output pipe).  Use this to not close intermediate
    pipes in the parent of a subprocess chain; retaining them open is
    rarely desirable.  The @a flags may include #Last; it is ignored.

    If @a output is null the behaviour depends on #Last and #Read.  If
    #Last is given without #Read, the subprocess inherits the parent's
    standard output.  Otherwise the an output pipe is allocated and
    the subprocess output is redirected to the sink of the pipe.  The
    sink is closed in the parent.  The parent is assumed to read the
    output from the my @c pipe()->source(), or to chain another process
    to this one.
    
    If @a error is non-null, it is used for standard error output
    from the subprocess.  Otherwise the subprocess inherits the
    parent's standard error. */
pid_t
SubProcess::run (const char **argz,
		 unsigned flags,
		 IOChannel *input,
		 IOChannel *output /* = 0 */,
		 IOChannel *error /* = 0 */)
{
    ASSERT (input || ! (flags & Write));

    if (input && ! (flags & Write))
	flags |= NoCloseInput;

    if (output && ! (flags & Read))
	flags |= NoCloseOutput;

    return dorun (argz, flags, input, output, error, 0);
}

pid_t
SubProcess::dorun (const char **argz, unsigned flags,
		   IOChannel *input, IOChannel *output,
		   IOChannel *error, IOChannel *cleanup)
{
    // Check we are not already open
    if (m_sub)
	detach ();

    ASSERT (m_status == -1);
    ASSERT (! m_sub);
    ASSERT (! m_input);
    ASSERT (! m_output);
    ASSERT (! m_error);
    ASSERT (! m_pipe);

    // Check that the command is not empty
    ASSERT (argz);
    ASSERT (*argz);
    ASSERT (**argz);

    // If we need a read pipe, make it.  We own this pipe.
    if (! output && ((flags & Read) || ! (flags & Last)))
    {
	ASSERT (! m_pipe);
	m_pipe = new Pipe;
	ASSERT (m_pipe->source ()->fd () != IOFD_INVALID);
	ASSERT (m_pipe->sink ()->fd () != IOFD_INVALID);

	output = m_pipe->sink ();
	cleanup = m_pipe->source ();
    }

    // Remember the I/O channels
    m_input = input;
    m_output = output;
    m_error = error;

    // Pass it on to the platform
    pid_t child = sysrun (argz, flags, input, output, error, cleanup);

    // Wait right away if it was to run synchronously
    if (flags & Synchronous)
	m_status = wait ();

    return child;
}

IOChannel *
SubProcess::input (void) const
{ return m_input; }

IOChannel *
SubProcess::output (void) const
{ return m_output; }

IOChannel *
SubProcess::error (void) const
{ return m_error; }

Pipe *
SubProcess::pipe (void) const
{ return m_pipe; }

void
SubProcess::detach (void)
{
    sysdetach ();

    if (m_pipe)
    {
	delete m_pipe;
	m_pipe = 0;
    }

    // FIXME: clear()?
    m_status = -1;
    m_input = 0;
    m_output = 0;
    m_error = 0;
}

} // namespace lat
