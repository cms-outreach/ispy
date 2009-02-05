//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/ShellEnvironment.h"
#include <cstdlib>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

#if !HAVE_UNSETENV
extern "C" { extern char **environ; }
#endif

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

bool
ShellEnvironment::has (const std::string &name) const
{ return getenv (name.c_str ()); }

std::string
ShellEnvironment::get (const std::string &name, bool *have /* = 0 */) const
{
    std::string value;
    if (const char *val = getenv (name.c_str ()))
    {
	value = val;
	if (have) *have = true;
    }
    else
    {
	value = "";
	if (have) *have = false;
    }

    return value;
}


void
ShellEnvironment::unset (const std::string &name)
{
#if HAVE_UNSETENV
    unsetenv (name.c_str ());
#else
    char **val;
    char **out;
    for (val = out = environ; *val; ++val)
	if (strncmp (*val, name.c_str (), name.size ())
	    || (*val) [name.size ()] != '=')
	    *out++ = *val;

    *out++ = 0;
#endif
}

void
ShellEnvironment::set (const std::string &name, const std::string &value)
{
#if HAVE_SETENV
    setenv (name.c_str (), value.c_str (), true);
#else
    char *val = (char *) malloc (name.size () + 1 + value.size () + 1);
    strcpy (val, name.c_str ());
    strcat (val, "=");
    strcat (val, value.c_str ());
    putenv (val);
#endif
}

} // namespace lat
