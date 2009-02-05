//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/Log.h" 
#if HAVE_NEW
# include <new>
#elif HAVE_NEW_H
# include <new.h>
#endif

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/** Logging device that discard all output.  #logstream uses this as a
    fallback when the user-specified device fails to initialise.  It
    may also be used explicitly at run time via the @c $LOG.  */
class SinkLog : public logstream
{
public:
    SinkLog (void);
    ~SinkLog (void);

protected:
    virtual void	emit (const char *string, int length);
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

logstream *
logstream::createSinkLogDevice (const char *)
{
    // make sure the log stays around beyond destructors
    static char		data[sizeof(SinkLog)];
    static SinkLog	*log = new (&data[0]) SinkLog;
    return log;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/** Initialise the sink.  */
SinkLog::SinkLog (void)
{ }

/** No-op.  */
SinkLog::~SinkLog (void)
{ }

/** Discard output. */
void
SinkLog::emit (const char *, int)
{ }

} // namespace lat
