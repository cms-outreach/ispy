//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DStringControl.h"
#include "Ig_Extensions/IgQt/interface/IgQtLineEdit.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
#include <classlib/utils/DebugAids.h>
#include <qlineedit.h>
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

Ig3DStringControl::Ig3DStringControl (IgControlCategory *pane, QString label)
    : Ig3DFieldControl (pane)
{
    ASSERT (pane);
    ASSERT (pane->bodyArea ());
    ASSERT (pane->bodyLayout ());

    QWidget	*area = pane->bodyArea ();
    int		row = gridRows ();

    ASSERT (row >= 0);

    // Add a new row to the grid, label on the left and the value on the right
    QWidget 	*vlab = makeIndentedLabel (label, area);
    addGridWidget (vlab, row, 0);

    // Add a new row to the grid, value on the left and the label on the right
    m_value = new IgQtLineEdit (area);
    addGridWidget (makeSpacedBox (area, m_value), row, 1);

    connect (m_value, SIGNAL (valueChanged (const QString&)),
	     this,   SLOT (textChanged (const QString&)));
    connect (this,    SIGNAL (editable   (bool)), 
             m_value, SLOT   (setEnabled (bool)));
    connect (this,      SIGNAL (editable    (bool)), 
             vlab,      SLOT   (setEnabled  (bool)));
}

//////////////////////////////////////////////////////////////////////
SoSFString *
Ig3DStringControl::field (void) const
{ return static_cast<SoSFString *> (Ig3DFieldControl::field ()); }

void
Ig3DStringControl::setField (SoSFString *field)
{ Ig3DFieldControl::setField (field); }

SbString
Ig3DStringControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValue (); }

IgQtLineEdit*
Ig3DStringControl::widget (void) const
{ return m_value; }

//////////////////////////////////////////////////////////////////////
SbString
Ig3DStringControl::value (void) const
{ return SbString (m_value->text ().latin1 ()); }

void
Ig3DStringControl::setValue (const SbString& newval)
{ m_value->setText (newval.getString ()); setDirty (); }

void
Ig3DStringControl::textChanged (const QString& /*newval*/)
{ setDirty (); }

void
Ig3DStringControl::updateValue (void)
{ setValue (fieldValue ()); }

void
Ig3DStringControl::applyValue (void)
{ field ()->setValue (value ()); }
