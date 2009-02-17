//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/config.h"
#include <QWidget>
#include <Inventor/SoOutput.h> 
#include <Inventor/SoPickedPoint.h> 
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoLineHighlightRenderAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/Qt/SoQt.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Signal.h>
#include <QApplication>
#include <QTimer>
#include <QWidget>
#include <cstdlib>
#include <cmath>

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

    QWidget		*mainWindow = SoQt::init (argc, argv, "IGUANA Shape Test");
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

    mainWindow->setGeometry (0, 0, 800, 600); // (0, 0, 400, 300);
    SoQtExaminerViewer *viewer = new SoQtExaminerViewer (mainWindow);
    // viewer->setGeometry (0, 0, 800, 600);
    viewer->setTitle (title.toLatin1());
    viewer->setGLRenderAction (new SoLineHighlightRenderAction);
    viewer->redrawOnSelectionChange (top);
    viewer->setSceneGraph (top);
    viewer->viewAll ();
    viewer->show ();
    SoQt::show (mainWindow);
    mainWindow->show ();

    QTimer::singleShot (10 * 60 * 1000, qApp, SLOT(quit()));
    app.exec ();
    SoQt::mainLoop ();
    top->unref ();
    delete viewer;

    return 0;
}
