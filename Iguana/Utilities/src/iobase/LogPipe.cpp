//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/Log.h"
#include "classlib/utils/Error.h"
#include "classlib/iobase/PipeCmd.h"
#if HAVE_NEW
# include <new>
#elif HAVE_NEW_H
# include <new.h>
#endif

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** Logging device sending all output to another process via a pipe. */
class PipeLog : public logstream
{
public:
    PipeLog (void);
    ~PipeLog (void);

    logstream *		initialise (const char *command);

protected:
    virtual void	emit (const char *string, int length);

private:
    void		close (void);

    PipeCmd		m_pipe; //< The pipe
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

logstream *
logstream::createPipeLogDevice (const char *args)
{
    // make sure the log stays around beyond destructors
    static char		data[sizeof(PipeLog)];
    static PipeLog	*log = new (&data[0]) PipeLog;
    return log->initialise (args);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/** Default-initialise the log device. */
PipeLog::PipeLog (void)
{}

/** Start up the command @a name as the pipe end point. */
logstream *
PipeLog::initialise (const char *command)
{
    try
    {
	m_pipe.open (command, IOFlags::OpenWrite);
	m_pipe.xwrite ("\n--- Log opened\n", 16);
    }
//      catch (Error &e)
//      {
//  	return 0;
//      }
    catch (...)
    {
	return 0;
    }

    return this;
}

/** Flush and close the log.  */
PipeLog::~PipeLog (void)
{ flush (); close (); }

/** Dump output from @a string for @a length bytes to the file.  */
void PipeLog::emit (const char *string, int length)
{ try { m_pipe.xwrite (string, length); } catch (...) {;} }

/* Shut down the coprocess connection. */
void PipeLog::close (void)
{
    emit ("\n\nLog closed, bailing out\n", 26);
    try { m_pipe.close (); } catch (...) {;}
}

} // namespace lat
