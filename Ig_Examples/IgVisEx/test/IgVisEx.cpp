#include "qapplication.h"
#include "qworkspace.h"
#include <Inventor/Qt/SoQt.h>
#include <Inventor/SoDB.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/lock/SoLockMgr.h> // for licensing 


#include "Ig_Modules/IgVis/interface/IgQtMDI.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIBaseShell.h"
#include "Ig_Modules/IgVis/interface/IgQtResourceMgr.h"
#include "Ig_Modules/IgVis/interface/IgQtTwig.h"

#include "Ig_Modules/IgVis/interface/IgQtMDIShellBuilder.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIShellFactory.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIBaseShell.h"
#include "Ig_Examples/IgVisEx/interface/IgQtMDITestShell.h"
#include "Ig_Examples/IgVisEx/interface/IgQtMDITest2DShell.h"
#include "Ig_Examples/IgVisEx/interface/IgQtPickTestShell.h"
#include "Ig_Examples/IgVisEx/interface/IgDemoExperiment.h"
#include "Ig_Examples/IgVisEx/interface/IgDemoExperiment2D.h"

#include "Ig_Extensions/IgOpenInventor/interface/IgSoRotSolid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoJet.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSimpleTrajectory.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoIdealTrack.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgParticleChar.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoPcon.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoShapeKit.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCoordinateAxis.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoAxis.h"

int
main(
     int argc,
     char ** argv )
{

    QApplication app(argc, argv);
    // Get a top-level MDI window
    IgQtMDI *mainW = new IgQtMDI(0,"Iguana");
    mainW->setCaption("Iguana");
    app.setMainWidget(mainW);
    SoQt::init(mainW);
    IgQtTwig::setStatusBar(mainW->statusBar());

    // connect the quit signal from the MDI to the application's quit
    QObject::connect(mainW,SIGNAL(quit()),&app,SLOT(quit()));

#if 0 // may use this call if we have a valid license string
    SoLockManager::SetUnlockString("License 3D-MasterSuite  3.5 31-Oct-2000 0 9sjefaa4bba3 \"FERMIP0524737\"");
#endif // license

    // initialise custom Inventor objects
    IgSoShapeKit::initClass();
    IgSoRotSolid::initClass();
    IgSoJet::initClass();
    IgSoSimpleTrajectory::initClass();
    IgSoAxis::initClass ();
    IgSoCoordinateAxis::initClass();
    IgSoPcon::initClass();
    // since IgSoIdealTrack assumes a magnetic field, it needs to
    // be initialized. Use IgSoSplineTrack if the field is unknown
    // but the track has multiple measure points (x,p)
    IgParticleChar::initParticles();    // Normally precedes initialisation of: IgSoIdealTrack
    IgSoIdealTrack::initClass();
    IgSoIdealTrack::setBField(4.F); // in T
    IgSoIdealTrack::setZMax(3.F); //+/- in m
    IgSoIdealTrack::setRMax(1.F);
    IgQtResourceMgr &mgr = IgQtResourceMgr::getInstance();
    if ( mgr.isValid() ) 
    {
	// have to do it in this order, even if it causes flashing since the main window show will reset any
	// maximized window in the workspace
	mainW->show();  
	mainW->restoreResources();
    }
    else
    {
 	IgDemoExperiment *gWindow = new IgDemoExperiment(mainW);
	mainW->show();
	gWindow->showMaximized();
	gWindow->viewAll();
    }

    // start the user input event loop
    SoQt::mainLoop();
    return 0;
}

