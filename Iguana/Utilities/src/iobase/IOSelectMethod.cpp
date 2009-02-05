//<<<<<< INCLUDES                                                       >>>>>>

#include "src/iobase/IOSelectMethod.h"

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

IOSelectMethod::~IOSelectMethod (void) {}
void IOSelectMethod::setSignal (int) {}

#if 0
size_t
IOSelectMethod::findReq (IOFD fd)
{
    for (size_t i = 0; i < m_requests.size (); ++i)
	if (m_requests [i].fd == fd)
	    return i;

    ASSERT (false);
    return m_requests.size ();
}
#endif

size_t
IOSelectMethod::addReq (const IOSelectRequest &req)
{
    m_requests.push_back (req);
    return m_requests.size () - 1;
}

void
IOSelectMethod::removeReq (const IOSelectRequest &req, size_t index)
{
    ASSERT (index < m_requests.size ());
    ASSERT (m_requests [index].fd == req.fd);
    ASSERT (m_requests [index].channel == req.channel);
    m_requests.erase (m_requests.begin () + index);
}

#if 0
void
IOSelectMethod::removeReq (const IOSelectRequest &req)
{ removeReq (req, findReq (req.fd)); }
#endif

void
IOSelectMethod::clearReqs (void)
{ std::vector<IOSelectRequest> junk; std::swap (m_requests, junk); }

} // namespace lat
