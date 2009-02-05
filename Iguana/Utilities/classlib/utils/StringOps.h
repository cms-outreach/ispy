#ifndef CLASSLIB_OPS_STRING_OPS_H
# define CLASSLIB_OPS_STRING_OPS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/StringList.h"
# include <string>
# include <vector>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Regexp;
class RegexpMatch;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class StringOps
{
public:
    typedef std::vector<int>		FieldList;

    /** Options for separator handling in #split() and #section().
	The default (no options) for #split() is that empty fields are
	allowed, all field separators matter and separator is matched
	case-sensitively.  The default for #section() is to ignore
	leading and trailing empty fields (i.e. initial and trailing
	separators) but to allow empty internal fields, and to match
	the separator case-sensitively.  */
    enum FieldOption {
	/** Trim away all empty fields.  */
	TrimEmpty		= 1,

	/** Keep leading empty fields.  This is meaningful with
	    #split() only when combined with #TrimEmpty.  */
	KeepLeadingEmpty	= 2,

	/** Keep trailing empty fields.  This is meaningful with
	    #split() only when combined with #TrimEmpty.  */
	KeepTrailingEmpty	= 4,

	/** Ignore case when matching the separator.  This option
	    is not meaningful with regular expressions since case
	    handling is defined when constructing the regexp.  */
	CaseInsensitiveSep	= 8
    };

    ////////////////////////////////////////////////////////////
    static std::string	join	 (const StringList	&items,
				  const std::string	&sep);

    ////////////////////////////////////////////////////////////
    static StringList	grep	 (const StringList	&items,
				  const std::string	&str,
				  bool			caseless = false);
    static StringList	grep	 (const StringList	&items,
				  const Regexp		&rx);

    ////////////////////////////////////////////////////////////
    // returns number of times there is an occurance
    static int		contains (const char		*s,
				  char			what,
				  bool			caseless = false);

    static int		contains (const std::string	&s,
				  char			what,
				  bool			caseless = false);

    static int		contains (const char		*s,
				  const char		*what,
				  bool			caseless = false);

    static int		contains (const std::string	&s,
				  const std::string	&what,
				  bool			caseless = false);

    static int		contains (const char		*s,
				  const Regexp		&rx);
    
    static int		contains (const std::string	&s,
				  const Regexp		&rx);
    
    ////////////////////////////////////////////////////////////
    // find first match starting for offset (-1 = last, -2 = next to last, ...)
    // return the position of first match or -1 if no match was found
    static int		find	 (const char		*s,
				  char			what,
				  int			offset = 0,
				  bool			caseless = false);

    static int		find	 (const std::string	&s,
				  char			what,
				  int			offset = 0,
				  bool			caseless = false);

    static int		find	 (const char		*s,
				  const char		*what,
				  int			offset = 0,
				  bool			caseless = false);

    static int		find	 (const std::string	&s,
				  const std::string	&what,
				  int			offset = 0,
				  bool			caseless = false);

    static int		find	 (const char		*s,
				  const Regexp		&r,
				  int			offset = 0);

    static int		find	 (const std::string	&s,
				  const Regexp		&r,
				  int			offset = 0);

    ////////////////////////////////////////////////////////////
    // find first match starting at offset, working backwards in the string
    // return the position of the match or -1 if no match was found

    // FIXME: align with find!
    static int		rfind	 (const char		*s,
				  char			what,
				  int			offset = -1,
				  bool			caseless = false);

    static int		rfind	 (const std::string	&s,
				  char			what,
				  int			offset = -1,
				  bool			caseless = false);

    static int		rfind	 (const char		*s,
				  const char		*what,
				  int			offset = -1,
				  bool			caseless = false);

    static int		rfind	 (const std::string	&s,
				  const std::string	&what,
				  int			offset = -1,
				  bool			caseless = false);

    static int		rfind	 (const char		*s,
				  const Regexp		&r,
				  int			offset = -1);

    static int		rfind	 (const std::string	&s,
				  const Regexp		&r,
				  int			offset = -1);

    ////////////////////////////////////////////////////////////
    static StringList	split	 (const char		*s,
				  char			sep,
				  int			flags = 0,
				  int			nmax = 0,
				  int			first = 0,
				  int			last = 0);

    static StringList	split	 (const std::string	&s,
				  char			sep,
				  int			flags = 0,
				  int			nmax = 0,
				  int			first = 0,
				  int			last = 0);

    static StringList	split	 (const char		*s,
				  const char		*sep,
				  int			flags = 0,
				  int			nmax = 0,
				  int			first = 0,
				  int			last = 0);

    static StringList	split	 (const std::string	&s,
				  const std::string	&sep,
				  int			flags = 0,
				  int			nmax = 0,
				  int			first = 0,
				  int			last = 0);

    static StringList	split	 (const char		*s,
				  const Regexp		&sep,
				  int			flags = 0,
				  int			nmax = 0,
				  int			first = 0,
				  int			last = 0);

    static StringList	split	 (const std::string	&s,
				  const Regexp		&sep,
				  int			flags = 0,
				  int			nmax = 0,
				  int			first = 0,
				  int			last = 0);

    ////////////////////////////////////////////////////////////
    // treat s as a sequence of fields separated by rx.  return fields
    // from position start to end (inclusive).  If no end, all fields
    // from start onwards are returned. fields are numbered 0, 1, 2, ...
    // from left and -1, -2, ... counting from right.  flags specifies
    // options; see above.

