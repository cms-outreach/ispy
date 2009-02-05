//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgXMLModel/interface/IgUIDModel.h"
#include "Ig_Modules/IgXMLModel/interface/IgUIDRep.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include <classlib/utils/DebugAids.h>
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

int
main (int /*argc*/, char ** /*argv*/)
{
    IgSimpleTwig *twig1 = new IgSimpleTwig ("foo");
    IgSimpleTwig *twig2 = new IgSimpleTwig ("bar", twig1);
    std::cerr << twig1 << std::endl;
    std::cerr << twig2 << std::endl;
    
    IgUIDModel *model = new IgUIDModel ();

    IgRepSet::lookup (twig1, model, true);
    IgRepSet::lookup (twig2, model, true);
        
    for (IgUIDModel::RepSet::iterator i = model->begin ();
	 i != model->end ();
	 i++)
    {
	IgUIDRep *rep =  model->lookup (*i);
	ASSERT (rep->id () == *i);
	std::cerr << std::hex << rep->id () << std::endl;
	std::cerr << std::hex << *i << std::endl;	
	std::cerr << std::hex << rep << std::endl;	
	std::cerr << rep->context ()->object () << std::endl;
    }
}
