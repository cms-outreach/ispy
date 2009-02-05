#ifndef CLASSLIB_LOG_H
# define CLASSLIB_LOG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/Macros.h"
# include "classlib/utils/DebugAids.h"
# include "classlib/utils/iosfwd.h"
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>

# ifndef NLOG
/** Conditionally emit log message.

    If #NLOG compilation flag is not enabled, #LOG() expands to code
    that will dump the output to the logging device.  If the flag is
    enabled, the macro expands to nothing; thus there is no danger
    that unwanted strings or code is left to the executable.

    @param level	The level at and beyond which this output
			should be dumped.  Zero is the lowest level,
			and -1 is interpreted as "always".

    @param what		Indicates the type of dump.  The value should
			be either @c trace, @c error or @c warning.

    @param mode		Debugging mode flag.  This indicates the
			context in which this output was written.  It
			can be one of the #logflag modes (usually
			starting with @c LF), e.g., @c LFgeneral.

    @param v		The expression to dump.  It should be in the
			standard stream insertion format; the first
			left shift operator is supplied by the macro.  */

#  define LOG(level,what,mode,v) lat::logstream::what (mode,level) << v

# else // NLOG
#  define LOG(level,what,mode,v)
# endif // !NLOG

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class logstream;
struct logflag;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>

/** Pseudo log mode that covers everything else.  */
CLASSLIB_API extern logflag LFall;

/** Pseudo log mode that excludes everything else.  */
CLASSLIB_API extern logflag LFnever;

/** Log mode for general messages.  */
CLASSLIB_API extern logflag LFgeneral;

/** Log mode for memory-management messages.  */
CLASSLIB_API extern logflag LFmemory;

/** Log mode for assertion failure messages.  */
CLASSLIB_API extern logflag LFassert;

/** Log mode for exception handling messages.  */
CLASSLIB_API extern logflag LFexception;

/** Log mode for signal handling messages.  */
CLASSLIB_API extern logflag LFsignal;

/** Log mode for I/O messages.  */
CLASSLIB_API extern logflag LFio;

/** Log mode for application initialisation messages.  */
CLASSLIB_API extern logflag LFinit;

/** Log mode for application closure messages.  */
CLASSLIB_API extern logflag LFexit;

/** Log mode for networking messages.  */
CLASSLIB_API extern logflag LFnet;

//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

logstream &flush  (logstream &o);
logstream &endl   (logstream &o);
logstream &ends   (logstream &o);
logstream &indent (logstream &o);
logstream &undent (logstream &o);

//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A log mode flag.

    Log mode flags are used for two purposes: to associate a
    particular dump to a mode and to control what kinds of outputs are
    actually emitted.  All output that is actually emitted is prefixed
    with the mode in which it was produced.  */
struct logflag
{
    /** Pointer to the next log flag.  User defined flags should
	always set this field to zero---it will be initialised when
	the mode flag is used for the first time.  */
    logflag	*next;

    /** A short and descriptive mode name.  It will be used in output
	to identify the mode.  */
    const char	*name;

    /** A flag indicating whether this mode is currently selected.
	The value of this field is determined to see if the dumps in
	this mode should actually emit any output.  Setting the field
	to @c true will enable the mode, while setting it to @c false
	will cause the output to discarded.

	The value of this flag may be changed at any time during the
	execution of the program.  It defaults to the value it is set
	in static initialisation.  That value can be overridden by the
	@c $LOG_MODES environment variable (effective only on first
	use of the mode flag).

	When enabled, the two special modes #LFall and #LFnever
	override the settings of all other modes.  When #LFall is
	enabled, all modes are enabled.  When #LFnever is enabled, all
	modes are disabled.  If both are enabled, #LFall takes
	precendece.  Only if neither is enabled is the value of this
	field consulted.  */
    bool	selected;

    /** If the mode is enabled, this indicates the logging level up to
	which, inclusively, logging will be performed.  The value must
	be -1 or greater; -1 indicates all possible logging levels.
	The value may be changed at any time during the execution of
	the program.

	The default of this value is whatever is assigned to it in
	static initialisers; it is recommended to set it to -1.  This
	may be overridden by the @c $LOG_MODES environment variable
	(effective only on the first use of the mode). */
    int		level;
};

