//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTemplateModel/interface/IgTemplateEngine.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Framework/IgPluginManager/interface/IgPluginManager.h"
#include <classlib/utils/DebugAids.h>
#include <iostream>

int 
main (int /*argc*/, char **/*argv*/)
{
    IgPluginManager::get ()->initialise ();    
    IgTemplateEngine *t 
	= new IgTemplateEngine ("The root twig is @@@rep twig=\"/\" kind=\"twigFullName\"@@@", 
				new IgState (),
				new IgSimpleTwig ("Foo"));    

    ASSERT (t->text () == "The root twig is /Foo");
}

