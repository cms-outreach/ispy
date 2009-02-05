//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DVectorBaseControl.h"
#include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"
#include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlFloat.h"
#include "Ig_Modules/IgControlCentre/interface/IgQtRangeSetting.h"
#include "Ig_Modules/Ig3DControls/interface/IgQtAxisAlignment.h"
#include "Ig_Modules/IgControlCentre/pixmaps/nx_rot.xpm"
#include "Ig_Modules/IgControlCentre/pixmaps/px_rot.xpm"
#include "Ig_Modules/IgControlCentre/pixmaps/ny_rot.xpm"
#include "Ig_Modules/IgControlCentre/pixmaps/py_rot.xpm"
#include "Ig_Modules/IgControlCentre/pixmaps/nz_rot.xpm"
#include "Ig_Modules/IgControlCentre/pixmaps/pz_rot.xpm"
#include <classlib/utils/DebugAids.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DVectorBaseControl::Ig3DVectorBaseControl (IgControlCategory *pane,
				      	      QString		label,
				      	      Style		style,
					      unsigned int	size,
					      const QString     &colSepLabels)
    : Ig3DFieldControl (pane),
      m_controls (0),
      m_alignment (0),
      m_offset (3)
{
    ASSERT (pane);
    ASSERT (pane->bodyArea ());
    ASSERT (pane->bodyLayout ());
    ASSERT (size);
    
    std::vector<QString> labels;
    QString tmpLabels (colSepLabels);
    for(unsigned int i = 0; i < size; i++)
      labels.push_back (IgQtRangeControlCommon::getSettingItem (tmpLabels));
    
    QWidget	*area = pane->bodyArea ();
    int		parts = AllParts;
    int		ctlparts = IgQtRangeControlFloat::AllParts;

    // Add label to the grid
    QWidget 	*vlab = makeIndentedLabel (label, area);
    addGridWidget (vlab, gridRows (), 0);
    QString tips ("Set the \"");
    tips += label + "\" of the item";
    QWhatsThis::add (vlab, tips.latin1 ());

    // Decide which bits should be added and with what style
    switch (style)
    {
    case DirectionStyle:
    case PositionStyle:
    case PositionWOAStyle:
        ASSERT (size >= m_offset);
        if (labels[0].isEmpty ()) labels[0] = "X Axis";
        if (labels[1].isEmpty ()) labels[1] = "Y Axis";
        if (labels[2].isEmpty ()) labels[2] = "Z Axis";
	parts &= ~(RotatorPart | RotatorButtonPart);
	if (style == PositionWOAStyle)
	  parts &= ~AlignmentPart;
	break;

    case RotationStyle:
        ASSERT (size >= m_offset);
        if (labels[0].isEmpty ()) labels[0] = "Rotate X";
        if (labels[1].isEmpty ()) labels[1] = "Rotate Y";
        if (labels[2].isEmpty ()) labels[2] = "Rotate Z";
	parts &= ~AlignmentPart;
	ctlparts &= ~(IgQtRangeControlFloat::ValuePart
		      | IgQtRangeControlFloat::SliderPart);

	break;
    case Vector:
        parts &= ~AlignmentPart;
	m_offset = 0;
	break;
    }

    for(unsigned int i = m_offset; i < size; i++)
      if (labels[i].isEmpty ()) labels[i] = QString("Index ")
	                                    + QString::number(i-m_offset);
    
    pane->pushIndent ();

    if (m_offset)
    {
      makeControl (pane, labels[0], parts, ctlparts,
	           QPixmap (px_rot_xpm), QPixmap (nx_rot_xpm));
      makeControl (pane, labels[1], parts, ctlparts,
	           QPixmap (py_rot_xpm), QPixmap (ny_rot_xpm));
      makeControl (pane, labels[2], parts, ctlparts,
		   QPixmap (pz_rot_xpm), QPixmap (nz_rot_xpm));
    
      // Add alignment button group if requested
      if (parts & AlignmentPart)
      {
	m_alignment = new IgQtAxisAlignment (area);
	addGridWidget (makeSpacedBox (area,
	                              m_alignment->widget ()),
				      gridRows (), 1);
	// Link the check boxes toggles to alignment selection code
        connect (this,			SIGNAL (editable    (bool)), 
	         m_alignment->widget (),SLOT   (setEnabled   (bool)));
        connect (m_alignment,		SIGNAL (dirty()),
	         this,			SLOT   (alignToAxis ()));
        connect (this,			SIGNAL (clean()),
		 m_alignment,		SLOT   (setClean ()));
      }
      
      for(unsigned int i = 0; i < m_offset; i++)
        m_controls[i]->updateEditSettingConnection (m_controls[0],
	                                            SLOT (editSettings (void)));

      for(unsigned int i = 1; i < m_offset; i++)
	m_controls[i]->setEmitSettingsChanged (false);
      
      connect (m_controls[0], SIGNAL (settingsChanged (void)),
               this, SLOT (syncSettings1 (void)));
    }
    
    for(unsigned int i = m_offset; i < size; i++)
      makeControl (pane, labels[i], parts, ctlparts,
	           QPixmap (), QPixmap ());
      
    pane->popIndent ();
    
    for(unsigned int i = m_offset; i < size; i++)
      m_controls[i]->updateEditSettingConnection (m_controls[m_offset],
	                                          SLOT (editSettings (void)));

    for(unsigned int i = m_offset+1; i < size; i++)
      m_controls[i]->setEmitSettingsChanged (false);
    
    if (m_offset < size)
      connect (m_controls[m_offset], SIGNAL (settingsChanged (void)),
               this, SLOT (syncSettings2 (void)));

    connect (this, SIGNAL (editable (bool)), vlab, SLOT (setEnabled (bool)));

    emit clean ();
}

