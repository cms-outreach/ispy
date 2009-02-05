//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DSystem/interface/Ig3DSystem.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppLoopService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgPluginLoader.h"
#include <Inventor/Qt/SoQt.h>
#include <functional>
#include <algorithm>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

bool				Ig3DSystem::s_initialised = false;
std::list<lat::Callback>	Ig3DSystem::s_actions;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
Ig3DSystem::doinit (IgState *state)
{
    // Override the event loop with SoQt's special filters
    ASSERT (IgQtAppLoopService::get (state));
    IgQtAppLoopService::get(state)->override(lat::CreateCallback(&SoQt::mainLoop));

    // Run all initialisers already registered
    std::for_each (s_actions.begin (), s_actions.end (),
		   std::mem_fun_ref (&lat::Callback::operator ()));

    // Autoload OpenInventor nodekit extensions.  Their
    // initialisation is global so put them into the global
    // (plug-in loader) state.
    ASSERT (IgPluginLoader::get (state));
    IgPluginLoader::get (state)->load ("Services/Inventor/NodeKits/",true);
}

void
Ig3DSystem::init (IgState *state, QWidget *mainWindow)
{
    // We need to initialise the 3D subsystem properly before using
    // it.  This function is that magic initialiser hook.  It is safe
    // to invoke this many times, only the first time around counts.

    if (s_initialised)
	return;

    s_initialised = true;

    // Initialise SoQt
    SoQt::init (mainWindow);
    doinit (state);
}

QWidget *
Ig3DSystem::init (IgState *state, int argc, char **argv,
		  const char *appname, const char *classname)
{
    // We need to initialise the 3D subsystem properly before using
    // it.  This function is that magic initialiser hook.  It is safe
    // to invoke this many times, only the first time around counts.
    ASSERT (! s_initialised);

    s_initialised = true;

    // Initialise SoQt
    QWidget *w = SoQt::init (argc, argv, appname, classname);
    doinit (state);
    return w;
}

void
Ig3DSystem::initAction (lat::Callback action)
{
    if (! s_initialised)
	s_actions.push_back (action);
    else
	action ();
}
