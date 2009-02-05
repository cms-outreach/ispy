//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgPython/interface/IgPythonService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Modules/IgPython/interface/IgPythonStream.h"
#include "classlib/utils/DebugAids.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgPythonService, "Extensions/Python");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


// TODO: use IgState to keep track of the different dictionaries.

IgPythonService::IgPythonService (IgState *state)
    :m_state (state),
     m_pythonStream (0) 
{
    // FIXME: this workaround is required because python init/deinit
    // code is done by the constructor/destructor of a static
    // IgPythonBoostrap object.
    static IgPythonStream *pythonStream = 0;
    
    if (pythonStream == 0)
    {
	pythonStream = new IgPythonStream ();	
	(*pythonStream) << "__name__ = \"__iguana__\"" << std::endl;
    }

    m_pythonStream = pythonStream;

    ASSERT (m_state);    
    m_state->put (s_key, this);    
}

IgPythonService::~IgPythonService (void)
{
    m_state->detach (s_key);    
}

IgPythonStream *
IgPythonService::pythonStream (void)
{
    return m_pythonStream;    
}

IgPythonInterpreter *
IgPythonService::interpreter (void)
{
    return m_pythonStream->interpreter ();
}
