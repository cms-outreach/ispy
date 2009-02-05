//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/iobase/FileAcl.h"

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

FileAcl::FileAcl (void)
    : m_owner (UID (-1)),
      m_group (GID (-1)),
      m_perms (0)
{}

FileAcl::FileAcl (int unixPerms,
		  UID uid /* = UID (-1) */,
		  GID gid /* = GID (-1) */)
    : m_owner (uid),
      m_group (gid),
      m_perms (unixPerms)
{}

//////////////////////////////////////////////////////////////////////
UID
FileAcl::owner (void) const
{ return m_owner; }

void
FileAcl::owner (UID uid)
{ m_owner = uid; }

//////////////////////////////////////////////////////////////////////
GID
FileAcl::group (void) const
{ return m_group; }

void
FileAcl::group (GID gid)
{ m_group = gid; }

//////////////////////////////////////////////////////////////////////
int
FileAcl::unix (void) const
{ return m_perms; }

int
FileAcl::addUnix (int mask)
{ return m_perms |= mask; }

//////////////////////////////////////////////////////////////////////
bool
FileAcl::readonly (void) const
{ return ! (m_perms & WriteBits); }

} // namespace lat
