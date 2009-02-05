//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/NamedPipeServer.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/NamedPipeServer.h"

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

/** Create a new named pipe object without a pipe attached to it.  */
NamedPipeServer::NamedPipeServer (void)
    : m_handle (IOFD_INVALID)
{}

/** Create a new named pipe.  The pipe will be give access permissions
    @a access.  */
NamedPipeServer::NamedPipeServer (const std::string &name, FileAcl access /* = 0666 */)
    : m_handle (IOFD_INVALID)
{ create (name.c_str (), access); }

/** Create a new named pipe.  The pipe will be give access permissions
    @a access.  */
NamedPipeServer::NamedPipeServer (const char *name, FileAcl access /* = 0666 */)
    : m_handle (IOFD_INVALID)
{ create (name, access); }

/** Destroy the named pipe object.  The client must have already
    called either #close() or #detach().  */
NamedPipeServer::~NamedPipeServer (void)
{ ASSERT (m_name.empty ()); ASSERT (m_handle == IOFD_INVALID); }

/** Create a new named pipe in the operating system.  Does not yet
    connect to it.  Redirected to the overloaded method taking a
    "const char *" name.  (FIXME: Can windows have named pipes that
    nobody is connected to?) */
void
NamedPipeServer::create (const std::string &name, FileAcl access /* = 0666 */)
{ create (name.c_str (), access); }

/** Create a new named pipe in the operating system.  Does not yet
    connect to it.  (FIXME: Can windows have named pipes that nobody
    is connected to?) */
void
NamedPipeServer::create (const char *name, FileAcl access /* = 0666 */)
{
    ASSERT (name && *name);
    close ();
    sysinit (name, access);
    m_name = name;
}

void
NamedPipeServer::close (void)
{
    if (! m_name.empty ())
    {
	sysclose ();
	m_name = "";
    }
}

void
NamedPipeServer::detach (void)
{
    ASSERT (! m_name.empty ());
    sysdetach ();
    m_name = "";
}

} // namespace lat
