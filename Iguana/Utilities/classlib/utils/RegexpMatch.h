#ifndef CLASSLIB_REGEXP_MATCH_H
# define CLASSLIB_REGEXP_MATCH_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/StringList.h"
# include <utility>
# include <string>
# include <vector>
# include <list>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class RegexpMatch
{
public:
    /** Match bounds marker.  The match begins at @c first and ends
	one before @c second.  The matched substring is empty if @c
	first == @c second.  If this subpattern (capture) did not
	match at all, @c first and @c second are both -1.  */
    typedef std::pair<int,int>		Marker;

    /** Vector of match markers.  The first element marks the region
	matched by the entire regular expression, elements therearfter
	are for the pattern subexpressions (captures), in the order of
	appearance of the left opening brace in the pattern.  If the
	regular expression was matched several times, for each match
	a set of markers is added: one for the whole expression and
	subsequent ones for the subcaptures.  */
    typedef std::vector<Marker>		MarkerList;

    RegexpMatch (void);
    RegexpMatch (const RegexpMatch &x);
    RegexpMatch &operator= (const RegexpMatch &x);
    ~RegexpMatch (void);

    // Interface for adding match results
    void		setCaptures  (int n);
    void		addMatchSet  (void);
    void		addMatchPos  (int begin, int end);
    void		addMatches   (const RegexpMatch &match);

    // what do matchPos/End/Length/String return when there is no match?
    void		reset        (void);

    int			numCaptures  (void) const;
    int			numMatchSets (void) const;
    int			numMatches   (void) const;

    MarkerList		allMatches   (void) const;
    bool		matched      (int n = 0) const;
    int			matchPos     (int n = 0) const;
    int			matchEnd     (int n = 0) const;
    int			matchLength  (int n = 0) const;

    std::string		matchString  (const std::string &s, int n = 0) const;
    static std::string	matchString  (const std::string &s, const Marker &m);
    StringList		matchStrings (const std::string &s) const;
    static StringList	matchStrings (const std::string &s, const MarkerList &m);

    std::string		matchString  (const char *s, int index = 0) const;
    static std::string	matchString  (const char *s, const Marker &m);
    StringList		matchStrings (const char *s) const;
    static StringList	matchStrings (const char *s, const MarkerList &m);

private:
    int			m_captures;
    int			m_matches;
    MarkerList		m_markers;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_REGEXP_MATCH_H
