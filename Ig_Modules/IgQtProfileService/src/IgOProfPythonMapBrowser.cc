//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfPythonMapBrowser.h"
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


IgOProfPythonMapBrowser::IgOProfPythonMapBrowser (IgOProfRepresentableInterface *representable)
    :m_representable (representable)
{    
    if (m_representable == 0)
    {
	m_representable = IgOProfRepresentableSingleton::instance ();	
    } 

    ASSERT (m_representable != 0);    
}

std::string 
IgOProfPythonMapBrowser::getResults (void)
{
    std::ostringstream s;

    s << "rep={}\n";
    

    for (IgOProfRepresentable::ProcessMap::iterator i = m_representable->begin (); 
	 i !=  m_representable->end ();
	 i++)
    {
	IgOProfProcessRepresentable &currentProcess = *(i->second);
	
	s << "rep['" << currentProcess.name () << "']=" <<  currentProcess.hits () << "\n";
    }
    s << std::flush;
    return s.str();
}
