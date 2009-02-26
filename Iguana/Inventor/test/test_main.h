//<<<<<< INCLUDES                                                       >>>>>>

#include <Inventor/SoOutput.h> 
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Quarter/Quarter.h>
#include <Quarter/QuarterWidget.h>
#include <QApplication>
#include <QTimer>
#include <QMainWindow>
#include <classlib/utils/Signal.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#ifdef WIN32
#define main IgOpenInventor_main
#endif

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

QString makeTest (SoSeparator *root);

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace SIM::Coin3D::Quarter;

int main (int argc, char **argv)
{
    lat::Signal::handleFatal (argv [0]);

    QApplication app (argc, argv);
    Quarter::init(true);

    SoSelection		*top = new SoSelection;
    SoSeparator		*root = new SoSeparator;

    top->policy = SoSelection::SINGLE;
    top->addChild (root);
    top->ref();

    SoMaterial *redPlastic = new SoMaterial;
    redPlastic->emissiveColor.setValue (1, 0, 0);
    redPlastic->diffuseColor.setValue (1, 0, 0);
    redPlastic->transparency = .5;
    root->addChild (redPlastic);
    
    QString title = makeTest (root);
    
    if (getenv ("IGUANA_DEBUG_SCENE"))
    {
	SoOutput out;    
	SoWriteAction wa (&out);
	wa.apply (root);
    }

    QMainWindow * mainwin = new QMainWindow();

    // Create a QuarterWidget for displaying a Coin scene graph
    QuarterWidget *viewer = new QuarterWidget (mainwin);
    viewer->setSceneGraph (top);
    viewer->setNavigationModeFile ();
    viewer->viewAll ();

    mainwin->setWindowTitle (title);
    mainwin->setCentralWidget(viewer);
    mainwin->setGeometry (0, 0, 800, 600);

    // Pop up the QuarterWidget
    mainwin->show();

    QTimer::singleShot (10 * 60 * 1000, qApp, SLOT(quit()));

    // Loop until exit.
    app.exec();

    // Clean up resources.
    top->unref();
    
    // FIXME: Deleting the QuarterWidget crashes X11
    // and not deleting it leaves some resources uncleaned?
    // 
    // delete viewer;

    Quarter::clean();

    return 0;
}
