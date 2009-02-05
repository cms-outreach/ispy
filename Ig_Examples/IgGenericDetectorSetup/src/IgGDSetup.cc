//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgGenericDetectorSetup/interface/IgGDSetup.h"
#include "Ig_Examples/IgGenericDetectorSetup/interface/IgGDMainExtension.h"
#include "Ig_Modules/IgStudio/interface/IgStudioDriver.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppStatusBarService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgExtensionDB.h"
#include <qapplication.h>
#include <qmessagebox.h>
#include <cstdlib>
#undef emit
#include <classlib/utils/DebugAids.h>
#include <cstdlib>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgGDSetup, "Services/Studio/Session Types/Vis Example--Generic Detector; Generic Detector");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgGDSetup::IgGDSetup (IgState *state)
    : m_state (state),
      m_studio (0)
{ ASSERT (m_state); m_state->put (s_key, this); }

IgGDSetup::~IgGDSetup (void)
{ ASSERT (m_state); m_state->detach (s_key); }

bool
IgGDSetup::setup (IgStudioDriver *into)
{
    ASSERT (m_state);
    ASSERT (into);
    ASSERT (!m_studio);

    m_studio = into;

    // OK to load the actual Generic Detector Visualisation plug-in.
    static const char		name [] = "Services/Generic/Detector";
    IgGDMainExtension           *app = 0;
    IgExtension			*newext = 0;

    try
    {
	// Try to instantiate it
	QApplication::setOverrideCursor (Qt::waitCursor);
	IgQtAppStatusBarService::get (m_state)
	    ->setMessage ("Building Generic Detector...");

	newext = IgExtensionDB::get ()->create (name, m_state);

	// Make sure we got what we expect
	ASSERT (newext);
 	ASSERT (dynamic_cast<IgGDMainExtension *> (newext));
 	app = static_cast<IgGDMainExtension *> (newext);

	// Try to set it up
	app->setup (this);
       
        IgQtAppStatusBarService::get (m_state)
	    ->setMessage ("Generic Detector is loaded.");

	QApplication::restoreOverrideCursor ();
    }
    // FIXME: too broad...
    catch (...)
    {
	QApplication::restoreOverrideCursor ();
	return EXIT_FAILURE;
    }

    return true;
}

IgState *
IgGDSetup::state (void) const
{ return m_state; }

IgStudioDriver *
IgGDSetup::studio (void) const
{ return m_studio; }
