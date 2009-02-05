//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iotools/FilterInputStream.h"
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

/** Constructs a new filter stream using @a input as the underlying
    stream object.  The class takes ownership of @a input.  */
FilterInputStream::FilterInputStream (InputStream *input)
    : m_input (input)
{}

/** Destructs the filter stream.  Deletes the underlying stream.  */
FilterInputStream::~FilterInputStream (void)
{ ASSERT (m_input); /* FIXME: delete m_input; */ }

/** Check if more input is available.  Forwarded upstream.  */
IOSize
FilterInputStream::available (void)
{ ASSERT (m_input); return m_input->available (); }

/** Read data from upstream.  */
IOSize
FilterInputStream::read (void *into, IOSize n)
{ ASSERT (m_input); return m_input->read (into, n); }

/** Skip @a n bytes upstream.  */
IOSize
FilterInputStream::skip (IOSize n)
{ ASSERT (m_input); return m_input->skip (n); /* FIXME: Read? */ }

/** Close the upstream input.   */
void
FilterInputStream::close (void)
{ ASSERT (m_input); m_input->close (); }

/** Reset the upstream input.  */
void
FilterInputStream::reset (void)
{ ASSERT (m_input); m_input->reset (); }

} // namespace lat
