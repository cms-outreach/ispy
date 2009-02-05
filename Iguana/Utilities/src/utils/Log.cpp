//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/Log.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/ProcessInfo.h"
#include "classlib/utils/BitTraits.h"
#include "classlib/utils/SaveErrno.h"
#include "classlib/utils/LongLong.h"
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <string>
#include <functional>
#include <algorithm>
#if HAVE_NEW_IOSTREAMS
# include <streambuf>
# include <ostream>
# include <iomanip>
#else
# include <iostream.h>
# include <iomanip.h>
#endif

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** Description of an available log output device.  */
struct logstream::device
{
    const char		*name;
    logstream *		(*creator) (const char *argument);
};

/** The @c std::streambuf implementation for #logstream. */
class logstream::buf : public std::streambuf
{
public:
#if HAVE_NEW_IOSTREAMS
//# define STREAM_EOF traits_type::eof ()
# define STREAM_EOF std::streambuf::traits_type::eof ()
    typedef std::streamsize	size_type;
#else
# define STREAM_EOF EOF
    typedef int			int_type;
    typedef char		char_type;
    typedef int			size_type;
#endif

    enum { SIZE = 32 };

    buf (logstream *log);
    ~buf (void);

    virtual int_type		overflow (int_type c = STREAM_EOF);
    virtual size_type		xsputn (const char_type *string,
					size_type length);

private:
    /// The logging device this buffer is connected to. 
    logstream			*m_log;
    /// The number of characters currently in the holding area.
    int				m_pos;
    /// The output holding area.
    char_type			m_buffer [SIZE];
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

// Predefined log modes

logflag			LFall	    = { 0,	      "all",       false, -1 };
logflag			LFnever	    = { &LFall,	      "never",     false, -1 };
logflag			LFgeneral   = { &LFnever,     "general",   true, -1 };
logflag			LFmemory    = { &LFgeneral,   "memory",    true, -1 };
logflag			LFassert    = { &LFmemory,    "assert",	   true, -1 };
logflag			LFexception = { &LFassert,    "exception", true, -1 };
logflag			LFsignal    = { &LFexception, "signal",    true, -1 };
logflag			LFio	    = { &LFsignal,    "io",	   true, -1 };
logflag			LFinit	    = { &LFio,	      "init",	   true, -1 };
logflag			LFexit	    = { &LFinit,      "exit",	   true, -1 };
logflag			LFnet	    = { &LFexit,      "net",	   true, -1 };

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

logflag *		logstream::s_flags = &LFnet;
logstream *		logstream::s_logger = 0;
bool			logstream::s_initialized = false;
bool			logstream::s_deleted = false;

// Log devices built into the program.  Alas, we can't register these
// dynamically by collecting whatever is linked in -- logstream needs
// to be available on first use, even if it is in global constructors.
// It may well happen that the devices get registered after first use.
logstream::device	logstream::s_devices [] = {
    { "sink",		&createSinkLogDevice },
    { "file",		&createFileLogDevice }, // NB: file or named pipe
    { "socket/tcp",	&createTcpSocketLogDevice },
    { "socket/udp",	&createUdpSocketLogDevice },
#if HAVE_LOCAL_SOCKETS
    { "socket/local",	&createLocalSocketLogDevice },
#endif
    { "stderr",		&createStderrLogDevice },
    { "console",	&createConsoleLogDevice },
    { "pipe",		&createPipeLogDevice },
    { 0,		0 }
};

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

/** Manipulator that flushes the @a o #logstream and returns the stream. */
logstream &
flush (logstream &o)
{ return o.flush (); }

/** Manipulator that flushes the @a o #logstream, sends a new line,
    and returns the stream.  */
logstream &
endl (logstream &o)
{ return o.put ('\n'); }

/** Manipulator that flushes the @a o #logstream, sends an end of
    string, and returns the stream.  */
logstream &
ends (logstream &o)
{ return o.put ('\0').flush ();}

/** Manipulator that increases the indentation level of @a o stream
    and returns the stream.  */
logstream &
indent (logstream &o)
{ return o.indent (); }

/** Manipulator that decreases the indentation level of @a o stream
    and returns the stream.  */
logstream &
undent (logstream &o)
{ return o.undent (); }

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Initialize the log buffer with @a log as the owner. */
logstream::buf::buf (logstream *log)
    : m_log (log), m_pos (0)
{
    setp (0, 0);
    setg (0, 0, 0);
}

/** Flush and release the buffer.  */
logstream::buf::~buf (void)
{ overflow (); }

/** Handle buffer output.

    Since the put area is empty we are in effect unbuffered, and this
    method will be called every time there is output.  We take the
    opportunity to adjust the line prefix here and then buffer #SIZE
    characters, or till end of the line, which ever is less.  When the
    buffer must be flushed, it is sent back to the #logstream.

    @param ch	Character to consume or end of file indicator to flush. 

    @return End of file indicator on failure, zero on success.  */
logstream::buf::int_type
logstream::buf::overflow (int_type ch)
{
    if (ch != STREAM_EOF)
    {
	if (m_log->m_newline)
	    m_log->readjust ();
	m_buffer [m_pos++] = (char) ch;

	// FIXME: should use the following, but MSVC++ does not have
	// `newline' in traits
	//   ch == traits_type::newline ()
	if (ch == '\n' || m_pos == SIZE)
	{
	    m_log->emit (m_buffer, m_pos);
	    m_pos = 0;
	    m_log->m_newline = (ch == '\n');
	    // = (ch == traits_type::newline ())
	}
    }
    else if (m_pos)
    {
	m_log->emit (m_buffer, m_pos);
	m_pos = 0;
    }

    return 0;
}

/** Handle lengthy buffer output.

    This function permits faster output when a longer sequence of
    known characters will be written.  Instead of writing every
    character separately it prints out the whole thing.

    @param string	The sequence of characters to output.
    @param length	The number of characters in the sequence.

    @return The number of characters delivered to output; the value is
    always equal to @a length. */
logstream::buf::size_type
logstream::buf::xsputn (const char_type *string, size_type length)
{
    for (int start = 0, end = 0; start < length; start = end)
    {
	if (m_log->m_newline)
	    m_log->readjust ();

	// FIXME: should use: `&& string [end] != traits_type::newline ()'
	while (end < length && string [end] != '\n')
	    ++end;

	if (end < length)
	{
	    // Include the newline
	    ++end;
	    m_log->m_newline = true;
	}

	// Copy either via buffer (if there was already something
	// there) or emit directly (if the buffer was empty or if
	// there are too many characters).  We use the buffer only on
	// the first string segment if the buffer is non-empty and on
	// the last segment if there isn't a trailing newline and the
	// segments fit into the buffer.
	if (m_pos)
	{
	    // Use the buffer since there is already something there.
	    while (m_pos < SIZE && start < end)
		m_buffer [m_pos++] = string [start++];

	    if (m_pos == SIZE || m_log->m_newline)
	    {
		m_log->emit (m_buffer, m_pos);
		m_pos = 0;
	    }
	}
	if (! m_log->m_newline && end - start < SIZE - m_pos)
	    // Copy a trailing, not newline-terminated segment that is
	    // short enough to fit to the buffer.  Note that the
	    // previous `if' might have already flushed out a first
	    // part of this string.
	    while (start < end)
		m_buffer [m_pos++] = string [start++];

	else if (start < end)
	    // Just write it out.  Either it is newline terminated or
	    // it is the rest of a too long string, or it is part of a
	    // multiline string.
	    m_log->emit (string + start, end - start);
    }

    return length;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/** Initialise a log stream. The default values are as follows:
     - objects should dump their members  (#deep() is @c true)
     - the log is enabled
     - the log is selected
     - all levels are enabled (#level() is -1); @c $LOG_LEVEL
       environment variable can be used to override the default
       level value
     - the logging mode is #LFall
     - the logging state is #Trace
     - long line headers are not printed.  */
logstream::logstream (void)
    : m_buf (new buf (this)),
      m_stream (new std::ostream (m_buf)),
      m_deep (true),
      m_enabled (true),
      m_selected (true),
      m_mode (&LFall),
      m_state (Trace),
      m_newline (true),
      m_indent (0),
      m_modeNameLength (strlen (m_mode->name)),
      m_longestModeName (10), // Longest known LF* name length
      m_longLineHeader (false)
{
    const char *format = getenv ("LOG_FORMAT");
    if (format && strcmp (format, "long") == 0)
	m_longLineHeader = true;
}

/** Free the resources used by the log stream. */
logstream::~logstream (void)
{
    delete m_stream;
    delete m_buf;
    m_stream = 0;
    m_buf = 0;

    if (s_logger == this)
	s_deleted = true;
}

/** Check what the environment says object a mode flag.

    The environment variable @c $LOG_MODES is used to determine which
    modes should be turned on and which ones should be turned off.
    The value of the variable must be a delimited set of mode names;
    valid delimiters are comma, colon, space, tab and newline.  The
    mode name may be prefixed with a modifier telling whether the mode
    is turned on or off; no modifier is interpreted as request to turn
    on.  Valid modifiers are @c `+' (plus) to turn on and @c `-'
    (minus) or @c `!' (bang) to turn off.  A lone modifier without a
    mode name applies to all modes.  The first occurence of the mode
    name (or a lone modifier) is used to determine the setting.  If
    the mode name is followed by a a slash @c `/', it is expected to
    be followed by a number that specifies which logging levels the
    flag should be enabled at; all levels upto and including the given
    level are enabled.  To enable the mode at all levels, use -1.

    For example, if the value of @c $LOG_MODES is @c `general:exit',
    then the modes @c general and @c exit are enabled; other modes are
    not affected.  The same effect could be achieved by specifying @c
    `+general +exit'.  To enable @em only the modes @c general and @c
    exit, set @c $LOG_MODES to @c `+general:+exit:-'.  To enable all
    modes regardless of the compile time settings, set @c $LOG_MODES
    to @c `+'.  To turn off a particular mode, say @c net, set the
    value to @c `-net'.  To indicate that mode @c net is to be enabled
    only upto level 3, specify @c `+net/3'.  To indicate that all
    modes should be enabled upto level 2, specify @c `+/2'.

    @param mode			The flag to look for.

    @return The selected status of the mode is assigned @c true if the
    flag should be selected, @c false otherwise.  If the environment
    does not specify anything, the value is left unchanged.  If the
    level is defined in the environment, the level status is modified.  */
void
logstream::checkMode (logflag &mode)
{
    const char *modes = getenv ("LOG_MODES");
    if (modes)
	while (true)
	{
	    switch (*modes)
	    {
	    case '\0':
		// Reached the end of the strings.
		return;

	    case ',': case ':': case ' ': case '\t': case '\n':
		// Skip flag delimiters
		++modes;
		continue;

	    case '+':
		++modes; // fall thru
	    default:
		{
		    const char *start = modes;
		    while (*modes != '\0'
			   && *modes != '/'
			   && *modes != ':'
			   && *modes != ','
			   && *modes != ' '
			   && *modes != '\t'
			   && *modes != '\n')
			++modes;

		    if (strncmp (mode.name, start, modes-start) == 0)
		    {
			int level = -1;
			if (*modes == '/')
			{
			    start = ++modes;
			    while (isdigit ((unsigned char) *modes))
				++modes;
			}
			else
			    start = getenv ("LOG_LEVEL");

			if (start && start != modes)
			    sscanf (start, "%d", &level);

			mode.level = level;
			mode.selected = true;
			return;
		    }

		    continue;
		}

	    case '-': case '!':
		{
		    const char *start = ++modes;

		    // `/' is ignored in negative
		    while (*modes != '\0'
			   && *modes != ':'
			   && *modes != ','
			   && *modes != ' '
			   && *modes != '\t'
			   && *modes != '\n')
			++modes;

		    if (strncmp (mode.name, start, modes-start) == 0)
		    {
			mode.selected = false;
			return;
		    }

		    continue;
		}
	    }
	}
}

/** Register @a mode if it hasn't been registered yet and return self.
    If the mode is already known, nothing is done.  If it is a new
    one, the @c $LOG_MODES environment variable is checked to see
    whether the mode should be selected or not.

    @sa #checkMode() on impact of environment variables.  */
logstream &
logstream::registerMode (logflag &mode)
{
    if (mode.next != 0 || &mode == &LFall)
	return *this;

    int length = strlen (mode.name);
    if (length > m_longestModeName)
	m_longestModeName = length;

    mode.next = s_flags;
    s_flags = &mode;
    checkMode (mode);

    return *this;
}

/** Set the logging mode and return reference to self.  If the logging
    mode for following output (@a mode) is selected and the logging
    level for the output (@a level) does not exceed the current limit,
    further output is allowed.  In any other case further output in
    this mode and level is suppressed.  If this method changes the
    current mode, a newline is forced.  */
logstream &
logstream::mode (logflag &mode, int level)
{
    // Force a new line at a beginning of the mode.
    if (m_mode != &mode && ! m_newline)
	*this << '\n';

    registerMode (mode);
    m_mode = &mode;
    m_modeNameLength = strlen (mode.name);

    // Shut off by level
    if (mode.level >= 0 && level > mode.level)
    {
	m_selected = false;
	return *this;
    }

    // If LFall is selected, all modes are selected.  Similarly, if LFnever is
    // selected, no mode is selected (LFall overrides LFnever).  If neither
    // LFall nor LFnever define anything, the mode itself determines whether
    // it is selected.
    if (LFall.selected)
    {
	m_selected = true;
	return *this;
    }
    else if (LFnever.selected || m_mode == &LFnever)
    {
	m_selected = false;
	return *this;
    }

    m_selected = m_mode->selected;
    return *this;
}

/** Make logging @a mode selected no matter what environment variables
    say and returns reference to self.  */
logstream &
logstream::select (logflag &mode)
{
    registerMode (mode);

    mode.selected = true;

    if (m_mode == &mode)
	m_selected = (&mode != &LFnever);

    return *this;
}

/** Make a mode unselected no matter what environment variables say
    and returns reference to self.  */
logstream &
logstream::unselect (logflag &mode)
{
    registerMode (mode);

    mode.selected = false;

    if (m_mode == &mode)
	m_selected = LFall.selected;

    return *this;
}

/** Set logging state to @a value and return reference to self.  The
    stream prefixes the actual logging output with the current state.
    If this method causes a state change, a newline is forced.  */
logstream &
logstream::state (State value)
{
    ASSERT (value == Trace || value == Warning || value == Error);

    if (value != m_state)
    {
	m_state = value;

	if (! m_newline)
	    *this << '\n';
    }
    return *this;
}

/** Increase the indent level by @a by and return reference to self.  */
logstream &
logstream::indent (int by /* = 4 */)
{ m_indent += by; return *this; }

/** Decrease the indent level by @a by and return reference to self.  */
logstream &
logstream::undent (int by /* = 4 */)
{ m_indent -= by; if (m_indent < 0) m_indent = 0; return *this; }

/** Tell how objects should log their contents: recursively if @a
    recursion is set, thinly otherwise.  Returns a reference to self.  */
logstream &
logstream::recurse (bool recursion /* = true */)
{ m_deep = recursion; return *this; }

/** Return the data structure recursion flag.  If the flag is @c true,
    objects should log their contents recursively, otherwise thinly.  */
bool
logstream::deep (void) const
{ return m_deep; }

/** Check if the stream is enabled, i.e. should actually output
    whatever is sent to it.  */
bool
logstream::enabled (void) const
{ return m_enabled; }

/** Check if the stream was selected by the most recent log level.  */
bool		
logstream::selected (void) const
{ return m_selected; }

/** Enable or disable the stream according to @a value.  The stream
    outputs whatever is sent to it only if it is enabled.  Returns a
    reference to self.  */
logstream &
logstream::enable (bool value /* = true */)
{ m_enabled = value; return *this; }

/** Get the reference to the current logger for the #LOG() macros.
    Initialises the logger on first access.  If the initialisation
    fails for any reason, falls back on a sink that eats all output.  */
logstream &
logstream::logger (void)
{
    if (! s_initialized)
    {
	s_initialized = true;

	// Set the levels of various flags
	checkMode (LFgeneral);
	checkMode (LFmemory);
	checkMode (LFassert);
	checkMode (LFexception);
	checkMode (LFsignal);
	checkMode (LFio);
	checkMode (LFinit);
	checkMode (LFexit);
	checkMode (LFnet);

	// Sink will not fail or throw
	const char	*name = getenv ("LOG");
	const char	*args;
	logstream *	(*creator) (const char *);

	if (! name)
	    name = "sink";

	for (args = name; *args && *args != ':'; ++args)
	    ;

	device *dev = s_devices;
	while (dev->name && strncmp (dev->name, name, args-name) != 0)
	    ++dev;

	VERIFY (creator = (dev->name ? dev->creator : &createSinkLogDevice));

	logstream *sink = createSinkLogDevice (0);
	logstream *log = 0;
	s_logger = sink;

	try { log = (*creator) (*args ? ++args : args); } catch (...) {;}
	if (log)
	    s_logger = log;
    }

    if (s_deleted)
	// Someone called logstream after it was destructed.  This should
	// never happen -- all logs should be constructed in local static
	// buffers and should never get destructed!
	abort ();

    return *s_logger;
}

/** Set the log state to #Error and return reference to self.  @a mode
    and @a level specify the conditions for subsequent output. */
logstream &
logstream::error (logflag &mode, int level)
{ return logger ().mode (mode, level).state (Error); }

/** Set the log state to #Warning and return reference to self.  @a
    mode and @a level specify the conditions for subsequent output. */
logstream &
logstream::warning (logflag &mode, int level)
{ return logger ().mode (mode, level).state (Warning); }

/** Set the log state to #Trace and return reference to self.  @a mode
    and @a level specify the conditions for subsequent output. */
logstream &
logstream::trace (logflag &mode, int level)
{ return logger ().mode (mode, level).state (Trace); }


#define PUT(name,arg)			\
logstream &				\
logstream::put (arg)			\
{					\
    if (m_enabled && m_selected) {	\
	SaveErrno save;			\
	m_stream->put (name);		\
    }					\
    return *this;			\
}

#define INS(name,arg)			\
logstream &				\
logstream::operator<< (arg)		\
{					\
    if (m_enabled && m_selected) {	\
	SaveErrno save;			\
	*m_stream << name;		\
    }					\
    return *this;			\
}

#if HAVE_OSTREAM_CHAR_OVERLOAD
PUT(ch, signed char ch)
PUT(ch, unsigned char ch)
#else
PUT((char) ch, signed char ch)
PUT((char) ch, unsigned char ch)
#endif
PUT(ch, char ch)

#if HAVE_OSTREAM_CHAR_OVERLOAD
INS(ch, signed char ch)
INS(ch, unsigned char ch)
INS(string, const unsigned char *string)
INS(string, const signed char *string)
#else
INS((char) ch, signed char ch)
INS((char) ch, unsigned char ch)
INS((const char *) string, const unsigned char *string)
INS((const char *) string, const signed char *string)
#endif
INS(ch, char ch)
INS(string, const std::string &string)
INS(string, const char *string)
INS(n, int n)
INS(n, unsigned int n)
INS(n, long n)
INS(n, unsigned long n)
INS(n, short n)
INS(n, unsigned short n)
#if HAVE_LONG_LONG
INS(n, long long n)
INS(n, unsigned long long n)
#elif HAVE_INT64
INS(n, __int64 n)
INS(n, unsigned __int64 n)
#endif
INS(f, double f)
INS(f, float f)
#if HAVE_OSTREAM_LONG_DOUBLE_OVERLOAD
INS(f, long double f)
#else
INS((double) f, long double f)
#endif
#if HAVE_NEW_IOSTREAMS
INS(fn, std::ios & (*fn) (std::ios &))
INS(fn, std::ios_base & (*fn) (std::ios_base &))
INS(fn, std::ostream & (*fn) (std::ostream &))
INS(buffer, std::streambuf *buffer)
#else
INS(fn, ios & (*fn) (ios &))
INS(buffer, streambuf *buffer)
#endif

#undef PUT
#undef INS

// Special cases not handled by the generic form.
#if HAVE_BOOL
logstream &
logstream::operator<< (bool b)
{
    if (m_enabled && m_selected)
    {
	SaveErrno save;
	*m_stream << (b ? "true" : "false");
    }
    return *this;
}
#endif

logstream &
logstream::operator<< (const void *pointer)
{
    if (m_enabled && m_selected)
    {
	SaveErrno save;
	*m_stream << (void *) pointer;
    }
    return *this;
}

logstream &
logstream::operator<< (logstream & (*fn) (logstream &))
{
    if (m_enabled && m_selected)
    {
	SaveErrno save;
	return (*fn) (*this);
    }
    else
	return *this;
}

logstream &
logstream::flush (void)
{
    if (m_enabled && m_selected)
    {
	SaveErrno save;
	m_stream->flush ();
    }
    return *this;
}

/** Emit @a n spaces.  @a n must be non-negative.  */
void
logstream::pad (int n)
{
    ASSERT (n >= 0);

    // The funny `spaces' is here to avoid an emit call for every single
    // space character.  It ain't elegant, but there isn't much of a choice
    // since we cannot allocate memory.
    static const char	spaces [] = "                                ";
    static const int	N_SPACES = sizeof (spaces)-1;

    for (int i = 0; i < n / N_SPACES; ++i)
	emit (spaces, N_SPACES);

    if (n % N_SPACES)
	emit (spaces, n % N_SPACES);
}

/** Readjust the stream: print the line title and pad to the current
    indentation level when we get new output after a new-line.  */
void
logstream::readjust (void)
{
    // Avoid infinite recursion in case an ASSERT fails deep inside.
    m_newline = false;

    // Print the daemonish long header if requested
    if (m_longLineHeader)
    {
	static const int MAX_SIZE
	    = 4+1 + 2+1 + 2+1 // `yyyy-dd-mm '
	    + 2+1 + 2+1 + 2+1 // `hh-mm-ss '
	    + 1+BitTraits<long>::Digits+2+1; // `[pid] \0'

	char	buffer [MAX_SIZE];
	Time	current = Time::current ();

	emit (buffer,
	      sprintf (buffer, "%04d-%02d-%02d %02d.%02d.%02d [%ld] ",
		       current.year (true), current.month (true),
		       current.day (true), current.hour (true),
		       current.minute (true), current.second (true),
		       (long) ProcessInfo::pid ()));
    }

    // Print the mode name and state, padded out the correct length.
    // Then pad out the indent level.  There is no way to know the
    // maximum length of the mode name, so emit it separately.
    static const int	N_STATE = 10;
    const char		*state = (m_state == Trace   ? "/       : " :
				  m_state == Warning ? "/warning: " :
				  /* Error */          "/error  : ");

    emit (m_mode->name, m_modeNameLength);
    pad (m_longestModeName - m_modeNameLength);
    emit (state, N_STATE);

    pad (m_indent);
}

} // namespace lat
