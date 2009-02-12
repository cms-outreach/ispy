//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgArgsElement.h"
#include "classlib/utils/DebugAids.h"
#include <algorithm>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgArgsElement, "Services/Global/Args");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgArgsElement::IgArgsElement (IgState *in, int argc, char **argv)
    : m_state (in),
      m_argc (argc),
      m_argv (argv)
{
    ASSERT (argv);
    ASSERT (std::find (argv, argv+argc+1, (char *) 0) == argv+argc);
    m_state->put (s_key, this);
}

int &
IgArgsElement::args (void)
{ return m_argc; }

char *
IgArgsElement::arg (int arg)
{ ASSERT (arg >= 0); ASSERT (arg < args ()); return argv ()[arg]; }

char **
IgArgsElement::argv (void)
{ return m_argv; }

bool
IgArgsElement::find (const std::vector<std::string> &arg, std::string &value,
                     bool remove /*= false*/ )
{
    for(int i = 1; i < m_argc - 1; i++)
    {
	for(unsigned int j = 0; j < arg.size (); j++)
	{
	    if (arg[j] == m_argv[i])
	    {
		value = m_argv[i+1];
		if (remove)
		{ m_argv[i][0] = '\0'; m_argv[i+1][0] = '\0';}
		return true;
	    }
	}
    }
    return false;
}

bool
IgArgsElement::find (const std::string& arg, std::string &value,
                     bool remove /*= false*/ )
{
    std::vector<std::string> argument;
    argument.push_back(arg);
    return find(argument, value, remove);
}

bool  
IgArgsElement::find (const std::string& arg, int &value,
                     bool remove /*= false*/ )
{
    std::vector<std::string> argument;
    argument.push_back (arg);
    std::string strValue;
    if (find (argument, strValue, remove))
    {
	value = atoi (strValue.c_str());
	return true;
    }
    return false;
}

void
IgArgsElement::remove (const std::vector<std::string> &arg,
                       bool hasvalue /*= false*/)
{
    int total = hasvalue ? m_argc - 1 : m_argc;
    for(int i = 1; i < total; i++)
    {
	for(unsigned int j = 0; j < arg.size (); j++)
	{
	    if (arg[j] == m_argv[i])
	    {
		m_argv[i][0]='\0';
		if (hasvalue)
		    m_argv[i+1][0]='\0';
		return;
	    }
	}
    }
}

void
IgArgsElement::remove (const std::string &arg,
                       bool hasvalue /*= false*/)
{
    std::vector<std::string> argument;
    argument.push_back (arg);
    remove (argument, hasvalue);
}

bool
IgArgsElement::exists (const std::vector<std::string> &arg,
                       bool hasvalue /*= false*/)
{
    int total = hasvalue ? m_argc - 1 : m_argc;
    for(int i = 1; i < total; i++)
    {
	for(unsigned int j = 0; j < arg.size (); j++)
	{
	    if (arg[j] == m_argv[i])
		return true;
	}
    }
    return false;
}

bool
IgArgsElement::exists (const std::string &arg,
                       bool hasvalue /*= false*/)
{
    std::vector<std::string> argument;
    argument.push_back (arg);
    return exists (argument, hasvalue);
}

