//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/interface/IgProfile.h"
#include "Ig_Modules/IgQtProfileService/interface/IgQtProfileService.h"
#include "Ig_Modules/IgStudio/interface/IgStudioDriver.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfMainWindow.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppStatusBarService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
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

IG_DEFINE_STATE_ELEMENT (IgProfile, "Services/Studio/Session Types/Profiler GUI");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgProfile::IgProfile (IgState *state)
    : m_state (state),
      m_studio (0)
{ ASSERT (m_state); }

IgProfile::~IgProfile (void)
{ ASSERT (m_state); }

bool
IgProfile::setup (IgStudioDriver *into)
{
	ASSERT (into);
	ASSERT (! m_studio);
	m_studio = into;
	
	// Main window from the studio
	QMainWindow *mainWindow = dynamic_cast<QMainWindow *> (m_studio->mainWindow ());
	ASSERT (mainWindow);
	
	QWidget *mainWidget = new QWidget(mainWindow);
	mainWindow->setCentralWidget(mainWidget);
	mainWidget->show();
	
	IgQtAppMenuService *menuBarService = new IgQtAppMenuService(m_state, mainWindow);
	
	//Create configuration and IgOProfMainWindow
	new IgQtProfileService(m_state, menuBarService->menuBar(), mainWidget,mainWindow->statusBar(),false);

	return true;
}

