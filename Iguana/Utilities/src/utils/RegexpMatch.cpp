//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/RegexpMatch.h"
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

/** Initialise an empty regular expression match.  The number of
    captures and match sets are set both to zero.  */
RegexpMatch::RegexpMatch (void)
    : m_captures (0),
      m_matches (0)
{}

/** Copy all the match information about a regular expression.  */
RegexpMatch::RegexpMatch (const RegexpMatch &x)
    : m_captures (x.m_captures),
      m_matches (x.m_matches),
      m_markers (x.m_markers)
{}

/** Copy all the match information about a regular expression.  */
RegexpMatch &
RegexpMatch::operator= (const RegexpMatch &x)
{
    m_captures = x.m_captures;
    m_matches = x.m_matches;
    m_markers = x.m_markers;
    return *this;
}

/** Release the resources used by the match information.  */
RegexpMatch::~RegexpMatch (void)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Set the number of capturing subpatterns in this regular expression
    match to @a n.

    Producers of a #RegexpMatch must invoke this method to indicate
    how many match markers clients should expect to find.  The number
    of markers is always num-matches*(num-captures+1).  The matchers
    are expected to subsequently call #addMatchSet(), and then for
    each such call to #addMatchPos() num-captures+1 times, i.e. @a n +
    1 times.

    It is ok to invoke this method more than once as long as @a n is
    not actually changing.  To reuse the object for a different
    #Regexp, first invoke #reset() and then this method again.  */
void
RegexpMatch::setCaptures (int n)
{ m_captures = n; }

/** Add a new match set to this object.  This call marks the beginning
    of a new regular expression match.  The client is expected to
    follow this call with immediate calls to #addMatchPos() the
    appropriate number of times (see #setCaptures() for more detail).  */
void
RegexpMatch::addMatchSet (void)
{ ++m_matches; m_markers.reserve (m_markers.size () + m_captures + 1); }

/** Add a new match to the current match set: @a begin marks the
    beginning of the capture match, @a end marks one past the end.

    @a begin == @a end means that an empty string was matched by the
    capturing pattern.  @a begin == @a end == -1 means that the
    subpattern did not match at all.

    #addMatchSet() must have been called prior to this method, and
    this method must be invoked #numCaptures()+1 times for each call
    to #addMatchSet().  */
