//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DMStringControl.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCentre.h"
#include "Ig_Modules/IgControlCentre/interface/IgStudioCentre.h"
#include "Ig_Extensions/IgQt/interface/IgQtLineEdit.h"
#include <classlib/utils/DebugAids.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qwhatsthis.h>
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

Ig3DMStringControl::Ig3DMStringControl (IgControlCategory *pane,
				        QString label)
    : Ig3DFieldControl (pane),
      m_value  (0),
      m_indexs (0),
      m_strings (1, SbString (""))
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
    QWhatsThis::add (vlab, "String values");

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

    m_lineEdit = new IgQtLineEdit (m_value);
    QWhatsThis::add (m_lineEdit, "String values");
    l->addWidget (m_lineEdit);
        
    addGridWidget (makeSpacedBox (area, m_value), row, 1);

    // Link the button press signal to popping up a color dialog
    connect (m_indexs,	SIGNAL (activated   (int)),
    	     this, 	SLOT   (newIndex    (int)));
    connect (m_lineEdit, SIGNAL (valueChanged (const QString&)),
	     this,   SLOT (textChanged (const QString&)));
    connect (this,      SIGNAL (editable    (bool)), 
             m_value,   SLOT   (setEnabled  (bool)));
    connect (this,      SIGNAL (editable    (bool)), 
             vlab,      SLOT   (setEnabled  (bool)));
}

//////////////////////////////////////////////////////////////////////
SoMFString *
Ig3DMStringControl::field (void) const
{ return static_cast<SoMFString *> (Ig3DFieldControl::field ()); }

void
Ig3DMStringControl::setField (SoMFString *field)
{ Ig3DFieldControl::setField (field); }

const SbString*
Ig3DMStringControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValues (0); }

QWidget*
Ig3DMStringControl::widget(void) const
{ return m_value; }

//////////////////////////////////////////////////////////////////////
const SbString*
Ig3DMStringControl::value (void) const
{ return &(m_strings[0]); }

void
Ig3DMStringControl::resize (unsigned int size)
{
  ASSERT (size);
  int sizeDiff = size - m_strings.size ();
  if (sizeDiff == 0) return;

  m_strings.resize  (size);
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
Ig3DMStringControl::setValue (unsigned int size, const SbString* newval)
{
  int index = m_indexs->currentItem ();
  if (index < 0)
    index = 0;
  
  resize (size);
  for (unsigned int i = 0; i < size; i++)
    m_strings [i] = newval [i];

  m_indexs->setCurrentItem (index);
  m_lineEdit->setText (newval [index].getString ());
  setDirty ();
}

void
Ig3DMStringControl::textChanged (const QString& newval)
{
    m_strings[m_indexs->currentItem ()] = newval.latin1 ();
    setDirty ();
}

void
Ig3DMStringControl::updateValue (void)
{ setValue (static_cast<SoMFString *>(field())->getNum (), fieldValue ()); }

void
Ig3DMStringControl::applyValue (void)
{ field ()->setValues (0, int(m_strings.size ()), value ()); }

void
Ig3DMStringControl::newIndex (int index)
{  m_lineEdit->setText (m_strings[index].getString ()); }
