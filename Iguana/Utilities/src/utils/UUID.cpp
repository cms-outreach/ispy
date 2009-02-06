//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/UUID.h"
#include "classlib/utils/UUIDError.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/DebugAids.h"
#define UUID INTERNAL_UUID	// Avoid conflicts with Windows headers.
#include "uuid.h"
#undef UUID
#ifdef _WIN32
# include <windows.h>
#endif

#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

/** Output a UUID @a id into stream @a out.  Uses #UUID::format() to
    format @a id into the standard 36-character form.  Returns @a out
    after output.  */
std::ostream &
operator<< (std::ostream &out, const UUID &id)
{
    char buf [37];
    id.format (buf);
    ASSERT (strlen(buf)==36);
    return out << buf;
}

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Compare UUIDs for equality.  Returns @c true if @c this and @a x
    are the same UUID, @c false otherwise.  */
bool
UUID::operator== (const UUID &x) const
{ return uuid_compare (m_uuid, x.m_uuid) == 0; }

/** Compare UUIDs for equality.  Returns @c true if @c this and @a x
    are not the same UUID, @c false otherwise.  */
bool
UUID::operator!= (const UUID &x) const
{ return uuid_compare (m_uuid, x.m_uuid) != 0; }

/** Order UUIDs.  Returns @c true if @c this is less than @a x
    according to lexicographic ordering rules, @c false otherwise.  */
bool
UUID::operator< (const UUID &x) const
{ return uuid_compare (m_uuid, x.m_uuid) < 0; }

/** Format the UUID into the buffer @a buf, which must have space at
    least for 37 bytes: 36 bytes for the string plus the terminating
    null.  */
void
UUID::format (char *buf) const
{
    ASSERT (DebugAids::validate (buf, 37, true));
    uuid_unparse (m_uuid, buf);
}

/** Construct a null UUID.  */
UUID::UUID (void)
{ uuid_clear (m_uuid); }

/** Construct a UUID by parsing the string @a text, which must be in
    the textual 37-byte format (36 characters terminated by a null).  */
UUID::UUID (const char *text)
{ if (uuid_parse (text, m_uuid)) throw UUIDError (); }

/** Construct a UUID by parsing the string @a text, which must be in
    the textual 37-byte format (36 characters terminated by a null).  */
UUID::UUID (const std::string &text)
{ if (uuid_parse (text.c_str (), m_uuid)) throw UUIDError (); }

/** Construct a UUID from the binary representation @a raw.  */
UUID::UUID (Binary raw)
{ uuid_copy (m_uuid, raw); }

/** Copy UUID @a x.  */
UUID::UUID (const UUID &x)
{ uuid_copy (m_uuid, x.m_uuid); }

/** Copy UUID @a x.  Returns reference to @c this after copy.  */
UUID &
UUID::operator= (const UUID &x)
{ uuid_copy (m_uuid, x.m_uuid); return *this; }

//////////////////////////////////////////////////////////////////////
/** Check if the UUID is null: all zeroes.  */
bool
UUID::isNull (void) const
{ return uuid_is_null (m_uuid); }

/** Return the time component of the UUID.  */
Time
UUID::time (void) const
{
    timeval tv;
    uuid_time (m_uuid, &tv);
    return Time ((Time::ValueType) (unsigned long) tv.tv_sec,
		 (Time::ValueType) (unsigned long) tv.tv_usec * 1000);
}

/** Return the type component of the UUID.  */
int
UUID::type (void) const
{ return uuid_type (m_uuid); }

/** Return the variant component of the UUID.  */
int
UUID::variant (void) const
{ return uuid_variant (m_uuid); }

//////////////////////////////////////////////////////////////////////
/** Generate a new universally unique identifier (UUID).  If there is
    a high-quality randomness source such as @c /dev/urandom, it is
    used to produce a random identifier.  Otherwise the current time,
    the local ethernet MAC address (if available) and pseudo-random
    data are used.  This is the preferred method of creating unique
    identifiers.  To force the selection of the algorithm, please use
    #generateRandom() or #generateTime().  */
UUID
UUID::generate (void)
{ Binary id; uuid_generate (id); return UUID (id); }

/** Generate a new all-random universally unique identifier (UUID).  A
    high-quality randomness source such as @c /dev/urandom is used if
    one exists, otherwise a pseudo-random number generator is
    substituted.  Note that the use of pseudo-random number generator
    may compromise the uniqueness of the resulting identifiers.  It is
    usually better to use #generate(), but please do note the issues
    that #generateTime() has -- it will be used if no high-quality
    randomness source exists.  */
UUID
UUID::generateRandom (void)
{ Binary id; uuid_generate_random (id); return UUID (id); }

/** Generate a new universally unique identifier (UUID) from current
    time and local ethernet MAC address (if available).  This method
    can leak information about where the origin of the identifier
    because the result includes the source MAC address.  This may be a
    privacy problem in some applications.  In general it is better to
    use #generate(), which uses this method only if no high-quality
    randomness source exists.  */
UUID
UUID::generateTime (void)
{ Binary id; uuid_generate_time (id); return UUID (id); }

} // namespace lat
