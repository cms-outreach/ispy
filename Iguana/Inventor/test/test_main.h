//<<<<<< INCLUDES                                                       >>>>>>

#include <Inventor/SoOutput.h> 
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
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

int main (int argc, char **argv)
{
    lat::Signal::handleFatal (argv [0]);

    QApplication app (argc, argv);
    QWidget * mainwin = SoQt::init (argc, argv, argv[0]);

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

    SoQtExaminerViewer *viewer = new SoQtExaminerViewer (mainwin);
    viewer->setSceneGraph (top);
    viewer->setFeedbackVisibility (true);
    viewer->show();

    mainwin->setWindowTitle (title);
    mainwin->setGeometry (0, 0, 800, 600);

    // Pop up the QuarterWidget
    mainwin->show();

    QTimer::singleShot (10 * 60 * 1000, qApp, SLOT(quit()));

    SoQt::show(mainwin);
    SoQt::mainLoop();
  
    delete viewer;

    // Clean up resources.
    top->unref();
    
    return 0;
}
