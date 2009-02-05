//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DMBoolControl.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
#include <classlib/utils/DebugAids.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qwhatsthis.h>
#include <qcombobox.h>
#include <qcheckbox.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DMBoolControl::Ig3DMBoolControl (IgControlCategory *pane,
				    QString label)
    : Ig3DFieldControl (pane),
      m_value  (0),
      m_indexs (0),
      m_values (1, SbBool (""))
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
    QWhatsThis::add (vlab, "Bool values");

    m_value = new QWidget(area);
    QHBoxLayout *l = new QHBoxLayout (m_value, 0, 5);
    l->addItem ( new QSpacerItem( 50, 20, QSizePolicy::Expanding,
                                          QSizePolicy::Minimum ));
    
    m_indexs = new QComboBox (m_value);
    l->addWidget (m_indexs);
    m_indexs->insertItem ("0");
    m_indexs->setCurrentItem (0);
    m_indexs->hide ();
    QWhatsThis::add (m_indexs, "Select the index");
    l->addItem ( new QSpacerItem( 50, 20, QSizePolicy::Expanding,
                                          QSizePolicy::Minimum ));

    m_checkBox = new QCheckBox ("", area);
    QWhatsThis::add (m_checkBox, "Bool values");
    l->addWidget (m_checkBox);
        
    addGridWidget (makeSpacedBox (area, m_value), row, 1);

    // Link the button press signal to popping up a color dialog
    connect (m_indexs,	SIGNAL (activated   (int)),
    	     this, 	SLOT   (newIndex    (int)));
    connect (m_checkBox, SIGNAL (toggled    (bool)), 
             this,    SLOT   (valueChanged(bool)));
    connect (this,      SIGNAL (editable    (bool)), 
             m_value,   SLOT   (setEnabled  (bool)));
    connect (this,      SIGNAL (editable    (bool)), 
             vlab,      SLOT   (setEnabled  (bool)));
}

//////////////////////////////////////////////////////////////////////
SoMFBool *
Ig3DMBoolControl::field (void) const
{ return static_cast<SoMFBool *> (Ig3DFieldControl::field ()); }

void
Ig3DMBoolControl::setField (SoMFBool *field)
{ Ig3DFieldControl::setField (field); }

const SbBool*
Ig3DMBoolControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValues (0); }

QWidget*
Ig3DMBoolControl::widget(void) const
{ return m_value; }

//////////////////////////////////////////////////////////////////////
const SbBool*
Ig3DMBoolControl::value (void) const
{ return &(m_values[0]); }

void
Ig3DMBoolControl::resize (unsigned int size)
{
  ASSERT (size);
  int sizeDiff = size - m_values.size ();
  if (sizeDiff == 0) return;

  m_values.resize  (size);
  if (sizeDiff > 0)
  {
    for (unsigned int i = size - sizeDiff; i < size; i++)
      m_indexs->insertItem (QString::number (i));
  }
  else {
    for (unsigned int i = size - sizeDiff - 1 ; i >= size; i--)
      m_indexs->removeItem (i);
  }

  if (size > 1)
    m_indexs->show ();
  else
    m_indexs->hide ();
}

void
Ig3DMBoolControl::setValue (unsigned int size, const SbBool* newval)
{
  int index = m_indexs->currentItem ();
  if (index < 0)
    index = 0;
  
  resize (size);
  for (unsigned int i = 0; i < size; i++)
    m_values [i] = newval [i];

  m_indexs->setCurrentItem (index);
  m_checkBox->setChecked (newval [index]);
  setDirty ();
}

void
Ig3DMBoolControl::valueChanged (bool newval)
{
    m_values[m_indexs->currentItem ()] = newval;
    setDirty ();
}

void
Ig3DMBoolControl::updateValue (void)
{ setValue (static_cast<SoMFBool *>(field())->getNum (), fieldValue ()); }

void
Ig3DMBoolControl::applyValue (void)
{ field ()->setValues (0, int(m_values.size ()), value ()); }

void
Ig3DMBoolControl::newIndex (int index)
{  m_checkBox->setChecked (m_values[index]); }
