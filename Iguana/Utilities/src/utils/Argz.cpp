//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/Argz.h"
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

Argz::Argz (void)
{}

Argz::Argz (const std::string &command)
{ build (command); }

Argz::Argz (const char **argz)
{ build (argz); }

Argz::Argz (const char *a1, const char *a2)
{ const char *argz [] = { a1, a2, 0 }; build (argz); }

Argz::Argz (const char *a1, const char *a2, const char *a3)
{ const char *argz [] = { a1, a2, a3, 0 }; build (argz); }

Argz::Argz (const char *a1, const char *a2, const char *a3, const char *a4)
{ const char *argz [] = { a1, a2, a3, a4, 0 }; build (argz); }

Argz::Argz (const Argz &x)
{ build (x.argz ()); }

Argz &Argz::operator= (const Argz &x)
{
    if (this != &x)
    {
	clear ();
	build (x.argz ());
    }
    return *this;
}

Argz::~Argz (void)
{ clear (); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Argz &
Argz::clear (void)
{
    m_data.erase ();
    m_argz.clear ();
    return *this;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Argz &
Argz::add (const char *arg)
{ const char *argz [] = { arg, 0 }; build (argz); return *this; }

Argz &
Argz::add (const char **argz)
{ build (argz); return *this; }

Argz &
Argz::add (const Argz &x)
{ build (x.argz ()); return *this; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
const char **
Argz::argz (void) const
{ return (const char **) &m_argz [0]; }

std::string
Argz::quote (void) const
{
    static const char SAFECHARS [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				     "abcdefghijklmnopqrstuvwxyz"
				     "01234567890,./:_-+";
    std::string result;
    result.reserve (m_data.size () * 2);

    for (const char **arg = argz (); *arg; ++arg)
    {
	// Check if we need to quote this argument
	bool quoteit = false;
	for (const char *p = *arg; *p && !quoteit; ++p)
	    quoteit = !strchr (SAFECHARS, *p);

	// Add it to the result
	if (! quoteit)
	    // Safe, add as-is
	    result += *arg;
	else
	{
	    // Quote it.  What we do here is add a single quote
	    // at the beginning and the end, and then handle a
	    // few special characters differently.  Basically
	    // anything goes within single quotes except single
	    // quote itself *and* backslash.  The latter because
	    // different systems handle backslash within quotes
	    // differently, so we take it out and quote it with
	    // another backslash.  (FIXME: WIN32: Does this work?)
	    // (FIXME: Trim leading or trailing empty quotes.)
	    result += '\'';
	    for (const char *p = *arg; *p; ++p)
		if (*p == '\'')
		    result += "'\\''";
		else if (*p == '\\')
		    result += "'\\\\'";
		else
		    result += *p;
	    result += '\'';
	}

	// If this isn't the last argument, add a space
	if (arg[1])
	    result += ' ';
    }

    return result;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Argz::reargz (size_t n)
{
    char *p;
    bool add;

    m_argz.clear ();
    m_argz.reserve (n+1);
    for (p = &m_data [0], add = true; p < &m_data [0] + m_data.size (); ++p)
    {
	if (add)
	    m_argz.push_back (p);
	add = (*p == '\0');
    }

    m_argz.push_back (0);
}

void
Argz::build (const char **argz)
{
    if (! argz || ! *argz)
    {
	if (m_argz.empty ())
	    reargz (0);
	return;
    }

    size_t n;

    for (n = 0; *argz; ++n, ++argz)
	m_data.append (*argz, strlen (*argz) + 1);
    m_data.c_str ();

    reargz (m_argz.size () + n);
}

void
Argz::build (const std::string &command)
{
    // This algorithm is loosely based on libiberty's "buildargv()".
    // The most signficant difference is memory management, but a few
    // other things have changed as well.
    //
    // buildarv() takes a "const char *" as argument and treats an
    // empty input ("") different from null input (0): the former
    // becomes one zero-length argument.  Since we use std::string
    // input, it doesn't make sense to make the separation; we just
    // ignore an empty string.  Clients that really want an empty
    // string can add a single separator to the std::string.
    //
    // Secondly, the quoting rules are different; this implementation
    // follows the bourne shell rules, where backslash (\) is not
    // active within single quotes, and only quotes certain characters
    // within double quotes.  More precisely, a backslash is a quote
    // character only outside quotes, and within double quotes if it
    // is followed by another backslash or a double quote.  (Other
    // special shell characters are not treated specially here.
    // Backtick in particular may nest quote level within bourne
    // shell.)
    //
    // Even though std::string can contain null characters, we stop at
    // first null for security reasons.  Otherwise there is too much
    // danger of being able to carry "hidden" information within the
    // string -- data that is ignored by some parts of the application
    // but taken into account here.
    //
    // Any space character will do as a separator (cf. isspace()).
    // Consequtive separators are considered as one.  Leading and
    // trailing separators are ignored, except that a "command"
    // consisting solely of spaces will result in one empty ("")
    // argument.

    if (command.empty ())
    {
	if (m_argz.empty ())
	    reargz (0);
	return;
    }

    std::string copy (command);
    const char	*input;
    size_t	n = 0;

    // First make sure it is null-terminated.  This may cause resize.
    input = copy.c_str ();
    while (*input)
    {
	// Skip leading separators.
	while (isspace (*input))
	    ++input;

	// Scan the argument.  As we do so, we move the cleaned-up
	// argument to the beginning of "copy", and then it to m_argz
	// once we are at the end of the argument.
	char *arg = &copy [0];
	bool squote = false;
	bool dquote = false;
	bool bslash = false;

	while (*input)
	{
	    if (isspace (*input) && ! squote && ! dquote && ! bslash)
		// Unquoted space terminates the argument.
		break;
	    else
	    {
		if (bslash)
		{
		    bslash = false;
		    *arg++ = *input;
		}
		else if (*input == '\\'
			 && ! squote
			 && (! dquote
			     || input[1] == '"'
			     || input[1] == '\\'))
		    bslash = true;
		else if (squote)
		{
		    if (*input == '\'')
			squote = false;
		    else
			*arg++ = *input;
		}
		else if (dquote)
		{
		    if (*input == '"')
			dquote = false;
		    else
			*arg++ = *input;
		}
		else
		{
		    if (*input == '\'')
			squote = true;
		    else if (*input == '"')
			dquote = true;
		    else
			*arg++ = *input;
		}
		++input;
	    }
	}

	// FIXME: Throw an exception if dquote || squote || bslash?

	// Terminate it and append it to m_argz.  Remember how
	// many arguments we have seen as a hint for reargz().
	char c = *arg;
	*arg = '\0';
	m_data.append (&copy[0], arg - &copy [0] + 1);
	*arg = c;
	n++;

	// Skip trailing space
	while (isspace (*input))
	    ++input;
    }

    reargz (m_argz.size () + n);
}

} // namespace lat
