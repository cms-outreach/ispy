//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgNTupleApp/interface/IgNTupleAppSetup.h"
#include "Ig_Modules/IgNTupleApp/interface/IgNTupleAppExtension.h"
#include "Ig_Modules/IgStudio/interface/IgStudioDriver.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppStatusBarService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgExtensionDB.h"
#include <qapplication.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgNTupleAppSetup, "Services/Studio/Session Types/Vis Example--NTuple browser; NTuple browser");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgNTupleAppSetup::IgNTupleAppSetup (IgState *state)
    : m_state (state),
      m_studio (0)
{ ASSERT (m_state); m_state->put (s_key, this); }


IgNTupleAppSetup::~IgNTupleAppSetup (void)
{ASSERT (m_state); m_state->detach (s_key);}

bool
IgNTupleAppSetup::setup (IgStudioDriver *into)
{
    ASSERT (m_state);
    ASSERT (into);
    ASSERT (!m_studio);

    m_studio = into;

    static const char		name [] = "Services/Generic/NTuple";
    IgNTupleAppExtension *	app = 0;
    IgExtension	*		newext = 0;

    try
    {
	// Try to instantiate it
	QApplication::setOverrideCursor (Qt::waitCursor);
	IgQtAppStatusBarService::get (m_state)
	    ->setMessage ("Building NTuple browser app...");

	newext = IgExtensionDB::get ()->create (name, m_state);

	// Make sure we got what we expect
	ASSERT (newext);
 	ASSERT (dynamic_cast<IgNTupleAppExtension *> (newext));
 	app = static_cast<IgNTupleAppExtension *> (newext);

	// Try to set it up
	app->setup (this);
       
        IgQtAppStatusBarService::get (m_state)
	    ->setMessage ("NTuple browser is loaded.");

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
IgNTupleAppSetup::state (void) const
{ return m_state; }

IgStudioDriver *
IgNTupleAppSetup::studio (void) const
{ return m_studio; }