//////////////////////////////////////////////////////////////////////
/** A logging stream.

    #logstream selectively outputs the objects sent to it.  The
    selection is done according to the current <em>logging mode</em>
    and <em>level</em>.  It is also possible to enable and disabled
    the whole system separately from the modes.  Output in a disabled
    mode is discarded.

    There are several ways to control the logging behaviour.  Firstly,
    the program can define default settings for the log flags and call
    the #logstream methods.  Secondly, the user can override the
    default settings from environment variables.  Thirdly, the actual
    #LOG() calls will selectively output the data.  Finally, all the
    logging code can be compiled conditionally.

    #logstream interface is very similar to that of @c std::ostream.
    The left shift insertion operator can be used to send objects to
    the log.  The standard stream manipulators work as usual.
    Polymorphic classes might wish to to inherit from #Debuggable to
    gain more advanced dumping facilities.

    #logstream only uses @c std::ostream for implementation---it does
    not inherit from one.  Most of the features not available in
    #logstream wouldn't be meaningful anyway.  In particular the user
    defined @c std::ostream insertion operators do not work.
    #logstream is meant for dumping objects' internal representation
    for debugging purposes, thus the @c std::ostream inserters would
    seldomly do the right thing.

    #logstream does not require any methods or attributes from the the
    logged objects, and thus the interface is limited to that of @c
    std::ostream.  However, there is support for more complex
    operations than what are allowed by bare @c std::ostream, such as:

     - Logging modes: the #LOG() macro can be used to select a logging
       mode.  According to user's selections output in the various
       modes is discarded or passed thru.  The mode is sticky so that
       objects being logged need not care in which mode they are
       operating in.

     - Logging level: output can be classified to a particular level
       of detail.  Output in a level exceeding user's choice is
       automatically discarded.  Like the mode, the level is sticky.

     - Automatic indenting: #logstream intercepts line breaks in the
       output and indents the lines according current settings.  The
       modules writing output need not ever consider indentation
       themselves.  Indent level can be easily adjusted by sending the
       #indent and #undent manipulators.

       The indentation level can be used to clarify complex data
       structures or program execution.  To increase indentation level
       by one, the program sends the #indent manipulator.  To decrease
       the level by one step, the #undent manipulator is sent.  The
       #logstream takes care of everything else.

     - Depth control: objects can check whether their members should
       be logged or not.  This lets the user to skim over deeply
       nested data structures.

    The logging output can be sent to a varieyt of logging devices:
    standard error, file, fifo, pipe, local or internet socket etc.
    The actual device to use is chosen at runtime via the @c $LOG
    environment variable.  It relatively easy to add new logging
    devices if the existing list is not sufficient.

    The #logstream class has been designed with much care to guarantee
    the minimum amount of interference with the rest of the program.
    Here are the most important principles:

     - #logstream and the logging devices avoid using the dynamic
       memory allocation.  More precisely, if memory is allocated at
       all, it is done only during initialisation of the logging
       device.  If memory has been allocated, it will be released at
       exit time (or whenever the logging stream is destructed).

     - When possible, all initialisation is done statically at compile
       time.

     - The logging operations are permitted throughout the life time
       of the program: the log stream is initialized on the first
       call, so even global constructors may use its services.  It
       remains valid to the end.

     - The logging devices and operations attempt not to interfere
       with system variables such as @c errno.  On certain platforms
       it is not possible to do this for all logging devices but in
       general causes no problems.  In any case no logging operation
       leaks an exception.

     - If the logging operations fail for whatever reason, they are
       silently ignored.  In particular, if the creation of the log
       device fails, #logstream reverts to a safe mode where it
       discards all output sent to it.  */
class logstream
{
public:
    enum State { Trace, Warning, Error };

    logstream (void);
    virtual ~logstream (void);

    static logstream &	logger (void);
    static logstream &	error (logflag &mode, int level);
    static logstream &	warning (logflag &mode, int level);
    static logstream &	trace (logflag &mode, int level);

    logstream &		registerMode (logflag &mode);
    logstream &		mode (logflag &flag, int level);
    logstream &		select (logflag &flag);
    logstream &		unselect (logflag &flag);
    logstream &		state (State value);

    logstream &		indent (int by = 4);
    logstream &		undent (int by = 4);

    logstream &		recurse (bool recursion = true);
    bool		deep (void) const;

    bool		enabled (void) const;
    logstream &		enable (bool value = true);

    bool		selected (void) const;

