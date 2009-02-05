//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgPython/interface/IgPythonCompleter.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgPythonCompleter::IgPythonCompleter (void)
{
}

bool IgPythonCompleter::isValid(const char */*name*/)
{
    return false;
}

std::string IgPythonCompleter::completeName(const char *objName)
{
    return std::string(objName);    
}

