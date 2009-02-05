//<<<<<< INCLUDES                                                       >>>>>>
#include "Ig_Modules/IgQtPython/interface/IgQtPythonLoadMenu.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppWindowService.h"
#include "Ig_Modules/IgPython/interface/IgPythonService.h"
#include "Ig_Modules/IgPython/interface/IgPythonInterpreter.h"
#include "Ig_Modules/IgPython/interface/IgPythonStream.h"
#include <qstring.h>
#include <qfiledialog.h>

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

IgQtPythonLoadMenu::IgQtPythonLoadMenu (IgState *state) 
    :m_state (state)
{
}

void 
IgQtPythonLoadMenu::slotPythonLoader (void)
{
    IgQtAppWindowService *ws = IgQtAppWindowService::get (m_state);    
    QString filename = QFileDialog::getOpenFileName (".", 
						     "*.py", 
						     ws->mainWindow ()); 
    IgPythonService *ps = IgPythonService::get (m_state);
    if (!filename.isEmpty ())
    {
	ps->interpreter ()->execute (filename.ascii ());
	*(ps->pythonStream ()) << std::flush;	
    }    
}