void
RegexpMatch::addMatchPos (int begin, int end)
{ m_markers.push_back (Marker (begin, end)); }

	    
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Add all match data including the number of captures from @a match. */
void
RegexpMatch::addMatches (const RegexpMatch &match)
{
    m_captures = match.m_captures;
    m_matches += match.m_matches;
    m_markers.reserve (m_matches * (m_captures + 1));
    for (size_t i = 0; i < match.m_markers.size(); ++i)
	m_markers.push_back (match.m_markers [i]);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Reset the match result.  Clears all markers and all knowledge
    about capturing subpatterns.  */
void
RegexpMatch::reset (void)
{
    // Clear the marker list
    MarkerList junk;
    std::swap (m_markers, junk);

    // Reset match and capture count
    m_captures = m_matches = 0;
}

/** Return the number of capturing subpatterns in the last match
    operation.  This number does not include the match returned for
    the regular expression itself, only the real subpatterns in it.
    Thus #numMatches() is (#numCaptures()+1)*#numMatchSets().  */
int
RegexpMatch::numCaptures (void) const
{ return m_captures; }

/** Return the number of times the regular expression matches
    accumulated in this result.  A simple match results in one match
    set.  If the regular expression was matched several times, then
    the result includes a set of match markers for each match.  That
    is, #numMatches() is (#numCaptures()+1)*#numMatchSets(). */
int
RegexpMatch::numMatchSets (void) const
{ return m_matches; }

/** Return the number of matches recorded in this result.  The first
    match is for the whole regular expression, and then one for each
    capturing subpattern: #numCaptures()+1 in total.  If the regular
    expression was searched and matched several times, this repeats
    #numMatchSets() times.  That is, #numMatches() is
    (#numCaptures()+1)*#numMatchSets().

    If the regular expression did not match at all, no matches are
    added to the result.  If the regular expression did match but a
    subpattern did not, the marker pair will be (-1, -1) for that
    capture.  */
int
RegexpMatch::numMatches (void) const
{ return m_markers.size (); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Return all marker positions in one operation.  */
RegexpMatch::MarkerList
RegexpMatch::allMatches (void) const
{ return m_markers; }

/** Check if the subexpression @a n (by default zero, i.e. the whole
    regular expression) matched.  Returns @c true if it did, @c false
    otherwise.  */
bool
RegexpMatch::matched (int n /* = 0 */) const
{
    ASSERT (n >= 0 && n < numMatches ());
    return m_markers [n].first != -1;
}

/** Return the beginning offset of the substring matched by subpattern
    number @a n.  By default @a n is zero, i.e. returns the beginning
    of the match of the whole regular expression.  See #matchEnd() on
    how to retrieve the one-past-end offset and #matchLength() to
    return the match length.

    If the subpattern did not match, returns -1.

    This method must not be called if the regular expression did not
    match at all, as no match data exists in that case.  You can find
    out whether the match failed by inspecting the return value of the
    match function; for example, #Regexp::search() returns -1 in case
    of no match. */
int
RegexpMatch::matchPos (int n /* = 0 */) const
{
    ASSERT (n >= 0 && n < numMatches ());
    return m_markers [n].first;
}

/** Return offset one past the end of the substring matched by
    subpattern number @a n.  By default @a n is zero, i.e. returns the
    one-past-end of the whole regular expression match.  See
    #matchPos() on how to retrieve the beginning offset and
    #matchLength() to return the match length.

    If the subpattern did not match, returns -1.

    This method must not be called if the regular expression did not
    match at all, as no match data exists in that case.  You can find
    out whether the match failed by inspecting the return value of the
    match function; for example, #Regexp::search() returns -1 in case
    of no match. */
int
RegexpMatch::matchEnd (int n /* = 0 */) const
{
    ASSERT (n >= 0 && n < numMatches ());
    return m_markers [n].second;
}

/** Return the length of the substring matched by subpattern number @a
    n.  By default @a n is zero, i.e. returns the length of the string
    matched by the whole regular expression.  See #matchPos() on how
    to retrieve the beginning offset and #matchEnd() for the
    one-past-end offset.

    If the subpattern did not match, returns 0.

    This method must not be called if the regular expression did not
    match at all, as no match data exists in that case.  You can find
    out whether the match failed by inspecting the return value of the
    match function; for example, #Regexp::search() returns -1 in case
    of no match. */
int
RegexpMatch::matchLength (int n /* = 0 */) const
{
    ASSERT (n >= 0 && n < numMatches ());
    const Marker &m = m_markers [n];
    return m.second - m.first;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Return the substring matched by subpattern number @a n.  By
    default @a n is zero, i.e. returns the region matched by the whole
    regular expression.  @a s must be the string that was used in
    matching the regular expression.

    See #matchPos(), #matchEnd() and #matchLength() for retrieving the
    related match offsets.

    This method returns an empty string if either the subpattern did
    not match at all, or if the pattern simply matched a zero-length
    string.  Use #matched() to distinguish the two cases.  */
std::string
RegexpMatch::matchString (const std::string &s, int n /* = 0 */) const
{
    ASSERT (n >= 0 && n < numMatches ());
    return matchString (s, m_markers [n]);
}

/** Return the substring delimited by match marker @a m.  @a s must be
    the string that was used in matching the regular expression.

    This method returns an empty string if either the subpattern did
    not match at all, or if the pattern simply matched a zero-length
    string.  Use #matched() to distinguish the two cases.  */
std::string
RegexpMatch::matchString (const std::string &s, const Marker &m)
{
    if (m.first == -1)
	return "";

    ASSERT (m.first >= 0);
    ASSERT (m.second >= 0);
    ASSERT (m.second >= m.first);
    ASSERT (size_t (m.second) <= s.size ());

    return std::string (s, m.first, m.second - m.first);
}

/** Return all the match substrings.  @a s must be the string that was
    used in matching the regular expression.

    The result will have #numMatches() elements; some of them will be
    empty if the corresponding subpattern did not match or matched an
    empty string.  See #matchString() for further details.  */
StringList
RegexpMatch::matchStrings (const std::string &s) const
{ return matchStrings (s, m_markers); }

/** Return all the match substrings listed in @a m.  @a s must be the
    string that was used in matching the regular expression.

    The result will have as many elements as @a m; some of them will
    be empty if the corresponding range did not match or matched an
    empty string.  See #matchString() for further details.  */
StringList
RegexpMatch::matchStrings (const std::string &s, const MarkerList &m)
{
    StringList result;
    result.reserve (m.size ());
    for (size_t i = 0; i < m.size (); ++i)
	result.push_back (matchString (s, m [i]));

    return result;
}

//////////////////////////////////////////////////////////////////////
/** Return the substring matched by subpattern number @a n.  By
    default @a n is zero, i.e. returns the region matched by the whole
    regular expression.  @a s must be the string that was used in
    matching the regular expression.

    See #matchPos(), #matchEnd() and #matchLength() for retrieving the
    related match offsets.

    This method returns an empty string if either the subpattern did
    not match at all, or if the pattern simply matched a zero-length
    string.  Use #matched() to distinguish the two cases.  */
std::string
RegexpMatch::matchString (const char *s, int n /* = 0 */) const
{
    ASSERT (n >= 0 && n < numMatches ());
    return matchString (s, m_markers [n]);
}

/** Return the substring delimited by match marker @a m.  @a s must be
    the string that was used in matching the regular expression.

    This method returns an empty string if either the subpattern did
    not match at all, or if the pattern simply matched a zero-length
    string.  Use #matched() to distinguish the two cases.  */
std::string
RegexpMatch::matchString (const char *s, const Marker &m)
{
    if (m.first == -1)
	return "";

    ASSERT (m.first >= 0);
    ASSERT (m.second >= 0);
    ASSERT (m.second >= m.first);

    return std::string (s + m.first, m.second - m.first);
}

/** Return all the match substrings.  @a s must be the string that was
    used in matching the regular expression.

    The result will have #numMatches() elements; some of them will be
    empty if the corresponding subpattern did not match or matched an
    empty string.  See #matchString() for further details.  */
StringList
RegexpMatch::matchStrings (const char *s) const
{ return matchStrings (s, m_markers); }

/** Return all the match substrings listed in @a m.  @a s must be the
    string that was used in matching the regular expression.

    The result will have as many elements as @a m; some of them will
    be empty if the corresponding range did not match or matched an
    empty string.  See #matchString() for further details.  */
StringList
RegexpMatch::matchStrings (const char *s, const MarkerList &m)
{
    StringList result;
    result.reserve (m.size ());
    for (size_t i = 0; i < m.size (); ++i)
	result.push_back (matchString (s, m [i]));

    return result;
}

} // namespace lat
