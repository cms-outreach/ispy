//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfNetworkRepresentable.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"
#include "Ig_Modules/IgPython/interface/IgPythonStream.h"
#include <fstream>
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

IgOProfNetworkRepresentable::IgOProfNetworkRepresentable (void)
    :m_configuration( IgOProfConfigurationSingleton::instance () ),
     m_pythonStream (new IgPythonStream () )
{
    std::ifstream pythonScript("share/Client.py");
    m_script << pythonScript.rdbuf () <<std::endl;
    
    *m_pythonStream << "from PyIgQtProfileService import *"<< std::endl;    
    pythonScript.close ();
}

void
IgOProfNetworkRepresentable::requestUpdateProcessList (void)
{
    std::string result;   
    std::string updateProcessScript;
    
    for (ProcessMap::iterator i = begin (); i != end (); i++)
    {
	IgOProfProcessRepresentable &t = *(i->second);
	
	if (t.upToDate () == false 
	    && t.isExpanded () == true)
	{
	    updateProcessScript += "p=rep.getProcess ('"+t.name ()+"')\\n";
	    updateProcessScript += "p.setIsExpanded (1)\\n";
	    updateProcessScript += "p.setUpToDate (0)\\n";	    
	}
    }
    
    (*m_pythonStream) << "updateProcessScript=\"" + updateProcessScript+"\"\n";
    
    *m_pythonStream << m_script.str () << std::flush;
}
