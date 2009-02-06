//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/zip/ZipMember.h"
#include "classlib/utils/Time.h"
#include <cstring>

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

ZipMember::ZipMember (Filename name)
    : m_name (name),
      m_position (0),
      m_xlocal (0),
      m_xlocalLen (0),
      m_xglobal (0),
      m_xglobalLen (0),
      m_time (0),
      m_csize (0),
      m_dsize (0),
      m_method (NONE),
      m_level (0),
      m_crc32 (0),
      m_attrInt (0),
      m_attrExt (0),
      m_directory (false)
{}

ZipMember::ZipMember (const ZipMember &x)
    : m_name (x.m_name),
      m_comment (x.m_comment),
      m_position (x.m_position),
      m_xlocal (0),
      m_xlocalLen (x.m_xlocalLen),
      m_xglobal (0),
      m_xglobalLen (x.m_xglobalLen),
      m_time (x.m_time),
      m_csize (x.m_csize),
      m_dsize (x.m_dsize),
      m_method (x.m_method),
      m_level (x.m_level),
      m_crc32 (x.m_crc32),
      m_attrInt (x.m_attrInt),
      m_attrExt (x.m_attrExt),
      m_directory (x.m_directory)
{
    ASSERT (x.m_xlocal || ! x.m_xlocalLen);
    ASSERT (x.m_xglobal || ! x.m_xglobalLen);

    if (x.m_xlocal)
    {
	m_xlocal = new unsigned char [m_xlocalLen];
	memcpy (m_xlocal, x.m_xlocal, m_xlocalLen);
    }

    if (x.m_xglobal)
    {
	m_xglobal = new unsigned char [m_xglobalLen];
	memcpy (m_xglobal, x.m_xglobal, m_xglobalLen);
    }
}

ZipMember::~ZipMember (void)
{
    delete [] (unsigned char *) m_xlocal;
    delete [] (unsigned char *) m_xglobal;
}

ZipMember &
ZipMember::operator= (const ZipMember &x)
{
    if (this == &x)
	return *this;

    delete [] (unsigned char *) m_xlocal;
    delete [] (unsigned char *) m_xglobal;

    m_name	= x.m_name;
    m_comment	= x.m_comment;
    m_position	= x.m_position;
    m_xlocal	= 0;
    m_xlocalLen	= x.m_xlocalLen;
    m_xglobal	= 0;
    m_xglobalLen = x.m_xglobalLen;
    m_time	= x.m_time;
    m_csize	= x.m_csize;
    m_dsize	= x.m_dsize;
    m_method	= x.m_method;
    m_level	= x.m_level;
    m_crc32	= x.m_crc32;
    m_attrInt	= x.m_attrInt;
    m_attrExt	= x.m_attrExt;
    m_directory = x.m_directory;

    ASSERT (x.m_xlocal || ! x.m_xlocalLen);
    ASSERT (x.m_xglobal || ! x.m_xglobalLen);

    if (x.m_xlocal)
    {
	m_xlocal = new unsigned char [m_xlocalLen];
	memcpy (m_xlocal, x.m_xlocal, m_xlocalLen);
    }

    if (x.m_xglobal)
    {
	m_xglobal = new unsigned char [m_xglobalLen];
	memcpy (m_xglobal, x.m_xglobal, m_xglobalLen);
    }

    return *this;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Filename
ZipMember::name (void) const
{ return m_name; }

    // internal state for ZipArchive reads; undefined for writes and streams
IOOffset
ZipMember::position (void) const
{ return m_position; }

void
ZipMember::position (IOOffset value)
{ ASSERT (value <= IOOffset (MAX_SIZE)); m_position = value; }

IOOffset
ZipMember::size (SizeType which) const
{ return which == COMPRESSED ? m_csize : m_dsize; }

void
ZipMember::size (SizeType which, IOOffset value)
{
    ASSERT (value <= IOOffset (MAX_SIZE));
    (which == COMPRESSED ? m_csize : m_dsize) = value;
}

unsigned
ZipMember::crc32 (void) const
{ return m_crc32; }

void
ZipMember::crc32 (unsigned value)
{ m_crc32 = value; }

//////////////////////////////////////////////////////////////////////
std::string
ZipMember::comment (void) const
{ return m_comment; }

void
ZipMember::comment (const std::string &value)
{ ASSERT (value.length () <= MAX_VAR_LEN); m_comment = value; }

Time
ZipMember::time (void) const
{ return m_time; }

void
ZipMember::time (Time value)
{ m_time = value; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
unsigned
ZipMember::method (void) const
{ return m_method; }

void
ZipMember::method (Method value)
{
    m_method = value;
    if (m_method == DEFLATED && m_level == 0)
	m_level = 3;
}

unsigned
ZipMember::level (void) const
{ return m_level; }

void
ZipMember::level (unsigned value)
{
    ASSERT (! value || m_method == DEFLATED);
    ASSERT (value <= BEST_COMPRESSION);
    m_level = value;
}

unsigned
ZipMember::flags (void) const
{
    // level = (flag & 6)/2;
    //    0 => Defl:N (Z-level=3-7) (normal);
    //    1 => Defl:X (Z-level=8-9) (best);
    //    2 => Defl:F (Z-level=2)   (fast);
    //    3 => Defl:F (Z-level=1)   (extra fast);

    // FIXME: directory?
    return (m_method == DEFLATED
	    ? (m_level >= 8 ? 2
	       : m_level >= 3 ? 0
	       : m_level >= 2 ? 4
	       : m_level >= 1 ? 6 : 0)
	    : 0);
}

bool
ZipMember::isDirectory (void) const
{ return m_directory; }

void
ZipMember::isDirectory (bool value)
{ ASSERT (m_method != DEFLATED); m_directory = value; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// FIXME: unavailable in stream reading
unsigned
ZipMember::attributes (Attribute which /* = EXTERNAL */) const
{ return which == EXTERNAL ? m_attrExt : m_attrInt; }

void
ZipMember::attributes (Attribute which, unsigned value)
{ (which == EXTERNAL ? m_attrExt : m_attrInt) = value; }

void
ZipMember::extra (ExtraField which, void **buffer, IOSize *size) const
{
    *size = (which == LOCAL ? m_xlocalLen : m_xglobalLen);
    if (buffer) *buffer = (which == LOCAL ? m_xlocal : m_xglobal);
}

void
ZipMember::extra (ExtraField which, void *buffer, IOSize size)
{
    ASSERT (! size || buffer);
    ASSERT (! buffer || DebugAids::validate (buffer, size, false));
    ASSERT (size <= MAX_VAR_LEN);

    if (which == LOCAL)
    {
	delete [] (unsigned char *) m_xlocal;
	m_xlocal = new unsigned char [m_xlocalLen = size];
	memcpy (m_xlocal, buffer, size);
    }
    else
    {
	delete [] (unsigned char *) m_xglobal;
	m_xglobal = new unsigned char [m_xglobalLen = size];
	memcpy (m_xglobal, buffer, size);
    }
}

} // namespace lat
