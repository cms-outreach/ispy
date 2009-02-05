//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DEnumControl.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
#include <Inventor/fields/SoSFEnum.h>
#include <classlib/utils/DebugAids.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DEnumControl::Ig3DEnumControl (IgControlCategory *pane, QString label)
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

    m_value = new QComboBox (area);
    m_value->setEditable(false);
    addGridWidget (makeSpacedBox (area, m_value), row, 1);

    // Always reflect the state of the real value control and vice versa.
    connect (m_value,            SIGNAL (activated    (int)), 
             this,               SLOT   (valueChanged (int)));
    connect (this,               SIGNAL (editable   (bool)), 
             m_value, 		 SLOT   (setEnabled (bool)));
    connect (this,               SIGNAL (editable   (bool)), 
             vlab,               SLOT   (setEnabled (bool)));
}

//////////////////////////////////////////////////////////////////////
SoSFEnum *
Ig3DEnumControl::field (void) const
{ return static_cast<SoSFEnum *> (Ig3DFieldControl::field ()); }

void
Ig3DEnumControl::setField (SoSFEnum *field)
{
    Ig3DFieldControl::setField (field);
    if (field)
    {
      int nums = field->getNumEnums ();
      if (nums != m_enums.size ())
      {
        m_enums.clear ();
	m_value->clear ();
	SbName name;
	for(int i=0; i < nums; i++)
	{
	  m_enums.push_back (field->getEnum(i, name));
	  m_value->insertItem (name.getString ());
	}
      }
    }
}

int
Ig3DEnumControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValue (); }

//////////////////////////////////////////////////////////////////////
int
Ig3DEnumControl::value (void) const
{ return m_enums[m_value->currentItem ()]; }

void
Ig3DEnumControl::setValue (int newval)
{
    int index = findIndex (newval);
    ASSERT (index != -1);
    
    m_value->setCurrentItem (index);
    setDirty (true);
}

void
Ig3DEnumControl::updateValue (void)
{ setValue (fieldValue ()); }

void
Ig3DEnumControl::applyValue (void)
{ field ()->setValue (value ()); }

//////////////////////////////////////////////////////////////////////
int
Ig3DEnumControl::findIndex (int value)
{
    int index = -1;
    for (unsigned int i = 0; i < m_enums.size (); i++)
    {
      if (m_enums[i] == value) { index = i ; break; }
    }    
    return index;
}

void
Ig3DEnumControl::addValidValue (int value, const QString &name)
{
    int index = findIndex (value);
    ASSERT (index == -1);
    
    m_value->insertItem (name);
    m_enums.push_back (value);
}

void
Ig3DEnumControl::valueChanged (int index)
{
    if (fieldValue () != m_enums[index])
      setDirty (true);
}
//////////////////////////////////////////////////////////////////////
QWidget *
Ig3DEnumControl::widget () const
{ return m_value; }
