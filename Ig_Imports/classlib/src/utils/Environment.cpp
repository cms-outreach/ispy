//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/Environment.h"
#include <string>

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Environment::~Environment (void)
{}

bool
Environment::has (const std::string &name) const
{
    bool have = false;
    get (name, &have);
    return have;
}

bool
Environment::get (const std::string &name, std::string &value) const
{
    bool have = false;
    value = get (name, &have);
    return have;
}

} // namespace lat
