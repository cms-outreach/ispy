//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgWebLego/interface/IgWebLegoService.h"
#include "Ig_Modules/IgOIVBrowser/interface/IgOIVBrowser.h"
#include "Ig_Modules/IgStudio/interface/IgDocumentData.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include "Ig_Modules/IgWebStudio/interface/IgBrowserManager.h"
#include <classlib/utils/Log.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/StringOps.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/SbLinear.h>
#include <qtextstream.h>
#include <qdatastream.h>
#include <qiodevice.h>
#include <qfile.h>
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

IgWebLegoService::IgWebLegoService (IgState *state)
    : IgWebService (state)
{
    registerCallback ("create", lat::CreateCallback (this, &IgWebLegoService::create));
    registerCallback ("browse", lat::CreateCallback (this, &IgWebLegoService::browse));
}


void
IgWebLegoService::create (Arguments *arguments)
{    
    IgState *state = arguments->state ();
    QIODevice *outputDevice = arguments->outputDevice ();    
    ArgumentsMap &args = *arguments->args ();


    std::string lookupName = "";
    
    if (args.find ("name") != args.end ())
	lookupName = args["name"];
    
    this->doCreate (state, lookupName);
    this->sendDone (outputDevice);    
}

void
IgWebLegoService::doCreate (IgState *state, const std::string &name)
{
    IgBrowserManager *manager =  IgBrowserManager::get (state);
    ASSERT (manager);
    
    IgOIVBrowser *browser = manager->lookup<IgOIVBrowser> (name);
    

    if (browser == 0)
    {
	// FIXME: use a proper LFwebLego stream
	LOG (0, trace, lat::LFgeneral, "Creating Lego browser");

	// FIXME: UGLY hack to get the Lego view working... 
	browser = new IgOIVBrowser (state, 0, (Ig3DBaseModel *) -4);
	browser->setCamera ();    

	manager->add (browser, name);
    }    
}


void 
IgWebLegoService::browse (Arguments *arguments)
{
    using namespace lat;
    IgState *state = arguments->state ();
    QIODevice *outputDevice = arguments->outputDevice ();    
    ArgumentsMap &args = *arguments->args ();
    
    IgDocumentData *dd = IgDocumentData::get (state);
    IgBrowserManager *manager = IgBrowserManager::get (state);

    std::string rootName = "";
    if (args.find ("root") != args.end ())
	rootName = args["root"];
    
    IgTwig *twig = dynamic_cast <IgTwig *> (dd->root (rootName));
    
    std::string browserName = "";
    if (args.find ("browserName") != args.end ())
	browserName = args["browserName"];

    
    IgOIVBrowser *browser = manager->lookup<IgOIVBrowser> (browserName);
    ASSERT (browser);

    browser->browse (twig);
    browser->viewAll ();
    
    if (args.find ("position") != args.end ())
    {
	float x, y, z;	 
	StringList coords = StringOps::split (args["position"], ",");
	if (coords.size () == 3)
	{
	    x = strtof (coords[0].c_str (), 0);
	    y = strtof (coords[1].c_str (), 0);
	    z = strtof (coords[2].c_str (), 0);			
	    browser->getCamera ()->position = SbVec3f (x, y ,z);		    
	}
    }

    if (args.find ("orientation") != args.end ())
    {
	float x,y,z,a;		    
	StringList coords = StringOps::split (args["orientation"], ",");
	if (coords.size () == 4)
	{
	    x = strtof (coords[0].c_str (), 0);
	    y = strtof (coords[1].c_str (), 0);
	    z = strtof (coords[2].c_str (), 0);			
	    a = strtof (coords[3].c_str (), 0);			
	    browser->getCamera ()->orientation 
		= SbRotation (SbVec3f (x, y ,z), a);		    
	}
    }

    if (args.find ("pointAt") != args.end ())
    {
	float x,y,z;		    
	StringList coords = StringOps::split (args["pointAt"], ",");
	if (coords.size () == 3)
	{
	    x = strtof (coords[0].c_str (), 0);
	    y = strtof (coords[1].c_str (), 0);
	    z = strtof (coords[2].c_str (), 0);			
	    browser->getCamera ()->pointAt (SbVec3f (x, y ,z));		    
	}
    }
		

    if (args.find ("viewAll") != args.end ())
    {
	browser->viewAll ();		    
    }

    if (args.find ("moveCameraScreen") != args.end ())
    {
	float x, y;
	StringList coords 
	    = StringOps::split (args["moveCameraScreen"], ",");
	if (coords.size () == 2)
	{
	    x = strtof (coords[0].c_str (), 0);
	    y = strtof (coords[1].c_str (), 0);
	    browser->moveCameraScreen (SbVec2f (x, y));			
	}		    
    }

    if (args.find ("zoom") != args.end ())
    {
	float diffValue = 0;
	diffValue = strtof (args["zoom"].c_str (), 0);
	browser->zoom (diffValue);
    }

    if (args.find ("spin") != args.end ())
    {
	float x, y;
	StringList pos 
	    = StringOps::split (args["spin"], ",");
	if (pos.size () == 2)
	{
	    x = strtof (pos[0].c_str (), 0);
	    y = strtof (pos[1].c_str (), 0);
	    const SbVec2f posn (x, y);
	    // FIXME: all this should be done in some eventHandling function.
	    browser->addToLog (posn, SbTime::getTimeOfDay ());			
	    browser->spin (posn);			
	} 
    }
    
    if (args.find ("lastSpin") != args.end ())
    {
	float x, y;
	StringList pos 
	    = StringOps::split (args["lastSpin"], ",");
	if (pos.size () == 2)
	{
	    x = strtof (pos[0].c_str (), 0);
	    y = strtof (pos[1].c_str (), 0);
			
	    const SbVec2f posn (x, y);
	    // FIXME: all this should be done in some eventHandling function.
	    browser->addToLog (posn, SbTime::getTimeOfDay ());			
	    browser->spin (posn);			
	}		    
	browser->clearLog ();		    
    }
						
    QByteArray tmpArray = browser->getJPGBuffer ();
    outputDevice->writeBlock (tmpArray.data (), tmpArray.size ());
}

const char *
IgWebLegoService::catalogLabel (void)
{
    return "Lego";
}
