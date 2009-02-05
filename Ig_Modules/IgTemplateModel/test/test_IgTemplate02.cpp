#include "Ig_Modules/IgTemplateModel/interface/IgTemplateEngine.h"
#include "Ig_Modules/IgTemplateModel/interface/IgTemplateModel.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Framework/IgPluginManager/interface/IgPluginManager.h"
#include <classlib/utils/DebugAids.h>
#include <iostream>

int 
main (int /*argc*/, char **/*argv*/)
{
    IgPluginManager::get ()->initialise ();    
    {
	IgTemplateModel model (new IgState ());
	model.textTemplate ("The root twig is called "
			    "@@@rep twig=\"/\" kind=\"twigFullName\"@@@ "
			    "@@@rep twig=\"/\" kind=\"twigFullName\"@@@");
	IgTwig *twig = new IgSimpleTwig ("Foo");    
	IgRepSet::lookup (twig, &model, true);
    
	ASSERT (model.text () == "The root twig is called /Foo /Foo");
    }
    {
	IgTemplateModel model (new IgState ());
	model.textTemplate ("@@@rep twig=\"/\" kind=\"twigFullName\"@@@ "
			    "The root twig is called "
			    "@@@rep twig=\"/\" kind=\"twigFullName\"@@@");
	IgTwig *twig = new IgSimpleTwig ("Foo");    
	IgRepSet::lookup (twig, &model, true);
    
	ASSERT (model.text () == "/Foo The root twig is called /Foo");
    }
    {
	IgTemplateModel model (new IgState ());
	model.textTemplate ("The root twig is called "
			    "@@@rep twig=\"/\" kind=\"twigFullName\"@@@"
			    "@@@rep twig=\"/\" kind=\"twigFullName\"@@@");
	IgTwig *twig = new IgSimpleTwig ("Foo");    
	IgRepSet::lookup (twig, &model, true);
	
	ASSERT (model.text () == "The root twig is called /Foo/Foo");
    }    
}

