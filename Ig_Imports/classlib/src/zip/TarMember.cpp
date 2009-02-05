//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/TarMember.h"

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

TarMember::TarMember (Filename name)
    : m_name (name), // FIXME: check length?
      m_mtime (0),
      m_size (0),
      m_mode (0),
      m_uid (0),
      m_gid (0),
      m_type (REGULAR),
      m_devmajor (0),
      m_devminor (0),
      m_ok (true)
{}

TarMember::TarMember (const TarMember &x)
    : m_name (x.m_name), // FIXME: check length?
      m_mtime (x.m_mtime),
      m_size (x.m_size),
      m_mode (x.m_mode),
      m_uid (x.m_uid),
      m_uname (x.m_uname),
      m_gid (x.m_gid),
      m_gname (x.m_gname),
      m_type (x.m_type),
      m_link (x.m_link),
      m_devmajor (x.m_devmajor),
      m_devminor (x.m_devminor),
      m_ok (x.m_ok)
{}

TarMember::~TarMember (void)
{}

TarMember &
TarMember::operator= (const TarMember &x)
{
    m_name	= x.m_name;
    m_mtime	= x.m_mtime;
    m_size	= x.m_size;
    m_mode	= x.m_mode;
    m_uid	= x.m_uid;
    m_uname	= x.m_uname;
    m_gid	= x.m_gid;
    m_gname	= x.m_gname;
    m_type	= x.m_type;
    m_link	= x.m_link;
    m_devmajor	= x.m_devmajor;
    m_devminor	= x.m_devminor;
    m_ok	= x.m_ok;
    return *this;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Filename
TarMember::name (void) const
{ return m_name; }

void
TarMember::name (Filename name)
{ m_name = name; } // FIXME: check length?

//////////////////////////////////////////////////////////////////////
int
TarMember::mode (void) const
{ return m_mode; }

void
TarMember::mode (int value)
{ m_mode = value; } // FIXME: check against permitted values?

//////////////////////////////////////////////////////////////////////
unsigned
TarMember::mtime (void) const
{ return m_mtime; }

void
TarMember::mtime (unsigned value)
{ m_mtime = value; }

//////////////////////////////////////////////////////////////////////
IOOffset
TarMember::size (void) const
{ return m_size; }

void
TarMember::size (IOOffset value)
{ m_size = value; }

// If typeflag=={LNKTYPE,SYMTYPE,DIRTYPE} then size must be 0.

//////////////////////////////////////////////////////////////////////
int
TarMember::uid (void) const
{ return m_uid; }

void
TarMember::uid (int value)
{ m_uid = value; }

//////////////////////////////////////////////////////////////////////
std::string
TarMember::uname (void) const
{ return m_uname; }

void
TarMember::uname (const std::string &value)
{ m_uname = value; } // FIXME: check length?

//////////////////////////////////////////////////////////////////////
int
TarMember::gid (void) const
{ return m_gid; }

void
TarMember::gid (int value)
{ m_gid = value; }

//////////////////////////////////////////////////////////////////////
std::string
TarMember::gname (void) const
{ return m_gname; }

void
TarMember::gname (const std::string &value)
{ m_gname = value; } // FIXME: check length?

//////////////////////////////////////////////////////////////////////
// If typeflag=={LNKTYPE,SYMTYPE,DIRTYPE} then size must be 0.
// linkname is only valid when typeflag==LNKTYPE.
// devmajor and devminor are only valid for typeflag=={BLKTYPE,CHRTYPE}
TarConstants::Type
TarMember::type (void) const
{ return m_type; }

void
TarMember::type (Type value)
{ m_type = value; } // FIXME: validate?

void
TarMember::type (char value)
{ m_type = (Type) value; } // FIXME: validate?

//////////////////////////////////////////////////////////////////////
// linkname is only valid when typeflag==LNKTYPE.
Filename
TarMember::linkname (void) const
{ return m_link; }

void
TarMember::linkname (Filename value)
{ m_link = value; } // FIXME: set type? check length?

//////////////////////////////////////////////////////////////////////
// devmajor and devminor are only valid for typeflag=={BLKTYPE,CHRTYPE}
int
TarMember::devmajor (void) const
{ return m_devmajor; }

int
TarMember::devminor (void) const
{ return m_devminor; }

void
TarMember::device (int major, int minor)
{ m_devmajor = major; m_devminor = minor; }

//////////////////////////////////////////////////////////////////////
// chksum contains the sum of all 512 bytes in the header block,
// treating each byte as an 8-bit unsigned value and treating the
// 8 bytes of chksum as blank characters.

} // namespace lat
