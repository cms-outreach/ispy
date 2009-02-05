//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/StdOutputStream.h"
#include "classlib/iotools/StdError.h"
#include "classlib/utils/DebugAids.h"

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

StdOutputStream::StdOutputStream (std::ostream *output)
    : m_output (output)
{}

StdOutputStream::~StdOutputStream (void)
{ /* FIXME: delete m_output; */ }

IOSize
StdOutputStream::write (const void *from, IOSize n)
{
    ASSERT (m_output);
#if ! HAVE_NEW_IOSTREAMS
    IOSize before = m_output->tellp ();
#endif
    if (m_output->write ((char *) from, n).fail ())
	throw StdError (m_output->rdstate ());
    else
    {
#if HAVE_NEW_IOSTREAMS
	// FIXME: The old stream solution seems to always return zero?
	// How do we find out with the new streams what was actually
	// written?
	return n;
#else
	return IOSize (m_output->tellp ()) - before;
#endif
    }
}

void
StdOutputStream::finish (void)
{
    ASSERT (m_output);
    if (m_output->flush ().fail ())
	throw StdError (m_output->rdstate ());
}

void
StdOutputStream::close (void)
{ ASSERT (m_output); /* FIXME: delete m_output; m_output = 0; */ }

} // namespace lat
