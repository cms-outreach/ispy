//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/StringOps.h"
#include "classlib/utils/Regexp.h"
#include "classlib/utils/DebugAids.h"
#include <cstring>
#include <cctype>

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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Compare two character strings @a x and @a y of length @a len for
    equality case insensitively.  This is like @c memcmp except for
    the case sensitivity, and like @c strncasecmp except it keep going
    despite null characters.  */
int
StringOps::imemcmp (const char *x, const char *y, size_t len)
{
    int diff;
    for (size_t i = 0; i < len; ++i)
	if ((diff = tolower ((unsigned char) x[i])
	     - tolower ((unsigned char) y[i])))
	    return diff;

    return 0;
}

/** Find @a what in string @a s.  The search begins at @a offset,
    which must be at least zero and less than @a slen, the length of
    @a s.  The search is case-insensitive if @a caseless is @c true.
    Returns the index at which first @a what was found, or -1 if the
    search fails.  */
int
StringOps::rawfind (const char		*s,
		    size_t		slen,
		    char		what,
		    int			offset /* = 0 */,
		    bool		caseless /* = false */)
{
    ASSERT (offset >= 0);
    ASSERT ((size_t) offset <= slen);

    const char *begin = s;

    if (caseless)
    {
	const char *end = begin + slen;
	const char *p = begin + offset;

	for (int ch = tolower ((unsigned char) what); p != end; ++p)
	    if (tolower ((unsigned char) *p) == ch)
		return p - begin;

	return -1;
    }
    else
    {
	const char *p = (const char *) memchr (begin + offset, what,
					       slen - offset);
	return p ? p - begin : -1;
    }
}

/** Find @a what, a string of @a len bytes, in string @a s.  The
    search begins at @a offset, which must be at least zero and less
    than slen, the length of @a s.  The search is case-insensitive if
    @a caseless is @c true.  Returns the index at which first @a what
    was found, or -1 if the search fails.  */
int
StringOps::rawfind (const char		*s,
		    size_t		slen,
		    const char		*what,
		    size_t		len,
		    int			offset /* = 0 */,
		    bool		caseless /* = false */)
{
    ASSERT (offset >= 0);
    ASSERT ((size_t) offset <= slen);
    ASSERT (what || !len);

    if (slen - offset < len)
	return -1;

    if (! what)
	return offset;

    const char *begin = s;
    const char *end = begin + slen;
    const char *p = begin + offset;

    if (caseless)
    {
	for ( ; p <= end-len; ++p)
	    if (! imemcmp (p, what, len))
		return p - begin;
    }
    else
    {
	for ( ; p <= end-len; ++p)
	    if (! memcmp (p, what, len))
		return p - begin;
    }

    return -1;
}

//////////////////////////////////////////////////////////////////////
/** Find @a what in string @a s.  The search begins at @a offset,
    which must be at least zero and less than @a slen, the length of
    @a s, and works backwards towards the beginning of @a s.  The
    search is case-insensitive if @a caseless is @c true.  Returns the
    index at which first @a what was found, or -1 if the search
    fails.  */
int
StringOps::rawrfind (const char		*s,
		     size_t		slen,
		     char		what,
		     int		offset,
		     bool		caseless /* = false */)
{
    ASSERT (offset >= 0);
    ASSERT ((size_t) offset <= slen);

    const char *begin = s;
    const char *p = begin + offset + 1;

    if (caseless)
    {
	for (int ch = tolower ((unsigned char) what); p != begin; --p)
	    if (tolower ((unsigned char) p[-1]) == ch)
		return p - begin - 1;
    }
    else
    {
	// NB: There's no memrchr, and we can't use strrchr as it
	// would stop at null bytes.
	for ( ; p != begin; --p)
	    if (p[-1] == what)
		return p - begin - 1;
    }

    return -1;
}

/** Find @a what, a string of @a len bytes, in string @a s.  The
    search begins at @a offset, which must be at least zero and less
    than @a slen, the length of @a s, and works backwards towards the
    beginning of @a s.  The search is case-insensitive if @a caseless
    is @c true.  Returns the index at which first @a what was found,
    or -1 if the search fails.  */
