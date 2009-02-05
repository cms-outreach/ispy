//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/PipedOutputStream.h"
#include "classlib/iotools/PipedInputStream.h"
#include "classlib/iobase/Pipe.h"
#include "classlib/utils/DebugAids.h"
#include <algorithm>
#include <cstdlib>

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

/** Allocate a new pipe stream.  Connects to @a input if it is non-null.  */
PipedOutputStream::PipedOutputStream (PipedInputStream *input /* = 0 */)
    : m_link (0),
      m_pipe (0)
{ connect (input); }

PipedOutputStream::~PipedOutputStream (void)
{ connect (0); }

void
PipedOutputStream::connect (PipedInputStream *link)
{
    if (m_link)
    {
	if (link)
	    // Nothing to do here, cut recursion
	    ASSERT (link == m_link);
	else
	{
	    PipedInputStream *old = m_link;
	    m_link = 0;
            old->connect (0);

	    // Close both ends of the pipe, nobody is interested in it
	    m_pipe->sink ()->close ();
	    m_pipe->source ()->close ();
	    delete m_pipe;
	    m_pipe = 0;
        }
    }
    else
    {
	if (! link)
	    // Nothing to do here, cut recursion
	    ;
	else
	{
	    // Create the pipe
	    ASSERT (! m_pipe);
	    m_pipe = new Pipe;

	    // Establish link
	    m_link = link;
	    m_link->connect (this);
	}
    }
}

Pipe *
PipedOutputStream::pipe (void) const
{ return m_pipe; }

/** Write to the internal pipe.  The write may block until there
    is more space in system pipe buffers to receive the data.  */
IOSize
PipedOutputStream::write (const void *from, IOSize n)
{ ASSERT (m_pipe); return m_pipe->sink ()->xwrite (from, n); }

/** Close the output.  Closes the write end of the pipe but not the
    read end nor does not remove link to the #PipedInputStream.  */
void
PipedOutputStream::close (void)
{ ASSERT (m_pipe); m_pipe->sink ()->close (); }

} // namespace lat
