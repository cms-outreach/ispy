//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DBoolControl.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
#include <classlib/utils/DebugAids.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qlabel.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DBoolControl::Ig3DBoolControl (IgControlCategory *pane, QString label)
    : Ig3DFieldControl (pane)
{
    ASSERT (pane);
    ASSERT (pane->bodyArea ());
    ASSERT (pane->bodyLayout ());

    QWidget	*area = pane->bodyArea ();
    int		row = gridRows ();

    ASSERT (row >= 0);

    // Add a new row to the grid, value on the left and the label on the right
    m_value = new QCheckBox (label, area);
    addGridWidget (makeBox (area, m_value), row, 0);

    // Link the check box toggle signal to my state
    connect (m_value, SIGNAL (toggled    (bool)), 
             this,    SLOT   (setValue   (bool)));
    connect (this,    SIGNAL (editable   (bool)), 
             m_value, SLOT   (setEnabled (bool)));
}

//////////////////////////////////////////////////////////////////////
SoSFBool *
Ig3DBoolControl::field (void) const
{ return static_cast<SoSFBool *> (Ig3DFieldControl::field ()); }

void
Ig3DBoolControl::setField (SoSFBool *field)
{ Ig3DFieldControl::setField (field); }

bool
Ig3DBoolControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValue (); }

QCheckBox*
Ig3DBoolControl::widget (void) const
{ return m_value; }

//////////////////////////////////////////////////////////////////////
bool
Ig3DBoolControl::value (void) const
{ return m_value->isChecked (); }

void
Ig3DBoolControl::setValue (bool newval)
{ m_value->setChecked (newval); setDirty (); }

void
Ig3DBoolControl::updateValue (void)
{ setValue (fieldValue ()); }

void
Ig3DBoolControl::applyValue (void)
{ field ()->setValue (value ()); }