int
StringOps::rawrfind (const char		*s,
		     size_t		slen,
		     const char		*what,
		     size_t		len,
		     int		offset,
		     bool		caseless /* = false */)
{
    ASSERT (offset >= 0);
    ASSERT ((size_t) offset <= slen);
    ASSERT (!what || !len);

    if (slen < len)
	return -1;

    if (slen - len < (size_t) offset)
	offset = slen - len;

    if (! what)
	return offset;

    const char *begin = s;
    const char *p = begin + offset + 1;

    if (caseless)
    {
	for ( ; p != begin; --p)
	    if (! imemcmp (p-1, what, len))
		return p - begin - 1;
    }
    else
    {
	for ( ; p != begin; --p)
	    if (! memcmp (p-1, what, len))
		return p - begin - 1;
    }

    return -1;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Compute the largest field we are going to be interested in from @a
    nmax, @a first, and @a last.  If any of them are negative, we have
    to determine all the fields.  Otherwise we are interested in up to
    the larger of @a nmax and @a last (@a first must be less than @a
    last).  */
inline size_t
StringOps::splitmax (int nmax, int first, int last)
{
    ASSERT (nmax >= 0);

    if (first < 0 || last < 0)
	return 0;

    ASSERT (last == 0 || first <= last);

    return nmax > last ? nmax : last;
}

    

StringOps::FieldList
StringOps::splitpos (const char		*s,
		     size_t		slen,
		     char		sep,
		     int		flags,
		     size_t		nmax)
{
    // FIXME: Add an option to add separator matches as items to the
    // result list; in case of rx, possibly also/only the capturing
    // subpatterns.

    // This is our return value.  It will have an even number of
    // indices, with pairs of [BEGIN, END), where BEGIN is the
    // beginning of a field and END is one past its end.
    FieldList	pos;
    
    // This marks the last non-empty field "pos".  We always push
    // everything into "pos", and then later decide whether to keep
    // them or not.  "mark" tells us how far to discard at the back of
    // the vector.
    size_t	mark = 0;

    // These two mark the current separator search location (index)
    // and where it was found (match), i.e. the begin and end of the
    // current field (but see below for explanation of begin).  We
    // need these as we step forward looking for non-empty fields.
    int		index = 0;
    int		match;

    // This marks the real beginning of the field being considered.
    // "index" is different from this only when we have matched a
    // zero-length separator; in that case we force the search to move
    // forwards by one character to make progress.
    int		begin = 0;

    // This tracks which empty fields we are interested in keeping.
    // Bit 0 (value 1) is for leading empty fields, bit 1 (value 2)
    // for leading empty and bit 2 (value 4) for trailing empty.
    // "curbit" and "lastbit" establish a state machine that tells
    // us how far we got in scanning when we bail out.
    unsigned	keep =
	((! (flags & TrimEmpty) || (flags & KeepLeadingEmpty) ? 1 : 0)
	 | (! (flags & TrimEmpty) || (flags & KeepTrailingEmpty) ? 4 : 0)
	 | (! (flags & TrimEmpty) ? 2 : 0));

    // This tracks "keep" bits for the empty fields we are currently
    // considering.  It points to leading (bit 0) as long as we are
    // seeing leading empty fields, and then moves to middle (bit 1).
    // Thus if exit having seen only empty fields, they are all
    // treated as leading empty fields.  We use this to select from
    // "keep" how we should treat the empty fields we find.
    unsigned	curbit = 1;

    // This tracks "keep" bits for the empty fields on when we exit.
    // It points initially to leading (bit 0): if all we ever see is
    // empty fields, then they are all leading empty fields.  When we
    // find the first non-empty field, we point this to trailing empty
    // (bit 2): anything following empty fields are then treated as
    // trailing empty on exit.  We use this to select from "keep" how
    // we should treat the empty fields we find.
    unsigned	lastbit = 1;

    // Keep looking as long as we can find fields.
    while ((size_t) index <= slen)
    {
	ASSERT (begin >= 0);
	ASSERT (index >= begin);

	// Find next separator match: either a legitimate separator
	// (match >= 0) or the end of the string (match == -1).  Fix
	// up the latter to avoid special handling: pretend there is a
	// separator at the end of s.  This may match a null-length
	// string at index, that's ok since it's handled in loop code
	// below (begin will be one less than index).
	if ((match = rawfind(s, slen, sep, index, flags & CaseInsensitiveSep)) == -1)
	    match = slen;

	// We have a field, determine what to do with it.  If it is
	// null, push it to pos but don't move the mark.
	if (match > begin)
	{
	    // Do we keep the empty fields we've seen so far?  Zap 'em
	    // if curbit is not set in keep.
	    if (! (keep & curbit))
		pos.erase (pos.begin () + mark, pos.end ());

	    curbit = 2;
	    lastbit = 4;

	    mark += 2;
	}

	pos.push_back (begin);
	pos.push_back (match);
	
	// Move to the end of matched separator and start there again.
	// To guarantee progress, if this match was null, move on by
	// at least one character, but leave begin to point to the
	// real field start position.
	begin = index = match + 1;

	// If we have reached the maximum number of fields we want to
	// consider and can safely stop searching, do so.  The logic
	// here is that if we have enough actual fields, we definitely
	// can stop.  Otherwise if we have enough normal and empty
	// fields and we know we are going to keep those empty fields,
	// we stop.  Otherwise we might discard some of those empty
	// fields so we need to keep on plodding.  That is, we can't
	// tell what we are going to do with the empty fields until we
	// see the end of the string or another non-empty field.  If
	// they are discardable middle empty fields, we are going to
	// need at least one more non-empty field, and if they are
	// trailing fields, we'll ignore them anyway.  In the former
	// case we won't do extra work, in the latter we'll have to
	// process the trailing junk but can't help it.
	//
	// The termination here is ok.  Either we've just bumped up
	// the "mark" and there is nothing after it in "pos", or there
	// is empty stuff behind it in "pos" which we are not going to
	// keep, and the code below rightfully removes it.
	if (nmax
	    && (mark >= nmax * 2
		|| (pos.size () >= nmax * 2
		    && ! (keep & (lastbit|curbit)))))
	    break;
    }

    // Now treat possible trailing empty fields.  Zap 'em if lastbit
    // is not set in keep.
    if (! (keep & lastbit))
	pos.erase (pos.begin () + mark, pos.end ());

    return pos;
}

StringOps::FieldList
StringOps::splitpos (const char		*s,
		     size_t		slen,
		     const char		*sep,
		     size_t		seplen,
		     int		flags,
		     size_t		nmax)
{
    // This is our return value.  It will have an even number of
    // indices, with pairs of [BEGIN, END), where BEGIN is the
    // beginning of a field and END is one past its end.
    FieldList	pos;
    
    // This marks the last non-empty field "pos".  We always push
    // everything into "pos", and then later decide whether to keep
    // them or not.  "mark" tells us how far to discard at the back of
    // the vector.
    size_t	mark = 0;

    // These two mark the current separator search location (index)
    // and where it was found (match), i.e. the begin and end of the
    // current field (but see below for explanation of begin).  We
    // need these as we step forward looking for non-empty fields.
    int		index = 0;
    int		match;

    // This marks the real beginning of the field being considered.
    // "index" is different from this only when we have matched a
    // zero-length separator; in that case we force the search to move
    // forwards by one character to make progress.
    int		begin = 0;

    // This tracks which empty fields we are interested in keeping.
    // Bit 0 (value 1) is for leading empty fields, bit 1 (value 2)
    // for leading empty and bit 2 (value 4) for trailing empty.
    // "curbit" and "lastbit" establish a state machine that tells
    // us how far we got in scanning when we bail out.
    unsigned	keep =
	((! (flags & TrimEmpty) || (flags & KeepLeadingEmpty) ? 1 : 0)
	 | (! (flags & TrimEmpty) || (flags & KeepTrailingEmpty) ? 4 : 0)
	 | (! (flags & TrimEmpty) ? 2 : 0));

    // This tracks "keep" bits for the empty fields we are currently
    // considering.  It points to leading (bit 0) as long as we are
    // seeing leading empty fields, and then moves to middle (bit 1).
    // Thus if exit having seen only empty fields, they are all
    // treated as leading empty fields.  We use this to select from
    // "keep" how we should treat the empty fields we find.
    unsigned	curbit = 1;

    // This tracks "keep" bits for the empty fields on when we exit.
    // It points initially to leading (bit 0): if all we ever see is
    // empty fields, then they are all leading empty fields.  When we
    // find the first non-empty field, we point this to trailing empty
    // (bit 2): anything following empty fields are then treated as
    // trailing empty on exit.  We use this to select from "keep" how
    // we should treat the empty fields we find.
    unsigned	lastbit = 1;

    // Keep looking as long as we can find fields.
    while ((size_t) index <= slen)
    {
	ASSERT (begin >= 0);
	ASSERT (index >= begin);

	// Find next separator match: either a legitimate separator
	// (match >= 0) or the end of the string (match == -1).  Fix
	// up the latter to avoid special handling: pretend there is a
	// separator at the end of s.  This may match a null-length
	// string at index, that's ok since it's handled in loop code
	// below (begin will be one less than index).
	if ((match = rawfind (s, slen, sep, seplen, index,
			      flags & CaseInsensitiveSep)) == -1)
	    match = slen;

	// We have a field, determine what to do with it.  If it is
	// null, push it to pos but don't move the mark.
	if (match > begin)
	{
	    // Do we keep the empty fields we've seen so far?  Zap 'em
	    // if curbit is not set in keep.
	    if (! (keep & curbit))
		pos.erase (pos.begin () + mark, pos.end ());

	    curbit = 2;
	    lastbit = 4;

	    mark += 2;
	}

	pos.push_back (begin);
	pos.push_back (match);
	
	// Move to the end of matched separator and start there again.
	// To guarantee progress, if this match was null, move on by
	// at least one character, but leave begin to point to the
	// real field start position.
	begin = match + seplen;
	index = begin + (seplen == 0);

	// If we have reached the maximum number of fields we want to
	// consider and can safely stop searching, do so.  The logic
	// here is that if we have enough actual fields, we definitely
	// can stop.  Otherwise if we have enough normal and empty
	// fields and we know we are going to keep those empty fields,
	// we stop.  Otherwise we might discard some of those empty
	// fields so we need to keep on plodding.  That is, we can't
	// tell what we are going to do with the empty fields until we
	// see the end of the string or another non-empty field.  If
	// they are discardable middle empty fields, we are going to
	// need at least one more non-empty field, and if they are
	// trailing fields, we'll ignore them anyway.  In the former
	// case we won't do extra work, in the latter we'll have to
	// process the trailing junk but can't help it.
	//
	// The termination here is ok.  Either we've just bumped up
	// the "mark" and there is nothing after it in "pos", or there
	// is empty stuff behind it in "pos" which we are not going to
	// keep, and the code below rightfully removes it.
	if (nmax
	    && (mark >= nmax * 2
		|| (pos.size () >= nmax * 2
		    && ! (keep & (lastbit|curbit)))))
	    break;
    }

    // Now treat possible trailing empty fields.  Zap 'em if lastbit
    // is not set in keep.
    if (! (keep & lastbit))
	pos.erase (pos.begin () + mark, pos.end ());

    return pos;
}

StringOps::FieldList
StringOps::splitpos (const char		*s,
		     size_t		slen,
		     const Regexp	&rx,
		     int		flags,
		     size_t		nmax)
{
    // This is our return value.  It will have an even number of
    // indices, with pairs of [BEGIN, END), where BEGIN is the
    // beginning of a field and END is one past its end.
    FieldList	pos;
    
    // This marks the last non-empty field "pos".  We always push
    // everything into "pos", and then later decide whether to keep
    // them or not.  "mark" tells us how far to discard at the back of
    // the vector.
    size_t	mark = 0;

    // These two mark the current separator search location (index)
    // and where it was found (match), i.e. the begin and end of the
    // current field (but see below for explanation of begin).  We
    // need these as we step forward looking for non-empty fields.
    int		index = 0;
    int		match;

    // This marks the real beginning of the field being considered.
    // "index" is different from this only when we have matched a
    // zero-length separator; in that case we force the search to move
    // forwards by one character to make progress.
    int		begin = 0;

    // This tracks which empty fields we are interested in keeping.
    // Bit 0 (value 1) is for leading empty fields, bit 1 (value 2)
    // for leading empty and bit 2 (value 4) for trailing empty.
    // "curbit" and "lastbit" establish a state machine that tells
    // us how far we got in scanning when we bail out.
    unsigned	keep =
	((! (flags & TrimEmpty) || (flags & KeepLeadingEmpty) ? 1 : 0)
	 | (! (flags & TrimEmpty) || (flags & KeepTrailingEmpty) ? 4 : 0)
	 | (! (flags & TrimEmpty) ? 2 : 0));

    // This tracks "keep" bits for the empty fields we are currently
    // considering.  It points to leading (bit 0) as long as we are
    // seeing leading empty fields, and then moves to middle (bit 1).
    // Thus if exit having seen only empty fields, they are all
    // treated as leading empty fields.  We use this to select from
    // "keep" how we should treat the empty fields we find.
    unsigned	curbit = 1;

    // This tracks "keep" bits for the empty fields on when we exit.
    // It points initially to leading (bit 0): if all we ever see is
    // empty fields, then they are all leading empty fields.  When we
    // find the first non-empty field, we point this to trailing empty
    // (bit 2): anything following empty fields are then treated as
    // trailing empty on exit.  We use this to select from "keep" how
    // we should treat the empty fields we find.
    unsigned	lastbit = 1;

    // Keep looking as long as we can find fields.
    while ((size_t) index <= slen)
    {
	ASSERT (begin >= 0);
	ASSERT (index >= begin);

	// Regular expression match data.
	RegexpMatch	m;
	int		end;
	int		len;

	// Find next separator match: either a legitimate separator
	// (match >= 0) or the end of the string (match == -1).  Fix
	// up the latter to avoid special handling: pretend there is a
	// separator at the end of s.  This may match a null-length
	// string at index, that's ok since it's handled in loop code
	// below (begin will be one less than index).  Note that we
	// can't ask a matchEnd() of a failed match.
	if ((match = rx.search (s, index, 0 /* FIXME opts */, &m)) == -1)
	{
	    match = end = slen;
	    len = 0;
	}
	else
	{
	    end = m.matchEnd ();
	    len = m.matchLength ();
	}

	// We have a field, determine what to do with it.  If it is
	// null, push it to pos but don't move the mark.
	if (match > begin)
	{
	    // Do we keep the empty fields we've seen so far?  Zap 'em
	    // if curbit is not set in keep.
	    if (! (keep & curbit))
		pos.erase (pos.begin () + mark, pos.end ());

	    curbit = 2;
	    lastbit = 4;

	    mark += 2;
	}

	pos.push_back (begin);
	pos.push_back (match);
	
	// Move to the end of matched separator and start there again.
	// To guarantee progress, if this match was null, move on by
	// at least one character, but leave begin to point to the
	// real field start position.
	begin = end;
	index = begin + (len == 0);

	// If we have reached the maximum number of fields we want to
	// consider and can safely stop searching, do so.  The logic
	// here is that if we have enough actual fields, we definitely
	// can stop.  Otherwise if we have enough normal and empty
	// fields and we know we are going to keep those empty fields,
	// we stop.  Otherwise we might discard some of those empty
	// fields so we need to keep on plodding.  That is, we can't
	// tell what we are going to do with the empty fields until we
	// see the end of the string or another non-empty field.  If
	// they are discardable middle empty fields, we are going to
	// need at least one more non-empty field, and if they are
	// trailing fields, we'll ignore them anyway.  In the former
	// case we won't do extra work, in the latter we'll have to
	// process the trailing junk but can't help it.
	//
	// The termination here is ok.  Either we've just bumped up
	// the "mark" and there is nothing after it in "pos", or there
	// is empty stuff behind it in "pos" which we are not going to
	// keep, and the code below rightfully removes it.
	if (nmax
	    && (mark >= nmax * 2
		|| (pos.size () >= nmax * 2
		    && ! (keep & (lastbit|curbit)))))
	    break;
    }

    // Now treat possible trailing empty fields.  Zap 'em if lastbit
    // is not set in keep.
    if (! (keep & lastbit))
	pos.erase (pos.begin () + mark, pos.end ());

    return pos;
}

/** Select from @s the fields marked in @a fields.  */
StringList
StringOps::split (const char *s, size_t slen,
		  const FieldList &fields,
		  int nmax, int first, int last)
{
    ASSERT (fields.size () % 2 == 0);
    ASSERT (nmax >= 0);

    // First normalise the range.  If it's out of bounds, clamp
    // or return immediately which ever is appropriate.
    int		count = fields.size () / 2;
    StringList	result;

    if (first < 0)
	first = count + first;
    if (first < 0)
	first = 0;
    if (first >= count)
	return result;

    if (last < 0)
	last = count + last;
    if (last < 0)
	return result;
    if (last == 0 || last >= count)
	last = count - 1;

    // Now compute selection range into the fields array.
    size_t nlimit = nmax > 0 ? nmax * 2: ~0u;
    size_t nfirst = first * 2;
    size_t nlast = last * 2;
    size_t end = fields [nfirst];

    ASSERT (nfirst < fields.size ());
    ASSERT (nlast+1 < fields.size ());
    ASSERT ((nlast - nfirst) % 2 == 0);

    // Pick fields.  If we exceed the field limit, append everything
    // between the previous field and current end, otherwise just add
    // a new field.
    for (size_t i = nfirst; i <= nlast; i += 2)
    {
	ASSERT ((size_t) fields [i] <= slen);    // FIXME: < ?
	ASSERT ((size_t) fields [i+1] <= slen);  // FIXME: < ?
	ASSERT (fields [i] <= fields [i+1]);
	ASSERT ((size_t) fields [i] >= end);

	if (i >= nlimit && ! result.empty ())
	    result.back ().append (s, end, fields[i+1]-end);
	else
	    result.push_back (std::string (s,fields[i],fields[i+1]-fields[i]));

	end = fields [i+1];
	
    }

    return result;
}

/** Select from @s the section bounded by @a fields.  */
std::string
StringOps::section (const char *s, size_t slen,
		    const FieldList &fields,
		    int first, int last)
{
    if (fields.empty ())
	return "";

    ASSERT (fields.size () % 2 == 0);

    // First normalise the range.  If it's out of bounds, clamp
    // or return immediately which ever is appropriate.
    int count = fields.size () / 2;

    if (first < 0)
	first = count + first;
    if (first < 0)
	first = 0;
    if (first >= count)
	return "";

    if (last < 0)
	last = count + last;
    if (last < 0)
	return "";
    if (last == 0 || last >= count)
	last = count - 1;

    // Now compute selection range into the fields array.
    size_t nfirst = first * 2;
    size_t nlast = last * 2;

    ASSERT (nfirst < fields.size ());
    ASSERT (nlast+1 < fields.size ());
    ASSERT ((nlast - nfirst) % 2 == 0);

    ASSERT ((size_t) fields [nfirst] <= slen);  // FIXME: < ?
    ASSERT ((size_t) fields [nlast+1] <= slen); // FIXME: < ?
    ASSERT (fields [nfirst] >= fields [nlast+1]);

    // Pick fields.
    return std::string (s, fields [nfirst], fields[nlast+1] - fields[nfirst]);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Join the list of strings in @a items with @a sep separating
    individual items.  Returns a single string.  */
std::string
StringOps::join (const StringList &items, const std::string &sep)
{
    // If items is empty, avoid checking below.
    if (items.empty ())
	return "";

    // First allocate enough space for the result string.
    size_t n = items.size ();
    size_t len = (n - 1) * sep.size ();
    for (size_t i = 0; i < n; ++i)
	len += items [i].size ();

    std::string result;
    result.reserve (len);

    // Now append all items.  We know "items" is not empty so we can
    // safely stuff the first element and then a separator before
    // each of the subsequent elements.
    result += items [0];
    for (size_t i = 1; i < n; ++i)
    {
	result += sep;
	result += items [i];
    }

    return result;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Return those strings from @a items that contain @a what; the
    comparison ignores case if @a caseless is @c true.  */
StringList
StringOps::grep (const StringList	&items,
		 const std::string	&what,
		 bool			caseless /* = false */)
{
    StringList result;
    for (size_t i = 0; i < items.size (); ++i)
	if (rawfind (items [i].c_str (), items [i].size(), what.c_str (), what.size (), 0, caseless) >= 0)
	    result.push_back (items [i]);

    return result;
}

/** Return a list of those strings from @a items that match the
    regular expression @a rx.  */
StringList
StringOps::grep (const StringList &items, const Regexp &rx)
{
    StringList result;
    for (size_t i = 0; i < items.size (); ++i)
	if (rx.search (items [i]) >= 0)
	    result.push_back (items [i]);

    return result;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Count the number of matches of @a what in @a s.  The search is
    case-insesitive if @a caseless is @c true; the default is an exact
    match.  Every character position in @a s is tested for a match of
    @a what.  Returns the number of matches.  */
int
StringOps::contains (const char		*s,
		     char		what,
		     bool		caseless /* = false */)
{
    int n = 0;
    int index = 0;
    size_t slen = strlen(s);
    while ((index = rawfind (s, slen, what, index, caseless)) >= 0)
	++n, ++index;

    return n;
}

/** Count the number of matches of @a what in @a s.  The search is
    case-insesitive if @a caseless is @c true; the default is an exact
    match.  Every character position in @a s is tested for a match of
    @a what.  Returns the number of matches.  */
int
StringOps::contains (const std::string	&s,
		     char		what,
		     bool		caseless /* = false */)
{
    int n = 0;
    int index = 0;
    while ((index = rawfind (s.c_str(), s.size(), what, index, caseless)) >= 0)
	++n, ++index;

    return n;
}

/** Count the number of matches of @a what in @a s.  The search is
    case-insesitive if @a caseless is @c true; the default is an exact
    match.  Every character position in @a s is tested for a match of
    @a what; the function may count overlapping matches if @a what is
    longer than one character (e.g. "aa" will match three, not two,
    times in "aaaa").  If @a what is a null pointer or a zero-length
    string, it will match at every character position.  Returns the
    number of matches.  */
int
StringOps::contains (const char		*s,
		     const char		*what,
		     bool		caseless /* = false */)
{
    int		n = 0;
    int		index = 0;
    size_t	slen = strlen(s);
    size_t	len = what ? strlen (what) : 0;

    while ((index = rawfind (s, slen, what, len, index, caseless)) >= 0)
	++n, ++index;

    return n;
}

/** Count the number of matches of @a what in @a s.  The search is
    case-insesitive if @a caseless is @c true; the default is an exact
    match.  Every character position in @a s is tested for a match of
    @a what; the function may count overlapping matches if @a what is
    longer than one character (e.g. "aa" will match three, not two,
    times in "aaaa").  If @a what is a zero-length string, it will
    match at every character position.  Returns the number of
    matches.  */
int
StringOps::contains (const std::string	&s,
		     const std::string	&what,
		     bool		caseless /* = false */)
{
    int		n = 0;
    int		index = 0;
    const char	*str = s.c_str();
    size_t	slen = s.size();
    const char	*whatstr = what.c_str ();
    size_t	len = what.size ();

    while ((index = rawfind (str, slen, whatstr, len, index, caseless)) >= 0)
	++n, ++index;

    return n;
}

/** Count the number of matches of @a what in @a s.  The search
    case-sensitivity is determined by the properties fo the regular
    expression.  Every character position in @a s is tested for a
    match of @a what; the function may count overlapping matches if @a
    what is longer than one character (e.g. "aa" will match three, not
    two, times in "aaaa").  @a what may match a zero-length string.
    Returns the number of matches.  */
int
StringOps::contains (const char		*s,
		     const Regexp	&rx)
{
    int n = 0;
    int index = 0;
    while ((index = rx.search (s, index)) >= 0)
	++n, ++index;

    return n;
}

/** Count the number of matches of @a what in @a s.  The search
    case-sensitivity is determined by the properties fo the regular
    expression.  Every character position in @a s is tested for a
    match of @a what; the function may count overlapping matches if @a
    what is longer than one character (e.g. "aa" will match three, not
    two, times in "aaaa").  @a what may match a zero-length string.
    Returns the number of matches.  */
int
StringOps::contains (const std::string	&s,
		     const Regexp	&rx)
{
    int n = 0;
    int index = 0;
    while ((index = rx.search (s, index)) >= 0)
	++n, ++index;

    return n;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Find @a what in @a s.  The search starts at index @a offset of @a
    s and proceeds forwards until a match is found.  The search is
    case-insesitive if @a caseless is @c true; the default is an exact
    match.

    A negative @a offset counts characters from the end of the string:
    -1 is the last character of @a s, -2 next to last and so on.  It
    is legitimate for @a offset to specify an out-of-bounds position.
    An out-of-bounds @a offset at the end opposite to the search
    direction (i.e. the beginning of @a s) is clamped to that end
    (i.e. zero).  An out-of-bounds @a offset in the search direction
    (i.e. at the end) simply causes the search to fail.

    Returns the index at which the first match of @a what is found, or
    -1 if no match exists.  */
int
StringOps::find (const char		*s,
		 char			what,
		 int			offset /* = 0 */,
		 bool			caseless /* = false */)
{
    size_t slen = strlen(s);

    if (offset < 0)
	offset += slen;

    if (offset < 0)
	offset = 0;

    if ((size_t) offset >= slen)
	return -1;

    return rawfind (s, slen, what, offset, caseless);
}

/** Find @a what in @a s.  The search starts at index @a offset of @a
    s and proceeds forwards until a match is found.  The search is
    case-insesitive if @a caseless is @c true; the default is an exact
    match.

    A negative @a offset counts characters from the end of the string:
    -1 is the last character of @a s, -2 next to last and so on.  It
    is legitimate for @a offset to specify an out-of-bounds position.
    An out-of-bounds @a offset at the end opposite to the search
    direction (i.e. the beginning of @a s) is clamped to that end
    (i.e. zero).  An out-of-bounds @a offset in the search direction
    (i.e. at the end) simply causes the search to fail.

    Returns the index at which the first match of @a what is found, or
    -1 if no match exists.  */
int
StringOps::find (const std::string	&s,
		 char			what,
		 int			offset /* = 0 */,
		 bool			caseless /* = false */)
{
    if (offset < 0)
	offset += s.size ();

    if (offset < 0)
	offset = 0;

    if ((size_t) offset >= s.size ())
	return -1;

    return rawfind (s.c_str(), s.size(), what, offset, caseless);
}

/** Find @a what in @a s.  The search starts at index @a offset of @a
    s and proceeds forwards until a match is found.  The search is
    case-insesitive if @a caseless is @c true; the default is an exact
    match.

    A negative @a offset counts characters from the end of the string:
    -1 is the last character of @a s, -2 next to last and so on.  It
    is legitimate for @a offset to specify an out-of-bounds position.
    An out-of-bounds @a offset at the end opposite to the search
    direction (i.e. the beginning of @a s) is clamped to that end
    (i.e. zero).  An out-of-bounds @a offset in the search direction
    (i.e. at the end) simply causes the search to fail.

    It is legitimate for @a what to be a null pointer or zero-length
    string.  In that case it will match everywhere, so the function
    will report success at @a offset if it is within the bounds of @a
    s, otherwise fail.

    Returns the index at which the first match of @a what is found, or
    -1 if no match exists.  */
int
StringOps::find (const char		*s,
		 const char		*what,
		 int			offset /* = 0 */,
		 bool			caseless /* = false */)
{
    size_t slen = strlen(s);

    if (offset < 0)
	offset += slen;

    if (offset < 0)
	offset = 0;

    if ((size_t) offset >= slen)
	return -1;

    return rawfind (s, slen,
		    what, what ? strlen (what) : 0,
		    offset, caseless);
}

/** Find @a what in @a s.  The search starts at index @a offset of @a
    s and proceeds forwards until a match is found.  The search is
    case-insesitive if @a caseless is @c true; the default is an exact
    match.

    A negative @a offset counts characters from the end of the string:
    -1 is the last character of @a s, -2 next to last and so on.  It
    is legitimate for @a offset to specify an out-of-bounds position.
    An out-of-bounds @a offset at the end opposite to the search
    direction (i.e. the beginning of @a s) is clamped to that end
    (i.e. zero).  An out-of-bounds @a offset in the search direction
    (i.e. at the end) simply causes the search to fail.

    It is legitimate for @a what to be a zero-length string.  In that
    case it will match everywhere, so the function will report success
    at @a offset if it is within the bounds of @a s, otherwise fail.

    Returns the index at which the first match of @a what is found, or
    -1 if no match exists.  */
int
StringOps::find (const std::string	&s,
		 const std::string	&what,
		 int			offset /* = 0 */,
		 bool			caseless /* = false */)
{
    if (offset < 0)
	offset += s.size ();

    if (offset < 0)
	offset = 0;

    if ((size_t) offset >= s.size ())
	return -1;

    return rawfind (s.c_str(), s.size(),
		    what.c_str (), what.size (),
		    offset, caseless);
}

/** Find @a what in @a s.  The search starts at index @a offset of @a
    s and proceeds forwards until a match is found.  The search
    case-sensitivity is determined by the regular expression.

    A negative @a offset counts characters from the end of the string:
    -1 is the last character of @a s, -2 next to last and so on.  It
    is legitimate for @a offset to specify an out-of-bounds position.
    An out-of-bounds @a offset at the end opposite to the search
    direction (i.e. the beginning of @a s) is clamped to that end
    (i.e. zero).  An out-of-bounds @a offset in the search direction
    (i.e. at the end) simply causes the search to fail.

    It is legitimate for @a what to match a zero-length string.  In
    that case the function will report success at the position where
    @a what matched.  If the regular expression always matches a null
    string, the result will be @a offset if it is within the bounds of
    @a s, otherwise a failure.  The regular expression may however
    match zero-length string only in restricted places, for example
    due to use of forward- or backward-testing assertions.

    Use #Regexp::search() directly if you need the match details.

    Returns the index at which the first match of @a what is found, or
    -1 if no match exists.  */
int
StringOps::find (const char		*s,
		 const Regexp		&what,
		 int			offset /* = 0 */)
{ return what.search (s, offset); }

/** Find @a what in @a s.  The search starts at index @a offset of @a
    s and proceeds forwards until a match is found.  The search
    case-sensitivity is determined by the regular expression.

    A negative @a offset counts characters from the end of the string:
    -1 is the last character of @a s, -2 next to last and so on.  It
    is legitimate for @a offset to specify an out-of-bounds position.
    An out-of-bounds @a offset at the end opposite to the search
    direction (i.e. the beginning of @a s) is clamped to that end
    (i.e. zero).  An out-of-bounds @a offset in the search direction
    (i.e. at the end) simply causes the search to fail.

    It is legitimate for @a what to match a zero-length string.  In
    that case the function will report success at the position where
    @a what matched.  If the regular expression always matches a null
    string, the result will be @a offset if it is within the bounds of
    @a s, otherwise a failure.  The regular expression may however
    match zero-length string only in restricted places, for example
    due to use of forward- or backward-testing assertions.

    Use #Regexp::search() directly if you need the match details.

    Returns the index at which the first match of @a what is found, or
    -1 if no match exists.  */
int
StringOps::find (const std::string	&s,
		 const Regexp		&what,
		 int			offset /* = 0 */)
{ return what.search (s, offset); }

//////////////////////////////////////////////////////////////////////
/** Find @a what in @a s.  The search starts at index @a offset of @a
    s and proceeds backwards until a match is found.  The search is
    case-insesitive if @a caseless is @c true; the default is an exact
    match.

    A negative @a offset counts characters from the end of the string:
    -1 is the last character of @a s, -2 next to last and so on.  It
    is legitimate for @a offset to specify an out-of-bounds position.
    An out-of-bounds @a offset at the end opposite to the search
    direction (i.e. the end of @a s) is clamped to that end (i.e. @c
    s.size()-1).  An out-of-bounds @a offset in the search direction
    (i.e. at the beginning) simply causes the search to fail.

    Returns the index at which the first match of @a what is found, or
    -1 if no match exists.  */
int
StringOps::rfind (const char		*s,
		  char			what,
		  int			offset /* = -1 */,
		  bool			caseless /* = false */)
{
    size_t slen = strlen(s);

    if (offset < 0)
	offset += slen;

    if ((size_t) offset >= slen)
	offset = slen - 1;

    if (offset < 0)
	return -1;
    
    return rawrfind (s, slen, what, offset, caseless);
}

/** Find @a what in @a s.  The search starts at index @a offset of @a
    s and proceeds backwards until a match is found.  The search is
    case-insesitive if @a caseless is @c true; the default is an exact
    match.

    A negative @a offset counts characters from the end of the string:
    -1 is the last character of @a s, -2 next to last and so on.  It
    is legitimate for @a offset to specify an out-of-bounds position.
    An out-of-bounds @a offset at the end opposite to the search
    direction (i.e. the end of @a s) is clamped to that end (i.e. @c
    s.size()-1).  An out-of-bounds @a offset in the search direction
    (i.e. at the beginning) simply causes the search to fail.

    Returns the index at which the first match of @a what is found, or
    -1 if no match exists.  */
int
StringOps::rfind (const std::string	&s,
		  char			what,
		  int			offset /* = -1 */,
		  bool			caseless /* = false */)
{
    if (offset < 0)
	offset += s.size ();

    if ((size_t) offset >= s.size ())
	offset = s.size () - 1;

    if (offset < 0)
	return -1;
    
    return rawrfind (s.c_str(), s.size(), what, offset, caseless);
}

/** Find @a what in @a s.  The search starts at index @a offset of @a
    s and proceeds backwards until a match is found.  The search is
    case-insesitive if @a caseless is @c true; the default is an exact
    match.

    A negative @a offset counts characters from the end of the string:
    -1 is the last character of @a s, -2 next to last and so on.  It
    is legitimate for @a offset to specify an out-of-bounds position.
    An out-of-bounds @a offset at the end opposite to the search
    direction (i.e. the end of @a s) is clamped to that end (i.e. @c
    s.size()-1).  An out-of-bounds @a offset in the search direction
    (i.e. at the beginning) simply causes the search to fail.

    It is legitimate for @a what to be a null pointer or zero-length
    string.  In that case it will match everywhere, so the function
    will report success at @a offset if it is within the bounds of @a
    s, otherwise fail.

    Returns the index at which the first match of @a what is found, or
    -1 if no match exists.  */
int
StringOps::rfind (const char		*s,
		  const char		*what,
		  int			offset /* = -1 */,
		  bool			caseless /* = false */)
{
    size_t slen = strlen(s);

    if (offset < 0)
	offset += slen;

    if ((size_t) offset >= slen)
	offset = slen - 1;

    if (offset < 0)
	return -1;

    return rawrfind (s, slen,
		     what, what ? strlen (what) : 0,
		     offset, caseless);
}

/** Find @a what in @a s.  The search starts at index @a offset of @a
    s and proceeds backwards until a match is found.  The search is
    case-insesitive if @a caseless is @c true; the default is an exact
    match.

    A negative @a offset counts characters from the end of the string:
    -1 is the last character of @a s, -2 next to last and so on.  It
    is legitimate for @a offset to specify an out-of-bounds position.
    An out-of-bounds @a offset at the end opposite to the search
    direction (i.e. the end of @a s) is clamped to that end (i.e. @c
    s.size()-1).  An out-of-bounds @a offset in the search direction
    (i.e. at the beginning) simply causes the search to fail.

    It is legitimate for @a what to be a zero-length string.  In that
    case it will match everywhere, so the function will report success
    at @a offset if it is within the bounds of @a s, otherwise fail.

    Returns the index at which the first match of @a what is found, or
    -1 if no match exists.  */
int
StringOps::rfind (const std::string	&s,
		  const std::string	&what,
		  int			offset /* = -1 */,
		  bool			caseless /* = false */)
{
    if (offset < 0)
	offset += s.size ();

    if ((size_t) offset >= s.size ())
	offset = s.size () - 1;

    if (offset < 0)
	return -1;

    return rawrfind (s.c_str(), s.size(),
		     what.c_str (), what.size (),
		     offset, caseless);
}

/** Find @a what in @a s.  The search starts at index @a offset of @a
    s and proceeds backwards until a match is found.  The search
    case-sensitivity is determined by the regular expression.

    A negative @a offset counts characters from the end of the string:
    -1 is the last character of @a s, -2 next to last and so on.  It
    is legitimate for @a offset to specify an out-of-bounds position.
    An out-of-bounds @a offset at the end opposite to the search
    direction (i.e. the end of @a s) is clamped to that end (i.e. @c
    s.size()-1).  An out-of-bounds @a offset in the search direction
    (i.e. at the beginning) simply causes the search to fail.

    It is legitimate for @a what to match a zero-length string.  In
    that case the function will report success at the position where
    @a what matched.  If the regular expression always matches a null
    string, the result will be @a offset if it is within the bounds of
    @a s, otherwise a failure.  The regular expression may however
    match zero-length string only in restricted places, for example
    due to use of forward- or backward-testing assertions.

    Use #Regexp::rsearch() directly if you need the match details.

    Note that this function is very expensive, much more so than the
    forward search version or the reverse find functions for a fixed
    string or a character.

    Returns the index at which the first match of @a what is found, or
    -1 if no match exists.  */
int
StringOps::rfind (const char		*s,
		  const Regexp		&what,
		  int			offset /* = -1 */)
{ return what.rsearch (s, offset); }

/** Find @a what in @a s.  The search starts at index @a offset of @a
    s and proceeds backwards until a match is found.  The search
    case-sensitivity is determined by the regular expression.

    A negative @a offset counts characters from the end of the string:
    -1 is the last character of @a s, -2 next to last and so on.  It
    is legitimate for @a offset to specify an out-of-bounds position.
    An out-of-bounds @a offset at the end opposite to the search
    direction (i.e. the end of @a s) is clamped to that end (i.e. @c
    s.size()-1).  An out-of-bounds @a offset in the search direction
    (i.e. at the beginning) simply causes the search to fail.

    It is legitimate for @a what to match a zero-length string.  In
    that case the function will report success at the position where
    @a what matched.  If the regular expression always matches a null
    string, the result will be @a offset if it is within the bounds of
    @a s, otherwise a failure.  The regular expression may however
    match zero-length string only in restricted places, for example
    due to use of forward- or backward-testing assertions.

    Use #Regexp::rsearch() directly if you need the match details.

    Note that this function is very expensive, much more so than the
    forward search version or the reverse find functions for a fixed
    string or a character.

    Returns the index at which the first match of @a what is found, or
    -1 if no match exists.  */
int
StringOps::rfind (const std::string	&s,
		  const Regexp		&what,
		  int			offset /* = -1 */)
{ return what.rsearch (s, offset); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Split the string @a s into fields separated by matches of @a sep.

    If @a flags is the default zero, all fields matter.  If @a flags
    has #TrimEmpty set, empty fields, that is leading, trailing or two
    adjacent separators, are ignored.  Leading empty fields are kept
    with #KeepLeadingEmpty set; #KeepTrailingEmpty similarly retains
    trailing empty fields.  If all three are set, leading and trailing
    empty fields are kept, but empty fields between non-empty ones are
    ignored.  Fields are always counted and trimmed as specified by @a
    flags.

    If @a nmax is the default zero, all fields are returned.  If it is
    greater than zero, @a s is considered to have at most that many
    fields, counting left to right up to @a nmax.  If the original
    string contains more than @a nmax fields (let's say N), the last
    reported field runs from the beginning of field[@a nmax-1] to the
    end of field[N-1] including intervening separators; extra fields
    and intervening separators as well as separators at the end are
    concatenated to the last reported field.

    @a first and @a last can be used to further restrict the fields
    returned.  With the default zero values all fields are accepted.
    Non-zero @a first indicates that fields from that count onwards
    are to be included in the result.  Non-zero @a last indicates that
    only fields upto and including @a last should be included in the
    result.  Both @a first and @a last count fields from one.  Values
    greater than (non-zero) @a nmax are legitimate and are interpreted
    to mean that the last field, handled as described above, contains
    subfields separated by the separator and the selected range is to
    be returned as a single string (fields up to but not including @a
    nmax are still returned as separate strings).

    (FIXME: Negative nmax, first, last.)

    The argument combination (@a nmax == 3, @a first == 1, @a last ==
    2) means that at most three fields will be split: two normal ones
    and all the rest.  Only the first two are returned.  The function
    returns immediately after finding them, it will not consider the
    rest of the string.

    The argument combination (@a nmax == 1, @a first == 2, @a last ==
    3, @a flags == #TrimEmpty) means that leading separators are
    trimmed.  If a non-empty field follows, it is discarded, as well
    as any empty fields following it.  If two more non-empty fields
    are found, they are returned as one string, including any
    intervening separators, even if there are empty fields.

    The argument combination (@a nmax == 1, @a flags == #TrimEmpty)
    means that the leading and trailing separators are trimmed and the
    rest is returned as a single field.  Intermediate separators, even
    if they separate empty fields, are returned untouched.

    The argument combination (@a nmax == 1, @a flags == #TrimEmpty |
    #KeepTrailingEmpty) means that only the leading separators are
    trimmed and the rest is returned as a single field.  Once the
    non-empty field is found, the rest of the string is passed
    untouched, without searching it at all.

    Returns the vector containing the requested field strings.  */
StringList
StringOps::split (const char		*s,
		  char			sep,
		  int			flags /* = 0 */,
		  int			nmax /* = 0 */,
		  int			first /* = 0 */,
		  int			last /* = 0 */)
{
    size_t slen = strlen(s);
    return split (s, slen,
		  splitpos (s, slen, sep, flags,
			    splitmax (nmax, first, last)),
		  nmax, first, last);
}

/** Split the string @a s into fields separated by matches of @a sep.

    If @a flags is the default zero, all fields matter.  If @a flags
    has #TrimEmpty set, empty fields, that is leading, trailing or two
    adjacent separators, are ignored.  Leading empty fields are kept
    with #KeepLeadingEmpty set; #KeepTrailingEmpty similarly retains
    trailing empty fields.  If all three are set, leading and trailing
    empty fields are kept, but empty fields between non-empty ones are
    ignored.  Fields are always counted and trimmed as specified by @a
    flags.

    If @a nmax is the default zero, all fields are returned.  If it is
    greater than zero, @a s is considered to have at most that many
    fields, counting left to right up to @a nmax.  If the original
    string contains more than @a nmax fields (let's say N), the last
    reported field runs from the beginning of field[@a nmax-1] to the
    end of field[N-1] including intervening separators; extra fields
    and intervening separators as well as separators at the end are
    concatenated to the last reported field.

    @a first and @a last can be used to further restrict the fields
    returned.  With the default zero values all fields are accepted.
    Non-zero @a first indicates that fields from that count onwards
    are to be included in the result.  Non-zero @a last indicates that
    only fields upto and including @a last should be included in the
    result.  Both @a first and @a last count fields from one.  Values
    greater than (non-zero) @a nmax are legitimate and are interpreted
    to mean that the last field, handled as described above, contains
    subfields separated by the separator and the selected range is to
    be returned as a single string (fields up to but not including @a
    nmax are still returned as separate strings).

    (FIXME: Negative nmax, first, last.)

    The argument combination (@a nmax == 3, @a first == 1, @a last ==
    2) means that at most three fields will be split: two normal ones
    and all the rest.  Only the first two are returned.  The function
    returns immediately after finding them, it will not consider the
    rest of the string.

    The argument combination (@a nmax == 1, @a first == 2, @a last ==
    3, @a flags == #TrimEmpty) means that leading separators are
    trimmed.  If a non-empty field follows, it is discarded, as well
    as any empty fields following it.  If two more non-empty fields
    are found, they are returned as one string, including any
    intervening separators, even if there are empty fields.

    The argument combination (@a nmax == 1, @a flags == #TrimEmpty)
    means that the leading and trailing separators are trimmed and the
    rest is returned as a single field.  Intermediate separators, even
    if they separate empty fields, are returned untouched.

    The argument combination (@a nmax == 1, @a flags == #TrimEmpty |
    #KeepTrailingEmpty) means that only the leading separators are
    trimmed and the rest is returned as a single field.  Once the
    non-empty field is found, the rest of the string is passed
    untouched, without searching it at all.

    Returns the vector containing the requested field strings.  */
StringList
StringOps::split (const std::string	&s,
		  char			sep,
		  int			flags /* = 0 */,
		  int			nmax /* = 0 */,
		  int			first /* = 0 */,
		  int			last /* = 0 */)
{
    return split (s.c_str(), s.size(),
		  splitpos (s.c_str(), s.size(), sep, flags,
			    splitmax (nmax, first, last)),
		  nmax, first, last);
}

/** Split the string @a s into fields separated by matches of @a sep.

    If @a flags is the default zero, all fields matter.  If @a flags
    has #TrimEmpty set, empty fields, that is leading, trailing or two
    adjacent separators, are ignored.  Leading empty fields are kept
    with #KeepLeadingEmpty set; #KeepTrailingEmpty similarly retains
    trailing empty fields.  If all three are set, leading and trailing
    empty fields are kept, but empty fields between non-empty ones are
    ignored.  Fields are always counted and trimmed as specified by @a
    flags.

    If @a nmax is the default zero, all fields are returned.  If it is
    greater than zero, @a s is considered to have at most that many
    fields, counting left to right up to @a nmax.  If the original
    string contains more than @a nmax fields (let's say N), the last
    reported field runs from the beginning of field[@a nmax-1] to the
    end of field[N-1] including intervening separators; extra fields
    and intervening separators as well as separators at the end are
    concatenated to the last reported field.

    @a first and @a last can be used to further restrict the fields
    returned.  With the default zero values all fields are accepted.
    Non-zero @a first indicates that fields from that count onwards
    are to be included in the result.  Non-zero @a last indicates that
    only fields upto and including @a last should be included in the
    result.  Both @a first and @a last count fields from one.  Values
    greater than (non-zero) @a nmax are legitimate and are interpreted
    to mean that the last field, handled as described above, contains
    subfields separated by the separator and the selected range is to
    be returned as a single string (fields up to but not including @a
    nmax are still returned as separate strings).

    (FIXME: Negative nmax, first, last.)

    The argument combination (@a nmax == 3, @a first == 1, @a last ==
    2) means that at most three fields will be split: two normal ones
    and all the rest.  Only the first two are returned.  The function
    returns immediately after finding them, it will not consider the
    rest of the string.

    The argument combination (@a nmax == 1, @a first == 2, @a last ==
    3, @a flags == #TrimEmpty) means that leading separators are
    trimmed.  If a non-empty field follows, it is discarded, as well
    as any empty fields following it.  If two more non-empty fields
    are found, they are returned as one string, including any
    intervening separators, even if there are empty fields.

    The argument combination (@a nmax == 1, @a flags == #TrimEmpty)
    means that the leading and trailing separators are trimmed and the
    rest is returned as a single field.  Intermediate separators, even
    if they separate empty fields, are returned untouched.

    The argument combination (@a nmax == 1, @a flags == #TrimEmpty |
    #KeepTrailingEmpty) means that only the leading separators are
    trimmed and the rest is returned as a single field.  Once the
    non-empty field is found, the rest of the string is passed
    untouched, without searching it at all.

    Returns the vector containing the requested field strings.  */
StringList
StringOps::split (const char		*s,
		  const char		*sep,
		  int			flags /* = 0 */,
		  int			nmax /* = 0 */,
		  int			first /* = 0 */,
		  int			last /* = 0 */)
{
    size_t slen = strlen(s);
    size_t seplen = sep ? strlen(sep) : 0;
    return split (s, slen,
		  splitpos (s, slen, sep, seplen, flags,
			    splitmax (nmax, first, last)),
		  nmax, first, last);
}

/** Split the string @a s into fields separated by matches of @a sep.

    If @a flags is the default zero, all fields matter.  If @a flags
    has #TrimEmpty set, empty fields, that is leading, trailing or two
    adjacent separators, are ignored.  Leading empty fields are kept
    with #KeepLeadingEmpty set; #KeepTrailingEmpty similarly retains
    trailing empty fields.  If all three are set, leading and trailing
    empty fields are kept, but empty fields between non-empty ones are
    ignored.  Fields are always counted and trimmed as specified by @a
    flags.

    If @a nmax is the default zero, all fields are returned.  If it is
    greater than zero, @a s is considered to have at most that many
    fields, counting left to right up to @a nmax.  If the original
    string contains more than @a nmax fields (let's say N), the last
    reported field runs from the beginning of field[@a nmax-1] to the
    end of field[N-1] including intervening separators; extra fields
    and intervening separators as well as separators at the end are
    concatenated to the last reported field.

    @a first and @a last can be used to further restrict the fields
    returned.  With the default zero values all fields are accepted.
    Non-zero @a first indicates that fields from that count onwards
    are to be included in the result.  Non-zero @a last indicates that
    only fields upto and including @a last should be included in the
    result.  Both @a first and @a last count fields from one.  Values
    greater than (non-zero) @a nmax are legitimate and are interpreted
    to mean that the last field, handled as described above, contains
    subfields separated by the separator and the selected range is to
    be returned as a single string (fields up to but not including @a
    nmax are still returned as separate strings).

    (FIXME: Negative nmax, first, last.)

    The argument combination (@a nmax == 3, @a first == 1, @a last ==
    2) means that at most three fields will be split: two normal ones
    and all the rest.  Only the first two are returned.  The function
    returns immediately after finding them, it will not consider the
    rest of the string.

    The argument combination (@a nmax == 1, @a first == 2, @a last ==
    3, @a flags == #TrimEmpty) means that leading separators are
    trimmed.  If a non-empty field follows, it is discarded, as well
    as any empty fields following it.  If two more non-empty fields
    are found, they are returned as one string, including any
    intervening separators, even if there are empty fields.

    The argument combination (@a nmax == 1, @a flags == #TrimEmpty)
    means that the leading and trailing separators are trimmed and the
    rest is returned as a single field.  Intermediate separators, even
    if they separate empty fields, are returned untouched.

    The argument combination (@a nmax == 1, @a flags == #TrimEmpty |
    #KeepTrailingEmpty) means that only the leading separators are
    trimmed and the rest is returned as a single field.  Once the
    non-empty field is found, the rest of the string is passed
    untouched, without searching it at all.

    Returns the vector containing the requested field strings.  */
StringList
StringOps::split (const std::string	&s,
		  const std::string	&sep,
		  int			flags /* = 0 */,
		  int			nmax /* = 0 */,
		  int			first /* = 0 */,
		  int			last /* = 0 */)
{
    return split (s.c_str(), s.size(),
		  splitpos (s.c_str(), s.size(), sep.c_str(), sep.size(), flags,
			    splitmax (nmax, first, last)),
		  nmax, first, last);
}

/** Split the string @a s into fields separated by matches of @a sep.

    If @a flags is the default zero, all fields matter.  If @a flags
    has #TrimEmpty set, empty fields, that is leading, trailing or two
    adjacent separators, are ignored.  Leading empty fields are kept
    with #KeepLeadingEmpty set; #KeepTrailingEmpty similarly retains
    trailing empty fields.  If all three are set, leading and trailing
    empty fields are kept, but empty fields between non-empty ones are
    ignored.  Fields are always counted and trimmed as specified by @a
    flags.

    If @a nmax is the default zero, all fields are returned.  If it is
    greater than zero, @a s is considered to have at most that many
    fields, counting left to right up to @a nmax.  If the original
    string contains more than @a nmax fields (let's say N), the last
    reported field runs from the beginning of field[@a nmax-1] to the
    end of field[N-1] including intervening separators; extra fields
    and intervening separators as well as separators at the end are
    concatenated to the last reported field.

    @a first and @a last can be used to further restrict the fields
    returned.  With the default zero values all fields are accepted.
    Non-zero @a first indicates that fields from that count onwards
    are to be included in the result.  Non-zero @a last indicates that
    only fields upto and including @a last should be included in the
    result.  Both @a first and @a last count fields from one.  Values
    greater than (non-zero) @a nmax are legitimate and are interpreted
    to mean that the last field, handled as described above, contains
    subfields separated by the separator and the selected range is to
    be returned as a single string (fields up to but not including @a
    nmax are still returned as separate strings).

    (FIXME: Negative nmax, first, last.)

    The argument combination (@a nmax == 3, @a first == 1, @a last ==
    2) means that at most three fields will be split: two normal ones
    and all the rest.  Only the first two are returned.  The function
    returns immediately after finding them, it will not consider the
    rest of the string.

    The argument combination (@a nmax == 1, @a first == 2, @a last ==
    3, @a flags == #TrimEmpty) means that leading separators are
    trimmed.  If a non-empty field follows, it is discarded, as well
    as any empty fields following it.  If two more non-empty fields
    are found, they are returned as one string, including any
    intervening separators, even if there are empty fields.

    The argument combination (@a nmax == 1, @a flags == #TrimEmpty)
    means that the leading and trailing separators are trimmed and the
    rest is returned as a single field.  Intermediate separators, even
    if they separate empty fields, are returned untouched.

    The argument combination (@a nmax == 1, @a flags == #TrimEmpty |
    #KeepTrailingEmpty) means that only the leading separators are
    trimmed and the rest is returned as a single field.  Once the
    non-empty field is found, the rest of the string is passed
    untouched, without searching it at all.

    Returns the vector containing the requested field strings.  */
StringList
StringOps::split (const char		*s,
		  const Regexp		&sep,
		  int			flags /* = 0 */,
		  int			nmax /* = 0 */,
		  int			first /* = 0 */,
		  int			last /* = 0 */)
{
    size_t slen = strlen(s);
    return split (s, slen,
		  splitpos (s, slen, sep, flags,
			    splitmax (nmax, first, last)),
		  nmax, first, last);
}

/** Split the string @a s into fields separated by matches of @a sep.

    If @a flags is the default zero, all fields matter.  If @a flags
    has #TrimEmpty set, empty fields, that is leading, trailing or two
    adjacent separators, are ignored.  Leading empty fields are kept
    with #KeepLeadingEmpty set; #KeepTrailingEmpty similarly retains
    trailing empty fields.  If all three are set, leading and trailing
    empty fields are kept, but empty fields between non-empty ones are
    ignored.  Fields are always counted and trimmed as specified by @a
    flags.

    If @a nmax is the default zero, all fields are returned.  If it is
    greater than zero, @a s is considered to have at most that many
    fields, counting left to right up to @a nmax.  If the original
    string contains more than @a nmax fields (let's say N), the last
    reported field runs from the beginning of field[@a nmax-1] to the
    end of field[N-1] including intervening separators; extra fields
    and intervening separators as well as separators at the end are
    concatenated to the last reported field.

    @a first and @a last can be used to further restrict the fields
    returned.  With the default zero values all fields are accepted.
    Non-zero @a first indicates that fields from that count onwards
    are to be included in the result.  Non-zero @a last indicates that
    only fields upto and including @a last should be included in the
    result.  Both @a first and @a last count fields from one.  Values
    greater than (non-zero) @a nmax are legitimate and are interpreted
    to mean that the last field, handled as described above, contains
    subfields separated by the separator and the selected range is to
    be returned as a single string (fields up to but not including @a
    nmax are still returned as separate strings).

    (FIXME: Negative nmax, first, last.)

    The argument combination (@a nmax == 3, @a first == 1, @a last ==
    2) means that at most three fields will be split: two normal ones
    and all the rest.  Only the first two are returned.  The function
    returns immediately after finding them, it will not consider the
    rest of the string.

    The argument combination (@a nmax == 1, @a first == 2, @a last ==
    3, @a flags == #TrimEmpty) means that leading separators are
    trimmed.  If a non-empty field follows, it is discarded, as well
    as any empty fields following it.  If two more non-empty fields
    are found, they are returned as one string, including any
    intervening separators, even if there are empty fields.

    The argument combination (@a nmax == 1, @a flags == #TrimEmpty)
    means that the leading and trailing separators are trimmed and the
    rest is returned as a single field.  Intermediate separators, even
    if they separate empty fields, are returned untouched.

    The argument combination (@a nmax == 1, @a flags == #TrimEmpty |
    #KeepTrailingEmpty) means that only the leading separators are
    trimmed and the rest is returned as a single field.  Once the
    non-empty field is found, the rest of the string is passed
    untouched, without searching it at all.

    Returns the vector containing the requested field strings.  */
StringList
StringOps::split (const std::string	&s,
		  const Regexp		&sep,
		  int			flags /* = 0 */,
		  int			nmax /* = 0 */,
		  int			first /* = 0 */,
		  int			last /* = 0 */)
{
    return split (s.c_str(), s.size(),
		  splitpos (s.c_str(), s.size(), sep, flags,
			    splitmax (nmax, first, last)),
		  nmax, first, last);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Extract from the string @a s a substring of fields separated by
    matches of @a sep.

    This function behaves like the corresponding #split() function
    except that it returns a single substring section from @a s as
    designated by the field limits, much like if #split() was called
    with @c nmax set to one.  Thus if #split() would return N fields,
    #section() extracts from @a s a substring that begins at the
    beginning of the first field and runs to the end of the Nth field.
    Intervening separators are not trimmed.

    If @a flags is the default zero, all fields matter.  If @a flags
    has #TrimEmpty set, empty fields, that is leading, trailing or two
    adjacent separators, are ignored.  Leading empty fields are kept
    with #KeepLeadingEmpty set; #KeepTrailingEmpty similarly retains
    trailing empty fields.  If all three are set, leading and trailing
    empty fields are kept, but empty fields between non-empty ones are
    ignored.  Fields are always counted and trimmed as specified by @a
    flags.

    @a first and @a last can be used to further restrict the fields
    returned.  With the default zero values all fields are accepted.
    Non-zero @a first indicates that fields from that count onwards
    are to be included in the result.  Non-zero @a last indicates that
    only fields upto and including @a last should be included in the
    result.  Both @a first and @a last count fields from one.

    (FIXME: Negative first, last.)

    The argument combination (@a first == 1, @a last == 2) means that
    at most three fields are split: two normal ones and all the rest.
    The section from the beginning of the first field to the end of
    the second is returned.  The function returns immediately after
    finding the first two fields, it will not consider the rest of the
    string.

    The argument combination (@a first == 2, @a last == 3, @a flags ==
    #TrimEmpty) means that leading separators are trimmed.  If a
    non-empty field follows, it is discarded, as well as any empty
    fields following it.  If two more non-empty fields are found, they
    are returned as one string, including any intervening separators,
    even if there are empty fields.

    The argument combination (@a flags == #TrimEmpty) means that the
    leading and trailing separators are trimmed.  The rest is returned
    untouched, even if there are intermediate empty fields.

    The argument combination (@a flags == #TrimEmpty |
    #KeepTrailingEmpty) means that only the leading separators are
    trimmed and the rest is returned untouched.  Once a non-empty
    field is found, the rest of @a s is not searched at all.

    Returns the section of @a s containing the requested fields.  */
std::string
StringOps::section (const char		*s,
		    char		sep,
		    int			first,
		    int			last /* = -1 */,
		    int			flags /* = 0 */)
{
    size_t slen = strlen(s);
    return section (s, slen,
		    splitpos (s, slen, sep, flags,
			      splitmax (0, first, last)),
		    first, last);
}

/** Extract from the string @a s a substring of fields separated by
    matches of @a sep.

    This function behaves like the corresponding #split() function
    except that it returns a single substring section from @a s as
    designated by the field limits, much like if #split() was called
    with @c nmax set to one.  Thus if #split() would return N fields,
    #section() extracts from @a s a substring that begins at the
    beginning of the first field and runs to the end of the Nth field.
    Intervening separators are not trimmed.

    If @a flags is the default zero, all fields matter.  If @a flags
    has #TrimEmpty set, empty fields, that is leading, trailing or two
    adjacent separators, are ignored.  Leading empty fields are kept
    with #KeepLeadingEmpty set; #KeepTrailingEmpty similarly retains
    trailing empty fields.  If all three are set, leading and trailing
    empty fields are kept, but empty fields between non-empty ones are
    ignored.  Fields are always counted and trimmed as specified by @a
    flags.

    @a first and @a last can be used to further restrict the fields
    returned.  With the default zero values all fields are accepted.
    Non-zero @a first indicates that fields from that count onwards
    are to be included in the result.  Non-zero @a last indicates that
    only fields upto and including @a last should be included in the
    result.  Both @a first and @a last count fields from one.

    (FIXME: Negative first, last.)

    The argument combination (@a first == 1, @a last == 2) means that
    at most three fields are split: two normal ones and all the rest.
    The section from the beginning of the first field to the end of
    the second is returned.  The function returns immediately after
    finding the first two fields, it will not consider the rest of the
    string.

    The argument combination (@a first == 2, @a last == 3, @a flags ==
    #TrimEmpty) means that leading separators are trimmed.  If a
    non-empty field follows, it is discarded, as well as any empty
    fields following it.  If two more non-empty fields are found, they
    are returned as one string, including any intervening separators,
    even if there are empty fields.

    The argument combination (@a flags == #TrimEmpty) means that the
    leading and trailing separators are trimmed.  The rest is returned
    untouched, even if there are intermediate empty fields.

    The argument combination (@a flags == #TrimEmpty |
    #KeepTrailingEmpty) means that only the leading separators are
    trimmed and the rest is returned untouched.  Once a non-empty
    field is found, the rest of @a s is not searched at all.

    Returns the section of @a s containing the requested fields.  */
std::string
StringOps::section (const std::string	&s,
		    char		sep,
		    int			first,
		    int			last /* = -1 */,
		    int			flags /* = 0 */)
{
    return section (s.c_str(), s.size(),
		    splitpos (s.c_str(), s.size(), sep, flags,
			      splitmax (0, first, last)),
		    first, last);
}

/** Extract from the string @a s a substring of fields separated by
    matches of @a sep.

    This function behaves like the corresponding #split() function
    except that it returns a single substring section from @a s as
    designated by the field limits, much like if #split() was called
    with @c nmax set to one.  Thus if #split() would return N fields,
    #section() extracts from @a s a substring that begins at the
    beginning of the first field and runs to the end of the Nth field.
    Intervening separators are not trimmed.

    If @a flags is the default zero, all fields matter.  If @a flags
    has #TrimEmpty set, empty fields, that is leading, trailing or two
    adjacent separators, are ignored.  Leading empty fields are kept
    with #KeepLeadingEmpty set; #KeepTrailingEmpty similarly retains
    trailing empty fields.  If all three are set, leading and trailing
    empty fields are kept, but empty fields between non-empty ones are
    ignored.  Fields are always counted and trimmed as specified by @a
    flags.

    @a first and @a last can be used to further restrict the fields
    returned.  With the default zero values all fields are accepted.
    Non-zero @a first indicates that fields from that count onwards
    are to be included in the result.  Non-zero @a last indicates that
    only fields upto and including @a last should be included in the
    result.  Both @a first and @a last count fields from one.

    (FIXME: Negative first, last.)

    The argument combination (@a first == 1, @a last == 2) means that
    at most three fields are split: two normal ones and all the rest.
    The section from the beginning of the first field to the end of
    the second is returned.  The function returns immediately after
    finding the first two fields, it will not consider the rest of the
    string.

    The argument combination (@a first == 2, @a last == 3, @a flags ==
    #TrimEmpty) means that leading separators are trimmed.  If a
    non-empty field follows, it is discarded, as well as any empty
    fields following it.  If two more non-empty fields are found, they
    are returned as one string, including any intervening separators,
    even if there are empty fields.

    The argument combination (@a flags == #TrimEmpty) means that the
    leading and trailing separators are trimmed.  The rest is returned
    untouched, even if there are intermediate empty fields.

    The argument combination (@a flags == #TrimEmpty |
    #KeepTrailingEmpty) means that only the leading separators are
    trimmed and the rest is returned untouched.  Once a non-empty
    field is found, the rest of @a s is not searched at all.

    Returns the section of @a s containing the requested fields.  */
std::string
StringOps::section (const char		*s,
		    const char		*sep,
		    int			first,
		    int			last /* = -1 */,
		    int			flags /* = 0 */)
{
    size_t slen = strlen(s);
    size_t seplen = sep ? strlen(sep) : 0;
    return section (s, slen,
		    splitpos (s, slen, sep, seplen, flags,
			      splitmax (0, first, last)),
		    first, last);
}

/** Extract from the string @a s a substring of fields separated by
    matches of @a sep.

    This function behaves like the corresponding #split() function
    except that it returns a single substring section from @a s as
    designated by the field limits, much like if #split() was called
    with @c nmax set to one.  Thus if #split() would return N fields,
    #section() extracts from @a s a substring that begins at the
    beginning of the first field and runs to the end of the Nth field.
    Intervening separators are not trimmed.

    If @a flags is the default zero, all fields matter.  If @a flags
    has #TrimEmpty set, empty fields, that is leading, trailing or two
    adjacent separators, are ignored.  Leading empty fields are kept
    with #KeepLeadingEmpty set; #KeepTrailingEmpty similarly retains
    trailing empty fields.  If all three are set, leading and trailing
    empty fields are kept, but empty fields between non-empty ones are
    ignored.  Fields are always counted and trimmed as specified by @a
    flags.

    @a first and @a last can be used to further restrict the fields
    returned.  With the default zero values all fields are accepted.
    Non-zero @a first indicates that fields from that count onwards
    are to be included in the result.  Non-zero @a last indicates that
    only fields upto and including @a last should be included in the
    result.  Both @a first and @a last count fields from one.

    (FIXME: Negative first, last.)

    The argument combination (@a first == 1, @a last == 2) means that
    at most three fields are split: two normal ones and all the rest.
    The section from the beginning of the first field to the end of
    the second is returned.  The function returns immediately after
    finding the first two fields, it will not consider the rest of the
    string.

    The argument combination (@a first == 2, @a last == 3, @a flags ==
    #TrimEmpty) means that leading separators are trimmed.  If a
    non-empty field follows, it is discarded, as well as any empty
    fields following it.  If two more non-empty fields are found, they
    are returned as one string, including any intervening separators,
    even if there are empty fields.

    The argument combination (@a flags == #TrimEmpty) means that the
    leading and trailing separators are trimmed.  The rest is returned
    untouched, even if there are intermediate empty fields.

    The argument combination (@a flags == #TrimEmpty |
    #KeepTrailingEmpty) means that only the leading separators are
    trimmed and the rest is returned untouched.  Once a non-empty
    field is found, the rest of @a s is not searched at all.

    Returns the section of @a s containing the requested fields.  */
std::string
StringOps::section (const std::string	&s,
		    const std::string	&sep,
		    int			first,
		    int			last /* = -1 */,
		    int			flags /* = 0 */)
{
    return section (s.c_str(), s.size(),
		    splitpos (s.c_str(), s.size(), sep.c_str(), sep.size(), flags,
			      splitmax (0, first, last)),
		    first, last);
}

/** Extract from the string @a s a substring of fields separated by
    matches of @a sep.

    This function behaves like the corresponding #split() function
    except that it returns a single substring section from @a s as
    designated by the field limits, much like if #split() was called
    with @c nmax set to one.  Thus if #split() would return N fields,
    #section() extracts from @a s a substring that begins at the
    beginning of the first field and runs to the end of the Nth field.
    Intervening separators are not trimmed.

    If @a flags is the default zero, all fields matter.  If @a flags
    has #TrimEmpty set, empty fields, that is leading, trailing or two
    adjacent separators, are ignored.  Leading empty fields are kept
    with #KeepLeadingEmpty set; #KeepTrailingEmpty similarly retains
    trailing empty fields.  If all three are set, leading and trailing
    empty fields are kept, but empty fields between non-empty ones are
    ignored.  Fields are always counted and trimmed as specified by @a
    flags.

    @a first and @a last can be used to further restrict the fields
    returned.  With the default zero values all fields are accepted.
    Non-zero @a first indicates that fields from that count onwards
    are to be included in the result.  Non-zero @a last indicates that
    only fields upto and including @a last should be included in the
    result.  Both @a first and @a last count fields from one.

    (FIXME: Negative first, last.)

    The argument combination (@a first == 1, @a last == 2) means that
    at most three fields are split: two normal ones and all the rest.
    The section from the beginning of the first field to the end of
    the second is returned.  The function returns immediately after
    finding the first two fields, it will not consider the rest of the
    string.

    The argument combination (@a first == 2, @a last == 3, @a flags ==
    #TrimEmpty) means that leading separators are trimmed.  If a
    non-empty field follows, it is discarded, as well as any empty
    fields following it.  If two more non-empty fields are found, they
    are returned as one string, including any intervening separators,
    even if there are empty fields.

    The argument combination (@a flags == #TrimEmpty) means that the
    leading and trailing separators are trimmed.  The rest is returned
    untouched, even if there are intermediate empty fields.

    The argument combination (@a flags == #TrimEmpty |
    #KeepTrailingEmpty) means that only the leading separators are
    trimmed and the rest is returned untouched.  Once a non-empty
    field is found, the rest of @a s is not searched at all.

    Returns the section of @a s containing the requested fields.  */
std::string
StringOps::section (const char		*s,
		    const Regexp	&sep,
		    int			first,
		    int			last /* = -1 */,
		    int			flags /* = 0 */)
{
    size_t slen = strlen(s);
    return section (s, slen,
		    splitpos (s, slen, sep, flags,
			      splitmax (0, first, last)),
		    first, last);
}

/** Extract from the string @a s a substring of fields separated by
    matches of @a sep.

    This function behaves like the corresponding #split() function
    except that it returns a single substring section from @a s as
    designated by the field limits, much like if #split() was called
    with @c nmax set to one.  Thus if #split() would return N fields,
    #section() extracts from @a s a substring that begins at the
    beginning of the first field and runs to the end of the Nth field.
    Intervening separators are not trimmed.

    If @a flags is the default zero, all fields matter.  If @a flags
    has #TrimEmpty set, empty fields, that is leading, trailing or two
    adjacent separators, are ignored.  Leading empty fields are kept
    with #KeepLeadingEmpty set; #KeepTrailingEmpty similarly retains
    trailing empty fields.  If all three are set, leading and trailing
    empty fields are kept, but empty fields between non-empty ones are
    ignored.  Fields are always counted and trimmed as specified by @a
    flags.

    @a first and @a last can be used to further restrict the fields
    returned.  With the default zero values all fields are accepted.
    Non-zero @a first indicates that fields from that count onwards
    are to be included in the result.  Non-zero @a last indicates that
    only fields upto and including @a last should be included in the
    result.  Both @a first and @a last count fields from one.

    (FIXME: Negative first, last.)

    The argument combination (@a first == 1, @a last == 2) means that
    at most three fields are split: two normal ones and all the rest.
    The section from the beginning of the first field to the end of
    the second is returned.  The function returns immediately after
    finding the first two fields, it will not consider the rest of the
    string.

    The argument combination (@a first == 2, @a last == 3, @a flags ==
    #TrimEmpty) means that leading separators are trimmed.  If a
    non-empty field follows, it is discarded, as well as any empty
    fields following it.  If two more non-empty fields are found, they
    are returned as one string, including any intervening separators,
    even if there are empty fields.

    The argument combination (@a flags == #TrimEmpty) means that the
    leading and trailing separators are trimmed.  The rest is returned
    untouched, even if there are intermediate empty fields.

    The argument combination (@a flags == #TrimEmpty |
    #KeepTrailingEmpty) means that only the leading separators are
    trimmed and the rest is returned untouched.  Once a non-empty
    field is found, the rest of @a s is not searched at all.

    Returns the section of @a s containing the requested fields.  */
std::string
StringOps::section (const std::string	&s,
		    const Regexp	&sep,
		    int			first,
		    int			last /* = -1 */,
		    int			flags /* = 0 */)
{
    return section (s.c_str(), s.size(),
		    splitpos (s.c_str(), s.size(), sep, flags,
			      splitmax (0, first, last)),
		    first, last);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Remove all matches of @a what from @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    Returns the @a s with the changes made.  */
std::string
StringOps::remove (const char		*s,
		   char			what,
		   int			offset /* = 0 */)
{ return replace (s, offset, what, ""); }

/** Remove all matches of @a what from @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    Returns the @a s with the changes made.  */
std::string
StringOps::remove (const std::string	&s,
		   char			what,
		   int			offset /* = 0 */)
{ return replace (s.c_str(), offset, what, ""); }

/** Remove all matches of @a what from @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    Once an instance of @a what has been removed, the search for the
    next match begins after the end of the previous one.  If @a what
    is an empty string, @a s is left intact (i.e. the empty string is
    removed at every position it matches in, which is at every
    character).

    Returns the @a s with the changes made.  */
std::string
StringOps::remove (const char		*s,
		   const char		*what,
		   int			offset /* = 0 */)
{ return replace (s, offset, what, ""); }

/** Remove all matches of @a what from @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    Once an instance of @a what has been removed, the search for the
    next match begins after the end of the previous one.  If @a what
    is an empty string, @a s is left intact (i.e. the empty string is
    removed at every position it matches in, which is at every
    character).

    Returns the @a s with the changes made.  */
std::string
StringOps::remove (const std::string	&s,
		   const std::string	&what,
		   int			offset /* = 0 */)
{ return replace (s.c_str(), offset, what.c_str(), ""); }

/** Remove all matches of @a what from @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    Once an instance of @a what has been removed, the search for the
    next match begins after the end of the previous one.  If @a what
    matches an empty string, @a s is left intact at that position
    (i.e. the empty string is removed at every match position).

    Returns the @a s with the changes made.  */
std::string
StringOps::remove (const char		*s,
		   const Regexp		&what,
		   int			offset /* = 0 */)
{ return replace (s, offset, what, ""); }

/** Remove all matches of @a what from @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    Once an instance of @a what has been removed, the search for the
    next match begins after the end of the previous one.  If @a what
    matches an empty string, @a s is left intact at that position
    (i.e. the empty string is removed at every match position).

    Returns the @a s with the changes made.  */
std::string
StringOps::remove (const std::string	&s,
		   const Regexp		&what,
		   int			offset /* = 0 */)
{ return replace (s.c_str(), offset, what, ""); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Replace all matches of @a what with @a with in @a s.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  An empty @a with string
    causes all matches of @a what to be removed.

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const char		*s,
		    char		what,
		    const char		*with)
{ return replace (s, 0, what, with); }

/** Replace all matches of @a what with @a with in @a s.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  An empty @a with string
    causes all matches of @a what to be removed.

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const std::string	&s,
		    char		what,
		    const std::string	&with)
{ return replace (s.c_str(), 0, what, with.c_str()); }

/** Replace all matches of @a what with @a with in @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  An empty @a with string
    causes all matches of @a what to be removed.

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const char		*s,
		    int			offset,
		    char		what,
		    const char		*with)
{
    std::string result (s);
    size_t	withlen = strlen(with);
    int		index;

    while ((index = find (result, what, offset)) >= 0)
    {
	ASSERT ((size_t) index < result.size ());
	ASSERT (index >= offset);

	result.replace (index, 1, with);
	offset = index + withlen + 1;
    }

    return result;
}

/** Replace all matches of @a what with @a with in @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  An empty @a with string
    causes all matches of @a what to be removed.

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const std::string	&s,
		    int			offset,
		    char		what,
		    const std::string	&with)
{
    return replace(s.c_str(), offset, what, with.c_str());
}

/** Replace all matches of @a what with @a with in @a s.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  Thus overlapping matches
    are never considered: for example, in @a s "aaaa", a @a what "aa"
    would be replaced twice, not three times, at positions 0 and 2.
    An empty @a with string causes all matches of @a what to be
    removed.  An empty @a what string causes @a with to be inserted
    before every character in @a s.

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const char		*s,
		    const char		*what,
		    const char		*with)
{ return replace (s, 0, what, with); }

/** Replace all matches of @a what with @a with in @a s.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  Thus overlapping matches
    are never considered: for example, in @a s "aaaa", a @a what "aa"
    would be replaced twice, not three times, at positions 0 and 2.
    An empty @a with string causes all matches of @a what to be
    removed.  An empty @a what string causes @a with to be inserted
    before every character in @a s.

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const std::string	&s,
		    const std::string	&what,
		    const std::string	&with)
{ return replace (s.c_str(), 0, what.c_str(), with.c_str()); }

/** Replace all matches of @a what with @a with in @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  Thus overlapping matches
    are never considered: for example, in @a s "aaaa", a @a what "aa"
    would be replaced twice, not three times, at positions 0 and 2.
    An empty @a with string causes all matches of @a what to be
    removed.  An empty @a what string causes @a with to be inserted
    before every character in @a s.

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const char		*s,
		    int			offset,
		    const char		*what,
		    const char		*with)
{
    std::string result (s);
    size_t	withlen = strlen(with);
    size_t	whatlen = strlen(what);
    int		index;

    while ((index = find (result, what, offset)) >= 0)
    {
	ASSERT ((size_t) index < result.size ());
	ASSERT (index >= offset);

	result.replace (index, whatlen, with);
	offset = index + withlen + 1;
    }

    return result;
}

/** Replace all matches of @a what with @a with in @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  Thus overlapping matches
    are never considered: for example, in @a s "aaaa", a @a what "aa"
    would be replaced twice, not three times, at positions 0 and 2.
    An empty @a with string causes all matches of @a what to be
    removed.  An empty @a what string causes @a with to be inserted
    before every character in @a s.

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const std::string	&s,
		    int			offset,
		    const std::string	&what,
		    const std::string	&with)
{
    return replace (s.c_str (), offset, what.c_str (), with.c_str ());
}

/** Replace all matches of @a what with @a with in @a s.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  Thus overlapping matches
    are never considered: for example, in @a s "aaaa", a @a what "aa"
    would be replaced twice, not three times, at positions 0 and 2.
    An empty @a with string causes all matches of @a what to be
    removed.  An empty @a what string causes @a with to be inserted
    before every character in @a s.

    The replacement text may contain back-references to the capturing
    subpatterns in @a what.  The back-references will be replaced with
    the corresponding matched regions from @a s.  For details on the
    syntax of suitable @a what patterns please see #replace(const
    std::string &, const RegexMatch &, const std::string &).

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const char		*s,
		    const Regexp	&what,
		    const char		*with)
{ return replace (s, 0, what, 0, with); }

/** Replace all matches of @a what with @a with in @a s.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  Thus overlapping matches
    are never considered: for example, in @a s "aaaa", a @a what "aa"
    would be replaced twice, not three times, at positions 0 and 2.
    An empty @a with string causes all matches of @a what to be
    removed.  An empty @a what string causes @a with to be inserted
    before every character in @a s.

    The replacement text may contain back-references to the capturing
    subpatterns in @a what.  The back-references will be replaced with
    the corresponding matched regions from @a s.  For details on the
    syntax of suitable @a what patterns please see #replace(const
    std::string &, const RegexMatch &, const std::string &).

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const std::string	&s,
		    const Regexp	&what,
		    const std::string	&with)
{ return replace (s.c_str(), 0, what, 0, with.c_str()); }

/** Replace all matches of @a what with @a with in @a s.

    The regular expression search is executed with match @a options;
    see #Regexp for further details.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  Thus overlapping matches
    are never considered: for example, in @a s "aaaa", a @a what "aa"
    would be replaced twice, not three times, at positions 0 and 2.
    An empty @a with string causes all matches of @a what to be
    removed.  An empty @a what string causes @a with to be inserted
    before every character in @a s.

    The replacement text may contain back-references to the capturing
    subpatterns in @a what.  The back-references will be replaced with
    the corresponding matched regions from @a s.  For details on the
    syntax of suitable @a what patterns please see #replace(const
    std::string &, const RegexMatch &, const std::string &).

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const char		*s,
		    const Regexp	&what,
		    unsigned		options,
		    const char		*with)
{ return replace (s, 0, what, options, with); }

/** Replace all matches of @a what with @a with in @a s.

    The regular expression search is executed with match @a options;
    see #Regexp for further details.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  Thus overlapping matches
    are never considered: for example, in @a s "aaaa", a @a what "aa"
    would be replaced twice, not three times, at positions 0 and 2.
    An empty @a with string causes all matches of @a what to be
    removed.  An empty @a what string causes @a with to be inserted
    before every character in @a s.

    The replacement text may contain back-references to the capturing
    subpatterns in @a what.  The back-references will be replaced with
    the corresponding matched regions from @a s.  For details on the
    syntax of suitable @a what patterns please see #replace(const
    std::string &, const RegexMatch &, const std::string &).

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const std::string	&s,
		    const Regexp	&what,
		    unsigned		options,
		    const std::string	&with)
{ return replace (s.c_str(), 0, what, options, with.c_str()); }

/** Replace all matches of @a what with @a with in @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  Thus overlapping matches
    are never considered: for example, in @a s "aaaa", a @a what "aa"
    would be replaced twice, not three times, at positions 0 and 2.
    An empty @a with string causes all matches of @a what to be
    removed.  An empty @a what string causes @a with to be inserted
    before every character in @a s.

    The replacement text may contain back-references to the capturing
    subpatterns in @a what.  The back-references will be replaced with
    the corresponding matched regions from @a s.  For details on the
    syntax of suitable @a what patterns please see #replace(const
    std::string &, const RegexMatch &, const std::string &).

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const char		*s,
		    int			offset,
		    const Regexp	&what,
		    const char		*with)
{ return replace (s, offset, what, 0, with); }

/** Replace all matches of @a what with @a with in @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  Thus overlapping matches
    are never considered: for example, in @a s "aaaa", a @a what "aa"
    would be replaced twice, not three times, at positions 0 and 2.
    An empty @a with string causes all matches of @a what to be
    removed.  An empty @a what string causes @a with to be inserted
    before every character in @a s.

    The replacement text may contain back-references to the capturing
    subpatterns in @a what.  The back-references will be replaced with
    the corresponding matched regions from @a s.  For details on the
    syntax of suitable @a what patterns please see #replace(const
    std::string &, const RegexMatch &, const std::string &).

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const std::string	&s,
		    int			offset,
		    const Regexp	&what,
		    const std::string	&with)
{ return replace (s.c_str(), offset, what, 0, with.c_str()); }

/** Replace all matches of @a what with @a with in @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    The regular expression search is executed with match @a options;
    see #Regexp for further details.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  Thus overlapping matches
    are never considered: for example, in @a s "aaaa", a @a what "aa"
    would be replaced twice, not three times, at positions 0 and 2.
    An empty @a with string causes all matches of @a what to be
    removed.  An empty @a what string causes @a with to be inserted
    before every character in @a s.

    The replacement text may contain back-references to the capturing
    subpatterns in @a what.  The back-references will be replaced with
    the corresponding matched regions from @a s.  For details on the
    syntax of suitable @a what patterns please see #replace(const
    std::string &, const RegexMatch &, const std::string &).

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const char		*s,
		    int			offset,
		    const Regexp	&what,
		    unsigned		options,
		    const char		*with)
{
    std::string result (s);
    RegexpMatch	match;
    int		index;
    int		delta = 0;

    while ((index = what.search (s, offset, options, &match)) >= 0)
    {
	ASSERT ((size_t) index < result.size ());
	ASSERT (index >= offset);
	ASSERT (match.numMatches () > 0);
	ASSERT (index == match.matchPos ());

	std::string replacement (replace (s, match, with));
	result.replace (match.matchPos () + delta, match.matchLength (), replacement);
	delta += int(replacement.size()) - match.matchLength();
	offset = std::max(offset+1, match.matchEnd());
    }

    return result;
}

/** Replace all matches of @a what with @a with in @a s.

    The search for @a what in @a s begins at @a offset, which as usual
    can be a negative to count from the end of the string.  The search
    proceeds in forward direction.

    The regular expression search is executed with match @a options;
    see #Regexp for further details.

    Once an instance of @a what has been replaced, the search for the
    next match begins after the end of the previous one: replacements
    are never made in already replaced text.  Thus overlapping matches
    are never considered: for example, in @a s "aaaa", a @a what "aa"
    would be replaced twice, not three times, at positions 0 and 2.
    An empty @a with string causes all matches of @a what to be
    removed.  An empty @a what string causes @a with to be inserted
    before every character in @a s.

    The replacement text may contain back-references to the capturing
    subpatterns in @a what.  The back-references will be replaced with
    the corresponding matched regions from @a s.  For details on the
    syntax of suitable @a what patterns please see #replace(const
    std::string &, const RegexMatch &, const std::string &).

    Returns the @a s with the changes made.  */
std::string
StringOps::replace (const std::string	&s,
		    int			offset,
		    const Regexp	&what,
		    unsigned		options,
		    const std::string	&with)
{
    return replace(s.c_str(), offset, what, options, with.c_str());
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Replace regular expression result @a matches in template @a
    pattern.  Use @a s as the string that was originally searched by
    the regular expression and produced @a matches.

    The @a pattern is searched for instances of "\DDD" where "D" is a
    decimal digit.  More specifically the pattern is searched for a
    backslash followed by a decimal digit.  When one is found, up to
    three decimal digits are taken as a regular expression capture
    reference.  The capture reference is replaced by the corresponding
    match section from @a s as recorded in @a matches.

    If a capture reference is larger than the number of matches in @a
    matches, the reference is left intact.  The zeroeth capture bounds
    the whole string matched by the regular expression.

    The pattern "A \1 was \2" will have the "\1" replaced by what ever
    was matched in @a s by the first capturing subpattern, and "\2"
    will be replaced similary the contents of the second capturing
    subpattern.  If @a matches has only one match, the second
    replacement will not occur.

    For example, "A \00010" will become "A " followed by the entire
    string matched by the regular expression (@c matches.match(0)),
    followed by "10".

    Returns the @a pattern string with the replacements done.  */
std::string
StringOps::replace (const char		*s,
		    const RegexpMatch	&matches,
		    const char		*pattern)
{
    static const char	digits [] = "0123456789";
    size_t		slen = strlen(s);
    std::string		result;
    char		ch;

    result.reserve (slen);
    for (const char *p = pattern; *p; ++p)
	if ((ch = *p) != '\\')
	    // Append anything that doesn't start with "\"
	    result += ch;
	else if (! p [1])
	    // Backslash in trailing position, leave it alone
	    result += ch;
    	else if ((ch = *++p) == '\\')
	    // "\\" => "\"
	    result += ch;
	else if (const char *d = strchr (digits, *p))
	{
	    // "\[digit]".  Keep going as long as we see digits and
	    // build the capture reference number from them.  We read
	    // at most three digits; if the client needs to follow the
	    // capture pattern with hardcoded digits, it can simply
	    // pad the capture reference with zeroes (e.g. "\00910"
	    // will always replace the ninth capture and then append
	    // the literal string "10" to it).
	    const char	*begin = p++;
	    int		n = d - digits;

	    while (p - begin < 3 && *p && (d = strchr (digits, *p)))
		n = n * 10 + (d - digits);

	    // If the pattern didn't have that many captures, leave
	    // this reference untouched.  Otherwise replace it with
	    // with the match string.
	    if (n >= matches.numMatches ())
		result += std::string (begin-1, p - begin + 1);
	    else
		result += matches.matchString (s, n);

	    // Backtrack one position since we are already at the
	    // character we want to reconsider, and the loop above
	    // will increment it again.
	    --p;
	}
	else
	    // False alarm, it was "\[non-digit]" -- keep it as it is
	    result += '\\', result += *p;

    return result;
}

/** Replace regular expression result @a matches in template @a
    pattern.  Use @a s as the string that was originally searched by
    the regular expression and produced @a matches.

    The @a pattern is searched for instances of "\DDD" where "D" is a
    decimal digit.  More specifically the pattern is searched for a
    backslash followed by a decimal digit.  When one is found, up to
    three decimal digits are taken as a regular expression capture
    reference.  The capture reference is replaced by the corresponding
    match section from @a s as recorded in @a matches.

    If a capture reference is larger than the number of matches in @a
    matches, the reference is left intact.  The zeroeth capture bounds
    the whole string matched by the regular expression.

    The pattern "A \1 was \2" will have the "\1" replaced by what ever
    was matched in @a s by the first capturing subpattern, and "\2"
    will be replaced similary the contents of the second capturing
    subpattern.  If @a matches has only one match, the second
    replacement will not occur.

    For example, "A \00010" will become "A " followed by the entire
    string matched by the regular expression (@c matches.match(0)),
    followed by "10".

    Returns the @a pattern string with the replacements done.  */
std::string
StringOps::replace (const std::string	&s,
		    const RegexpMatch	&matches,
		    const std::string	&pattern)
{
    return replace(s.c_str(), matches, pattern.c_str());
}

} // namespace lat
