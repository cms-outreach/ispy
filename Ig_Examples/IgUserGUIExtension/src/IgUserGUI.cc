//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgUserGUIExtension/src/IgUserGUI.h"
#include "Ig_Modules/IgStudio/interface/IgStudioDriver.h"
#include <qapplication.h>
#include <qmainwindow.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qdockwindow.h>
#include <qtextedit.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgUserGUI, "Services/Studio/Session Types/Vis Example--IGUANA Extension");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgUserGUI::IgUserGUI (IgState *state)
    : m_state (state),
      m_studio (0)
{ ASSERT (m_state); }

IgUserGUI::~IgUserGUI (void)
{ ASSERT (m_state); }

bool
IgUserGUI::setup (IgStudioDriver *into)
{
    ASSERT (into);
    ASSERT (! m_studio);
    m_studio = into;

    // Main window from the studio.
    QMainWindow *mainWindow = dynamic_cast<QMainWindow *> (m_studio->mainWindow ());
    ASSERT (mainWindow);

    // Change window caption.
    mainWindow->setCaption ("Howto write an IGUANA Extension");

    // Add a dock window with a test pushbutton.      
    QDockWindow *docWindow = new QDockWindow (QDockWindow::OutsideDock, 0, "PushButton Test");
    QPushButton *button = new QPushButton ("Test", docWindow);
    docWindow->setWidget (button);
    mainWindow->addDockWindow (docWindow);

    // Add a text editor.
    QTextEdit *edit = new QTextEdit (mainWindow, "editor");
    edit->setText ("This is an example of how to write a GUI as an IGUANA extension.\n "
		   "The Test push-button is a docking window. It doesn't do anything. "
		   "If you would like to add some functionality here, please, refer to the Qt user guide.");  
    edit->show ();

    mainWindow->setCentralWidget (edit);
   
    return true;
}
