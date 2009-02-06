#ifndef CLASSLIB_REGEXP_H
# define CLASSLIB_REGEXP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/RegexpMatch.h"
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

class Regexp
{
public:
    ////////////////////////////////////////////////////////////
    // Predefined regular expressions.
    static const Regexp rxwhite;	// = \s+   (or: [ \n\t\r\v\f]+)
    static const Regexp rxint;		// = -?\d+ (or: -?[0-9]+)
    static const Regexp rxdouble;	// = -?(\d+\.\d*|\d+|\.\d+)([eE][-+]?\d+)?
    // = -?(([0-9]+\\.[0-9]*)|([0-9]+)|(\\.[0-9]+))([eE][---+]?[0-9]+)?
    static const Regexp rxalpha;	// = [[:alpha:]]+
    static const Regexp rxlowercase;	// = [[:lower:]]+
    static const Regexp rxuppercase;	// = [[:upper:]]+
    static const Regexp rxalphanum;	// = [[:alpha:]\d]+
    static const Regexp rxidentifier;	// = [A-Za-z_$][A-Za-z0-9_$]*

    ////////////////////////////////////////////////////////////
    // Regular expression syntax.
    enum Syntax {
	POSIXBasic	= 0x0001,	//< POSIX.1 basic regular expression
	POSIXExtended	= 0x0002,	//< POSIX.1 extended regular expression
	Perl		= 0x0003,	//< PERL regular expression
	Wildcard	= 0x0004	//< Bourne shell file name glob pattern
    };

    ////////////////////////////////////////////////////////////
    // Regular expression options.
    enum Options {
	AnchorAtStart	= 0x0010, // PCRE
	IgnoreCase	= 0x0020, // PCRE, POSIX
	DollarAtEndOnly	= 0x0040, // PCRE
	DotMatchesAll	= 0x0080, // PCRE
	Extended	= 0x0100, // PCRE
	MultiLine	= 0x0200, // PCRE
	Minimal		= 0x0400, // PCRE
	// UTF8Chars	= 0x0400, // PCRE
	// Extra	= 0x0800, // PCRE
	// NoMatchData	= 0x1000, // PCRE?, POSIX
	StandardLocale	= 0x2000  // PCRE, POSIX?
    };

    ////////////////////////////////////////////////////////////
    // Match options.
    enum MatchOptions {
	/** Option to indicate that the beginning-of-line operator (^)
	    should fail at the beginning of the match string.  Use
	    this option when portions of a string are passed to
	    #match() and the beginning of the string is not a
	    beginning of line.

	    When #match() is called without this option and a non-zero
	    start offset, the beginning of the string, not the start
	    offset, is considered to be beginning of line.  This is
	    useful when looking for further matches in the string
	    after a previous success.

	    This option is valid for PERL and POSIX regexps (WILDCARD
	    regexps do not have the concept).  */
	MatchNotBOL	= 0x01,

	/** Option to indicate that the beginning-of-line operator (^)
	    should succeed at the given start offset, not at the
	    beginning of the string.  Use this option when portions of
	    a string are passed to #match() and the beginning of the
	    string is at the supplied start offset.

	    Without this option, #match() assumes that the beginning
	    of the string, not the start offset, is the beginning of
	    the line (except if #NotBOL is given).

	    This option is valid for PERL and POSIX regexps (WILDCARD
	    regexps do not have the concept).  */
	MatchOffsetBOL	= 0x02,

	/** Option to indicate that the end-of-line operator ($)
	    should fail at the end of the match string.  Use this
	    option when passing portions of strings to #match() and
	    the end of the string is not a end of line.

	    This option is valid for PERL and POSIX regexps (WILDCARD
	    regexps do not have the concept).  */
	MatchNotEOL	= 0x04,

	/** Option to indicate that an empty string should not be
	    considered a valid match.  If there are alternatives in
	    the pattern, they are tried.  If all alternatives match an
	    empty string, the entire match fails.

	    This option is valid only for PERL regexps.  */
	MatchNoEmpty	= 0x08,

	/** FIXME (PCRE_ANCHORED)  */
	MatchAnchored	= 0x10,

	/** Option to indicate to look for all possible matches.  */
	MatchAll	= 0x20,

	/** Option to indicate that result should not be automatically
	    cleared but appended to.  Use this to search incrementally
	    for more than one match, without it the search methods
	    reset the match result. */
	MatchContinue	= 0x40
    };

    ////////////////////////////////////////////////////////////
    Regexp (void);
    Regexp (const std::string &pat, unsigned options = 0, Syntax s = Perl);
    // Regexp (const std::string &pat, const char *options, Syntax s = Perl);
    Regexp (const Regexp &x);
    Regexp &operator= (const Regexp &x);
    Regexp &operator= (const std::string &x);
    ~Regexp (void);

    bool		operator== (const Regexp &x) const;
    bool		operator!= (const Regexp &x) const;

    static std::string	escape (const std::string &text);

    Syntax		syntax (void) const;
    bool		empty (void) const;
    bool		valid (void) const;

    int			errorOffset (void) const;
    const char *	errorMessage (void) const;
    void		clearError (void);

    unsigned		options (void) const;
    void		setOptions (unsigned value);
    void		enableOptions (unsigned mask);
    void		disableOptions (unsigned mask);

    std::string		pattern (void) const;
    void		setPattern (const std::string &pat);

    bool		compile (bool force = false);
    void		study (void);

    bool		exactMatch (const char *s) const;
    bool		exactMatch (const std::string &s) const;

    bool		match (const char *s, int offset = 0,
			       int flags = 0, RegexpMatch *result = 0) const;
    bool		match (const std::string &s, int offset = 0,
			       int flags = 0, RegexpMatch *result = 0) const;

    int			search (const char *s, int offset = 0,
				int flags = 0, RegexpMatch *result = 0) const;
    int			search (const std::string &s, int offset = 0,
				int flags = 0, RegexpMatch *result = 0) const;

    int			rsearch (const char *s, int offset = -1,
				 int flags = 0, RegexpMatch *result = 0) const;
    int			rsearch (const std::string &s, int offset = -1,
				 int flags = 0, RegexpMatch *result = 0) const;

protected:
    void		uncompile (void);

    bool		doExactMatch(const char *s, size_t len) const;
    bool		doMatch(const char *s, size_t len, int offset,
		    		int flags, RegexpMatch *result) const;
    int			doSearch(const char *s, size_t len, int offset,
		    		 int flags, RegexpMatch *result) const;
    int			doRevSearch(const char *s, size_t len, int offset,
		    		    int flags, RegexpMatch *result) const;

    // bool		dosearch (const std::string &s, int offset);
    // void		split (const std::string &s, int limit, int start,
    //			       int end, StringList &into);
    // std::string	replaceVarRefs (const std::string &s);
    // void		zero (void);

    // unsigned		parseMods (const char *mods);
    // int		doSubs (const std::string &s, std::string &final,
    // 				const char *with, int *ovec, int nmatch);

private:
    static const int SyntaxMask = 0x000F;
    struct Rep;
    struct PCRERep;
    struct POSIXRep;

    std::string		m_pattern;
    unsigned		m_options;
    Rep			*m_rep;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_REGEXP_H
