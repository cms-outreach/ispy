//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSocketDriver/interface/IgSocketDriver.h"
#include "Ig_Modules/IgSocketDriver/interface/IgServerSocket.h"
#include "Ig_Modules/IgSocketDriver/interface/IgSession.h"
#include "Ig_Modules/IgSocketDriver/interface/IgWebServiceRegistry.h"
#include "Ig_Modules/IgSocketDriver/interface/IgSocketDriverInfo.h"
#include "Ig_Modules/IgSocketDriver/interface/IgServerPool.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppLoopService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgArgsElement.h"

#include <qapplication.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSocketDriver::IgSocketDriver (IgState *state)
    :m_state (state)
{
}

int
IgSocketDriver::run (void)
{
    IgArgsElement *args = IgArgsElement::get (m_state);
    

    QApplication *app = new QApplication (args->args (), 
					  args->argv ());

    IgQtAppLoopService *loopService = new IgQtAppLoopService (m_state);    
    IgSocketDriverInfo *info = new IgSocketDriverInfo (m_state);    
    new IgWebServiceRegistry (m_state);    

    std::vector <std::string> serverOpts;
    serverOpts.push_back ("--ig-server");
    if (args->exists (serverOpts))
	IgServerPool::getInstance (m_state);
    
    info->setSocket (new IgServerSocket (m_state, true));
    loopService->run ();	
    
    app->exec ();    
    return 0;    
}

const char *
IgSocketDriver::catalogLabel (void)
{
    return "SOCKET";    
}

