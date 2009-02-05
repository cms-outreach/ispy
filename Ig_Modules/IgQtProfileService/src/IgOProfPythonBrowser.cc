//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfPythonBrowser.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfRepresentableInterface.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfRepresentable.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfProcessRepresentable.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfSymbolRepresentable.h"

#include <string>
#include <sstream>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


IgOProfPythonBrowser::IgOProfPythonBrowser (IgOProfRepresentableInterface *representable)
    :m_representable(representable)
{    
    if (m_representable == 0)
    {
	m_representable = IgOProfRepresentableSingleton::instance();	
    } 

    ASSERT(m_representable != 0);    
}

std::string 
IgOProfPythonBrowser::getResults()
{
    std::ostringstream s;

    s << "rep = IgOProfRepresentableConcrete()\n";    

    for (IgOProfRepresentable::ProcessMap::iterator i = m_representable->begin(); 
	 i !=  m_representable->end ();
	 i++)
    {
	IgOProfProcessRepresentable &currentProcess = *(i->second);
	
	s << "p=rep.getProcess(\"" << currentProcess.name () << "\")\n" 
	  << "p.m_hits=" << currentProcess.hits () << "\n";
	if((currentProcess.isExpanded () == true)
	   && (currentProcess.upToDate () == false))
	{	    
	    for (IgOProfProcessRepresentable::SymbolMap::iterator j = currentProcess.begin();
		 j != currentProcess.end ();
		 j++)
	    {
		IgOProfSymbolRepresentable &currentSymbol = *(j->second);
	    
		s << "s=p.getSymbol(\"" << currentSymbol.name () << "\")\n"
		  << "s.m_hits=" << currentSymbol.hits () << "\n";	    
	    }
	}	
    }
    s << std::flush;
    return s.str();
}
