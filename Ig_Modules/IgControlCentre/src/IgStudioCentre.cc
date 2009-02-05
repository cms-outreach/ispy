//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgStudioCentre.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCentre.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include <qpopupmenu.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Callback.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgStudioCentre, "Extensions/Qt GUI/Control Centre");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Instantiate control centre into a state (normally studio).  */
IgStudioCentre::IgStudioCentre (IgState *state)
    : m_state (state)
{
    ASSERT (m_state);
    m_state->put (s_key, this);
    
    m_centre = new IgControlCentre (0);

    IgQtAppActiveService::get (m_state)
      ->add (lat::CreateCallback (this, &IgStudioCentre::appActive));

    ASSERT (IgQtAppMenuService::get (m_state));
    QPopupMenu *file = IgQtAppMenuService::get (m_state)->subMenu
	(IgQtAppMenuService::MENU_ID_FILE);
    file->insertItem("&Control centre", m_centre, SLOT(show()), 0,
	 MENU_ID_CONTROL_CENTRE, IgQtAppMenuService::index
	 (file, MENU_ID_CONTROL_CENTRE));
    file->insertSeparator(file->indexOf(MENU_ID_CONTROL_CENTRE)+1);
}

IgControlCentre*
IgStudioCentre::controlCentre (void)
{ return m_centre; }

void
IgStudioCentre::appActive (IgQtAppActiveMessage message)
{
    if(message.active ())
      m_centre->updateOnWindowActive ();
}