void
Ig3DVectorBaseControl::makeControl (IgControlCategory *pane, QString label,
				    int parts, int ctlparts,
				    QPixmap npix, QPixmap ppix)
{
    ASSERT (pane);
    ASSERT (pane->bodyArea ());
    ASSERT (pane->bodyLayout ());

    QWidget	*area = pane->bodyArea ();
    int		row = gridRows ();

    ASSERT (row >= 0);

    IgQtRangeControlFloat *ctl = new IgQtRangeControlFloat (area, ctlparts);
    QWidget 	     *vlab = makeIndentedLabel (label, area);
    addGridWidget (vlab, row, 0);
    addGridWidget (makeSpacedBox (area, ctl->widget ()), row, 1);
    if (! label.isEmpty())
    {    
      QString tips ("Set the \"");
      tips += label + "\" of the item";
      QWhatsThis::add (vlab, tips.latin1 ());
    }

    if (parts & RotatorPart)
    {
	ctl->setBoundaryHandling (IgQtRangeControlFloat::MODULATE);
	ctl->setRange (-180,180);
	ctl->setSmallStep (1);
	ctl->setLongStep (5);
	ctl->setValue (0);
    }

    if (parts & RotatorButtonPart)
    {	
	QToolButton *rotationButton = new QToolButton (0);
	rotationButton->setPixmap (QPixmap (npix));
	rotationButton->setAutoRaise (true);
	rotationButton->setAutoRepeat (true);
	ctl->addRevButton (rotationButton);
	rotationButton = new QToolButton (0);
	rotationButton->setPixmap (QPixmap (ppix));
	rotationButton->setAutoRaise (true);
	rotationButton->setAutoRepeat (true);
	ctl->addFwdButton (rotationButton);
    }

    ctl->valueEditable (false);
    
    connect(ctl,  SIGNAL(dirty()), this, SLOT(setDirty ()));
    connect(this, SIGNAL(clean()), ctl,  SLOT(setClean ()));
    connect(pane, SIGNAL(tracking(bool)), ctl, SLOT(resetChangedValue(bool)));
    connect(this, SIGNAL(editable(bool)), ctl->widget(),SLOT(setEnabled(bool)));
    connect(this, SIGNAL(editable(bool)), vlab, SLOT(setEnabled(bool)));
    
    m_controls.push_back (ctl);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtRangeControlFloat *
Ig3DVectorBaseControl::control (unsigned int index) const
{ ASSERT (index < m_controls.size ()); return m_controls[index]; }

std::vector<IgQtRangeControlFloat *>
Ig3DVectorBaseControl::controls (void) const
{ return m_controls; }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
float
Ig3DVectorBaseControl::value (unsigned int index) const
{ ASSERT (index < m_controls.size ()); return m_controls[index]->value (); }

std::vector<float>
Ig3DVectorBaseControl::values (void) const
{
    std::vector<float> data;
    for(unsigned int i =0; i < m_controls.size (); i++)
      data.push_back (m_controls[i]->value ());
    return data;
}

void
Ig3DVectorBaseControl::setValues (const float *values, unsigned int size)
{
    for(unsigned int i = 0; i < size; i++)
      disconnect (m_controls[i], SIGNAL(dirty()), this, SLOT(setDirty()));
      
    for(unsigned int i = 0; i < size; i++)
      m_controls[i]->setValue (values[i]);
    
    for(unsigned int i = 0; i < size; i++)
      connect (m_controls[i], SIGNAL(dirty()), this, SLOT(setDirty()));
      
    setDirty (true);
}

void
Ig3DVectorBaseControl::setValues (const std::vector<float> &values)
{ setValues (&values[0], values.size ()); }

void
Ig3DVectorBaseControl::setValue (float value, unsigned int index)
{ ASSERT (index < m_controls.size ()); m_controls[index]->setValue (value);}

void
Ig3DVectorBaseControl::setDirty (bool dirty)
{ Ig3DFieldControl::setDirty (dirty); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
Ig3DVectorBaseControl::isChanged (void) const
{
    for(unsigned int i = 0; i < m_controls.size (); i++)
      if (m_controls[i]->isValueChanged ())
        return true;
    return false;
}

float
Ig3DVectorBaseControl::change (unsigned int index) const
{ ASSERT (index < m_controls.size ()); return m_controls[index]->valueChange (); }

std::vector<float>
Ig3DVectorBaseControl::changes (void) const
{
    std::vector<float> data;
    for(unsigned int i =0; i < m_controls.size (); i++)
      data.push_back (m_controls[i]->valueChange ());
    return data;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
float
Ig3DVectorBaseControl::minAxisAlign (void) const
{ ASSERT (m_alignment); return m_alignment->minAxisAlign (); }

float
Ig3DVectorBaseControl::maxAxisAlign (void) const
{ ASSERT (m_alignment); return m_alignment->maxAxisAlign (); }

void
Ig3DVectorBaseControl::setMinAxisAlign (float newval)
{ ASSERT (m_alignment); m_alignment->setMinAxisAlign (newval); }

void
Ig3DVectorBaseControl::setMaxAxisAlign (float newval)
{ ASSERT (m_alignment); m_alignment->setMaxAxisAlign (newval); }

void
Ig3DVectorBaseControl::alignToAxis (void)
{ ASSERT (m_alignment); setValues (m_alignment->value ().getValue (), 3); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DVectorBaseControl::updateValue (void)
{ setValues (values ()); }

void
Ig3DVectorBaseControl::applyValue (void)
{ ASSERT (false); }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
Ig3DVectorBaseControl::syncSettings1 (void)
{ syncSettings (0, m_offset); }

void
Ig3DVectorBaseControl::syncSettings2 (void)
{ syncSettings (m_offset, m_controls.size ()); }

void
Ig3DVectorBaseControl::syncSettings (unsigned int start, unsigned int end)
{
    for(unsigned int i = start+1; i < end; i++)
    {
      m_controls[i]->setPrecision (m_controls[start]->precision ());
      m_controls[i]->setRange (m_controls[start]->minValue (),
                               m_controls[start]->maxValue ());
      m_controls[i]->setSmallStep (m_controls[start]->getSmallStep ());
      m_controls[i]->setLongStep (m_controls[start]->getLongStep ());
    }
    fieldSettingsChanged ();
}

QString
Ig3DVectorBaseControl::getSettingString (void) const
{
  QString setting (m_controls[0]->getSettingString ());
  if (m_offset < m_controls.size())
    IgQtRangeControlCommon::addSettingItem (m_controls[m_offset]->
                                           getSettingString (),
                                           setting);
  return setting;
}
    
void
Ig3DVectorBaseControl::setSettingString (QString &setting)
{
  m_controls[0]->setSettingString (setting);
  if (m_offset < m_controls.size())
    m_controls[m_offset]->setSettingString (setting);
}
