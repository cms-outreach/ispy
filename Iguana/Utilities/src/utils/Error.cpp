//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/Error.h"

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

Error::Error (Error *chain /* = 0 */)
    : m_chain (chain)
{}

/** Copy the exception object @a x.  Makes a deep copy of the chain.
    In general this method needs to be overridden in derived classes
    only if they contain data members that cannot be copied correctly
    trivially.  The derived method does not need to copy the chain,
    just itself.  */
Error::Error (const Error &x)
    : m_chain (x.m_chain ? x.m_chain->clone () : 0)
{}

/** Copy the exception object @a x.  Makes a deep copy of the chain.
    In general this method needs to be overridden in derived classes
    only if they contain data members that cannot be copied correctly
    trivially.  The derived method does not need to copy the chain,
    just itself.  */
Error &
Error::operator= (const Error &x)
{
    if (this != &x)
    {
	delete m_chain;
	m_chain = 0;

	if (x.m_chain)
	    m_chain = x.m_chain->clone ();
    }

    return *this;
}

Error::~Error (void)
{ delete m_chain; }

/** Describe the reason for the error as text.  Unravels the chain for
    up to @a depth levels in addition to this error object, or the
    full chain if @a depth is negative.  */
std::string
Error::explain (int depth /* = -1 */) const
{
    std::string result (explainSelf ());
    Error	*chain = next ();

    if (chain && (depth < 0 || depth > 0))
    {
	result += " (because of ";
   	result += chain->explain (depth < 0 ? depth : depth-1);
       	result += ")";
    }

    return result;
}

} // namespace lat
