//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgUserGUIDriver/interface/IgGUITest.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppLoopService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppExtensions.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQMainWindowSite.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgArgsElement.h"
#include <qapplication.h>
#include <qmainwindow.h>
#include <qdockwindow.h>
#include <qpushbutton.h>
#include <qtextedit.h>

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
IgGUITest::catalogLabel (void)
{ return "Howto write an IGUANA driver"; }

IgGUITest::IgGUITest (IgState *state)
  : m_state (state)
{ 
    ASSERT (m_state);
}

IgGUITest::~IgGUITest (void)
{}

int
IgGUITest::run (void)
{    
    IgArgsElement *args = IgArgsElement::get (m_state);

    // Create a Qt application and main window
    QApplication qapp (args->args (), args->argv ());
    IgQMainWindowSite	*main = new IgQMainWindowSite (m_state, 0);
    QMainWindow *mainWindow = dynamic_cast<QMainWindow *> (IgQtSite::selfFrom (main));
    mainWindow->setCaption ("Howto write an IGUANA driver");

    IgQtAppExtensions::install (m_state, mainWindow);
    ASSERT (IgQtAppLoopService::get (m_state));
     
    QDockWindow *docWindow = new QDockWindow (QDockWindow::OutsideDock, 0, "PushButton Test");
    QPushButton *button = new QPushButton ("Test", docWindow);
    docWindow->setWidget (button);
    mainWindow->addDockWindow (docWindow);

    QTextEdit *edit = new QTextEdit (mainWindow, "editor");
    edit->setText ("This is an example of how to write a GUI as an IGUANA driver.\n "
		   "The Test push-button is a docking window. It doesn't do anything. "
		   "If you would like to add some functionality here, please, refer to the Qt user guide.");
    
    mainWindow->setCentralWidget (edit);
    mainWindow->resize (1000, 760);
    
    qapp.setMainWidget (mainWindow);
     
    // Run the application
    mainWindow->show ();
     
    // Run the main event loop
    IgQtAppLoopService::get (m_state)->run ();

    return EXIT_SUCCESS;
}


