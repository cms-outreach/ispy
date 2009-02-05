//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/IOChannelOutputStream.h"
#include "classlib/iobase/IOChannel.h"
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

IOChannelOutputStream::IOChannelOutputStream (IOChannel *output)
    : m_output (output)
{ ASSERT (m_output); }

// doesn't delete but closes the output
IOChannelOutputStream::~IOChannelOutputStream (void)
{ /* close (); */ }

IOSize
IOChannelOutputStream::write (const void *from, IOSize n)
{ return m_output->write (from, n); }

IOSize
IOChannelOutputStream::writev (const IOBuffer *from, IOSize buffers)
{ return m_output->writev (from, buffers); }

void
IOChannelOutputStream::close (void)
{ m_output->close (); /* FIXME: delete m_output; m_output = 0; */ }

} // namespace lat
