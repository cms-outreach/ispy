//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/PipedInputStream.h"
#include "classlib/iotools/PipedOutputStream.h"
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

/** Allocate a new input stream with read-ahead buffer of @a size bytes.  */
PipedInputStream::PipedInputStream (PipedOutputStream *output /* = 0 */)
    : m_link (0)
{ connect (output); }

PipedInputStream::~PipedInputStream (void)
{ connect (0); }

void
PipedInputStream::connect (PipedOutputStream *link)
{
    if (m_link)
    {
	if (link)
	    // Nothing to do here, cut recursion
	    ASSERT (link == m_link);
	else
	{
	    PipedOutputStream *old = m_link;
	    m_link = 0;
            old->connect (0);
        }
    }
    else
    {
	if (! link)
	    // Nothing to do here, cut recursion
	    ;
	else
	{
	    m_link = link;
	    m_link->connect (this);
	    ASSERT (m_link->pipe ());
	    ASSERT (m_link->pipe ()->source ());
	    ASSERT (m_link->pipe ()->source ()->fd () != IOFD_INVALID);
	}
    }
}

/** Read data from the pipe.  This may block until there is data
    to read.  */
IOSize
PipedInputStream::read (void *into, IOSize n)
{
    ASSERT (m_link);
    ASSERT (m_link->pipe ());
    ASSERT (m_link->pipe ()->source ());
    ASSERT (m_link->pipe ()->source ()->fd () != IOFD_INVALID);
    return m_link->pipe ()->source ()->read (into, n);
}

/** Close the read end of the pipe.  Does not close the write end
    nor does not disconnect from the output side.  */
void
PipedInputStream::close (void)
{ if (m_link) m_link->pipe ()->source ()->close (); }

} // namespace lat