    logstream &		put (char ch);
    logstream &		put (unsigned char ch);
    logstream &		put (signed char ch);

    logstream &		operator<< (char		ch);
    logstream &		operator<< (unsigned char	ch);
    logstream &		operator<< (signed char		ch);
    logstream &		operator<< (const char *	string);
    logstream &		operator<< (const unsigned char*string);
    logstream &		operator<< (const signed char *	string);
    logstream &		operator<< (const std::string &	string);
    logstream &		operator<< (const void *	pointer);
    logstream &		operator<< (int			n);
    logstream &		operator<< (unsigned int	n);
    logstream &		operator<< (long		n);
    logstream &		operator<< (unsigned long	n);
    logstream &		operator<< (short		n);
    logstream &		operator<< (unsigned short	n);
#if HAVE_LONG_LONG
    logstream &		operator<< (long long		n);
    logstream &		operator<< (unsigned long long	n);
#elif HAVE_INT64
    logstream &		operator<< (__int64		n);
    logstream &		operator<< (unsigned __int64	n);
#endif
#if HAVE_BOOL
    logstream &		operator<< (bool		b);
#endif
    logstream &		operator<< (double		f);
    logstream &		operator<< (float		f);
    logstream &		operator<< (long double		f);
    logstream &		operator<< (logstream & (*fn) (logstream &));
#if HAVE_NEW_IOSTREAMS
    logstream &		operator<< (std::ios & (*fn) (std::ios &));
    logstream &		operator<< (std::ios_base & (*fn) (std::ios_base &));
    logstream &		operator<< (std::ostream & (*fn) (std::ostream &));
#else
    logstream &		operator<< (ios & (*fn) (ios &));
#endif
    logstream &		operator<< (std::streambuf *buffer);

    logstream &		flush (void);

protected:
    virtual void	emit (const char *string, int length) = 0;
    
private:
    struct device;
    class buf;
    friend class buf;

    void		pad (int n);
    void		readjust (void);
    static void		checkMode (logflag &mode);

    static logstream *	createSinkLogDevice (const char *args);
    static logstream *	createFileLogDevice (const char *args);
    static logstream *	createTcpSocketLogDevice (const char *args);
    static logstream *	createUdpSocketLogDevice (const char *args);
    static logstream *	createLocalSocketLogDevice (const char *args);
    static logstream *	createSocketLogDevice (bool local, bool tcp,
					       const char *args);
    static logstream *	createStderrLogDevice (const char *args);
    static logstream *	createConsoleLogDevice (const char *args);
    static logstream *	createPipeLogDevice (const char *args);

    // ------------------------------------------------------------
    /** The logging output buffer.  */
    buf			*m_buf;

    /** The real output engine.  */
    std::ostream	*m_stream;

    /** A flag indicating that objects should dump their contents
	recursively.  */
    bool		m_deep;

    /** A flag indicating whether logging is enabled at all.  */
    bool		m_enabled;

    /** A flag indicating whether stream is in selected state, i.e.,
	the current mode has been enabled and the level is chosen.  */
    bool		m_selected;

    /** The current mode flag; defaults to #LFall.  */
    const logflag	*m_mode;

    /** The current state (error, warning or trace).  */
    State		m_state;

    /** Flag indicating whether the output is at the beginning of the
	line and the new line should prefixed and indented before
	actually outputting anything.  */
    bool		m_newline;

    /** The current indent level.  */
    int			m_indent;

    /** The length of the current mode name. */
    int			m_modeNameLength;

    /** The length of the longest mode name seen so far.  Used to pad
	out the mode names to same width on output. */
    int			m_longestModeName;

    /** A flag indicating whether the line header should be printed in
	the long format.  Most applications will prefer the short
	format; the long format is mostly useful for daemons and
	multiprocess applications, where it is important to know who
	produced a message and when.  */
    bool		m_longLineHeader;


    //------------------------------------------------------------
    /** The chain of registered log flags (i.e., used so far).  */
    static logflag	*s_flags;

    /** Pointer to the logging device used by the #LOG() macros.  */
    static logstream	*s_logger;

    /** A flag to indicate when the logging system has been
        initialized.  */
    static bool		s_initialized;

    /** A flag to indicate when the logging system has been destroyed.
	Used to catch post-destructor #LOG() attempts.  */
    static bool		s_deleted;

    /** List of available logging devices.  */
    static device	s_devices [];
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_LOG_H
