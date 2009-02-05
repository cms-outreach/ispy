//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControl.h"
#include <qframe.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtRangeMControl::IgQtRangeMControl (QWidget *parent, int parts,
				      unsigned int size,
		      		      const char *name)
    : IgQtRangeControlCommon (parent, parts, name)
{ 
    QHBoxLayout *layout = new QHBoxLayout (m_widget, 2, 5);

    // Add the listbox for index
    m_index = new QComboBox (m_widget);
    layout->addWidget (m_index);
    m_index->hide ();
    if (parts & IndexPart)
    {
      m_indexPart = true;
      connect (m_index, SIGNAL (activated (int)), this, SLOT (newIndex (int)));
    }
    else
      m_indexPart = false;

    m_layout = new QVBoxLayout (layout, 0, 0);
    IgQtRangeControlCommon::initialize (parts);
    resizeIndex (size);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeMControl::resizeIndex (unsigned int size)
{
    int countDiff = size - m_index->count ();
    if (countDiff == 0) return;
    
    if (countDiff > 0)
    {
      for (unsigned int i = size - countDiff; i < size; i++)
        m_index->insertItem (QString::number (i));
    }
    else {
      for (unsigned int i = size - countDiff - 1 ; i >= size; i--)
        m_index->removeItem (i);
    }
    if (size)
      m_index->setCurrentItem(0);

    if (m_indexPart)
    {
      if(size > 1)
        m_index->show ();
      else
        m_index->hide ();
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeMControl::newIndex (int index)
{
  ASSERT (index < m_index->count () && index >= 0);
  m_index->setCurrentItem(index);
  updateTextValue ();
}

void
IgQtRangeMControl::updateTextValue (void)
{ }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int
IgQtRangeMControl::currentIndex (void) const
{ return m_index->currentItem (); }
