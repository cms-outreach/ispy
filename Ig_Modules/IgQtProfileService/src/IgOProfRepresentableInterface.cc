//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfRepresentableInterface.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOProfProcessRepresentable *
IgOProfRepresentableInterface::getProcess(const char *name)
{
    IgOProfProcessRepresentable *&returnValue = m_processMap[name];
    
    if (!returnValue)
    {
	IgOProfProcessRepresentable *tmp;
	tmp = new IgOProfProcessRepresentable (name);	
	m_processMap[name] = tmp;	
	returnValue = tmp;
    }
    
    ASSERT (returnValue);
    
    return returnValue;
}