    // FIXME: alignt flags position with split
    static std::string	section	 (const char		*s,
				  char			sep,
				  int			first,
				  int			last = -1,
				  int			flags = 0);

    static std::string	section	 (const std::string	&s,
				  char			sep,
				  int			first,
				  int			last = -1,
				  int			flags = 0);

    static std::string	section	 (const char		*s,
				  const char		*sep,
				  int			first,
				  int			last = -1,
				  int			flags = 0);

    static std::string	section	 (const std::string	&s,
				  const std::string	&sep,
				  int			first,
				  int			last = -1,
				  int			flags = 0);

    static std::string	section	 (const char		*s,
				  const Regexp		&sep,
				  int			first,
				  int			last = -1,
				  int			flags = 0);

    static std::string	section	 (const std::string	&s,
				  const Regexp		&sep,
				  int			first,
				  int			last = -1,
				  int			flags = 0);

    ////////////////////////////////////////////////////////////
    // remove every occurance of rx (== replace (s, rx, ""))
    static std::string	remove	 (const char		*s,
				  char			what,
				  int			offset = 0);

    static std::string	remove	 (const std::string	&s,
				  char			what,
				  int			offset = 0);

    static std::string	remove	 (const char		*s,
				  const char		*what,
				  int			offset = 0);

    static std::string	remove	 (const std::string	&s,
				  const std::string	&what,
				  int			offset = 0);

    static std::string	remove	 (const char		*s,
				  const Regexp		&what,
				  int			offset = 0);

    static std::string	remove	 (const std::string	&s,
				  const Regexp		&what,
				  int			offset = 0);

    ////////////////////////////////////////////////////////////
    // replace every occurance of rx in s with replacement
    // \1, \2/ $1, $2 are replaced with captured matches
    static std::string	replace	 (const char		*s,
				  char			what,
				  const char		*with);

    static std::string	replace	 (const std::string	&s,
				  char			what,
				  const std::string	&with);

    static std::string	replace	 (const char		*s,
				  int			offset,
				  char			what,
				  const char		*with);

    static std::string	replace	 (const std::string	&s,
				  int			offset,
				  char			what,
				  const std::string	&with);

    static std::string	replace	 (const char		*s,
				  const char		*what,
				  const char		*with);

    static std::string	replace	 (const std::string	&s,
				  const std::string	&what,
				  const std::string	&with);

    static std::string	replace	 (const char		*s,
				  int			offset,
				  const char		*what,
				  const char		*with);

    static std::string	replace	 (const std::string	&s,
				  int			offset,
				  const std::string	&what,
				  const std::string	&with);

    static std::string	replace	 (const char		*s,
				  const Regexp		&what,
				  const char		*with);

    static std::string	replace	 (const std::string	&s,
				  const Regexp		&what,
				  const std::string	&with);

    static std::string	replace	 (const char		*s,
				  const Regexp		&what,
				  unsigned		options,
				  const char		*with);

    static std::string	replace	 (const std::string	&s,
				  const Regexp		&what,
				  unsigned		options,
				  const std::string	&with);

    static std::string	replace	 (const char		*s,
				  int			offset,
				  const Regexp		&what,
				  const char		*with);

    static std::string	replace	 (const std::string	&s,
				  int			offset,
				  const Regexp		&what,
				  const std::string	&with);

    static std::string	replace	 (const char		*s,
				  int			offset,
				  const Regexp		&what,
				  unsigned		options,
				  const char		*with);

    static std::string	replace	 (const std::string	&s,
				  int			offset,
				  const Regexp		&what,
				  unsigned		options,
				  const std::string	&with);

    static std::string	replace	 (const char		*s,
				  const RegexpMatch	&matches,
				  const char		*pattern);

    static std::string	replace	 (const std::string	&s,
				  const RegexpMatch	&matches,
				  const std::string	&pattern);

private:
    static int		imemcmp	 (const char *x, const char *y, size_t len);
    static int		rawfind	 (const char		*s,
				  size_t		slen,
				  char			what,
				  int			offset = 0,
				  bool			caseless = false);

    static int		rawfind	 (const char		*s,
				  size_t		slen,
				  const char		*what,
				  size_t		len,
				  int			offset = 0,
				  bool			caseless = false);

    static int		rawrfind (const char		*s,
				  size_t		slen,
				  char			what,
				  int			offset,
				  bool			caseless = false);

    static int		rawrfind (const char		*s,
				  size_t		slen,
				  const char		*what,
				  size_t		len,
				  int			offset,
				  bool			caseless = false);

    static size_t	splitmax (int nmax, int first, int last);
    static FieldList	splitpos (const char		*s,
				  size_t		slen,
				  const char		*sep,
				  size_t		seplen,
				  int			flags,
				  size_t		nmax);

    static FieldList	splitpos (const char		*s,
				  size_t		slen,
				  char			sep,
				  int			flags,
				  size_t		nmax);

    static FieldList	splitpos (const char		*s,
				  size_t		slen,
				  const Regexp		&rx,
				  int			flags,
				  size_t		nmax);

    static StringList	split	 (const char		*s,
				  size_t		slen,
				  const FieldList	&fields,
				  int			nmax,
				  int			first,
				  int			last);

    static std::string	section	 (const char		*s,
				  size_t		slen,
				  const FieldList	&fields,
				  int			first,
				  int			last);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_OPS_STRING_OPS_H
