//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSoViewerAppDriver/interface/IgSoViewerAppDriver.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgArgsElement.h"
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/SoInput.h>
#include <qwidget.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/iobase/Filename.h>
#include <iostream>
#include <cstdlib>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

const char *
IgSoViewerAppDriver::catalogLabel (void)
{ return "SoViewer"; }

IgSoViewerAppDriver::IgSoViewerAppDriver (IgState *state)
    : m_state (state)
{
    ASSERT (state);
}

int
IgSoViewerAppDriver::run (void)
{
    ASSERT (IgArgsElement::get (m_state));

    IgArgsElement *args = IgArgsElement::get (m_state);
    if (args->args () != 2)
    {
	std::cerr << "usage: " << args->arg (0) << " SoViewer FILE\n";
	return 1;
    }

    if (! lat::Filename (args->arg (1)).isReadable ())
    {
	std::cerr << args->arg (1) << ": cannot access\n";
	return 1;
    }

    // Create a SoQt application window to show the file.
    // QApplication qapp (m_argc, m_argv);
    // qapp.setStyle (new QWindowsStyle);
    // IgQtSheetManager *manager = new IgQtSheetManager;
    // manager->setGeometry (0, 0, 1050, 810);
    // manager->setCaption  ("IGUANA SoViewer");
    // qapp.setMainWidget (manager);
    QWidget *window = SoQt::init (args->args (), args->argv (),
				  "IGUANA SoViewer","SoViewer");
    SoNode  *node = 0;
    SoInput file;

    if (! file.openFile (args->arg (1)) || ! SoDB::read (&file, node))
    {
	std::cerr << "SoViewer: " << args->arg (1)
		  << ": error: could not read\n";
	return 1;
    }

    // Show the tree
    SoQtExaminerViewer *viewer = new SoQtExaminerViewer (window);
    viewer->setSceneGraph (node);
    viewer->show ();

    // Run the application
    window->setCaption ("IGUANA SoViewer");
    SoQt::show (window);
    SoQt::mainLoop ();
    
    return EXIT_SUCCESS;
}
