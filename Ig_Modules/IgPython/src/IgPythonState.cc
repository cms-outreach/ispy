//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgPython/interface/IgPythonState.h"
#include <iostream>
#include "classlib/utils/Log.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgPythonState::IgPythonState (void)
{
}

IgState *
IgPythonState::get (void)
{
    LOG (0, trace, lat::LFgeneral, "Python IgState is: " << std::hex << s_pythonState << "\n");    
    return s_pythonState;    
}

void
IgPythonState::set (IgState *state)
{
    s_pythonState = state;    
}

IgState *IgPythonState::s_pythonState = 0;
