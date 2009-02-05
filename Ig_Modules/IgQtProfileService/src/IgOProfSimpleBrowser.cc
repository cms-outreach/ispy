//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfSimpleBrowser.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfRepresentable.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfProcessRepresentable.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfSymbolRepresentable.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOProfSimpleBrowser::IgOProfSimpleBrowser (IgOProfRepresentable *representable)
    :m_representable (representable)
{
}

void
IgOProfSimpleBrowser::dump (void)
{
    m_representable->requestUpdateProcessList ();
    
    for (IgOProfRepresentable::ProcessMap::const_iterator i = m_representable->begin (); 
	 i != m_representable->end (); 
	 i++)
    {
	std::cout << (*i).second->name () 
		  << " " 
		  << (*i).second->hits () 
		  << std::endl;		

	if ((*i).second->isExpanded ())
	{
	    for ( IgOProfProcessRepresentable::SymbolMap::iterator j = i->second->begin ();
		  j != i->second->end();
		  j++)
	    {
		std::cout << (*j).second->name () 
			  << " " << (*j).second->hits () 
			  << (*j).second->library () 
			  << std::endl;		    
	    }			
	}		
    }
}
