//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlCommon.h"
#include "Ig_Extensions/IgQt/interface/IgQtLineEdit.h"
#include "Ig_Extensions/IgQt/interface/IgQtPixButton.h"
#include "Ig_Modules/IgControlCentre/pixmaps/gotoMin.xpm"
#include "Ig_Modules/IgControlCentre/pixmaps/fastRev.xpm"
#include "Ig_Modules/IgControlCentre/pixmaps/rev.xpm"
#include "Ig_Modules/IgControlCentre/pixmaps/fwd.xpm"
#include "Ig_Modules/IgControlCentre/pixmaps/fastFwd.xpm"
#include "Ig_Modules/IgControlCentre/pixmaps/gotoMax.xpm"
#include <qframe.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qslider.h>
#include <qpopupmenu.h>
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

IgQtRangeControlCommon::IgQtRangeControlCommon (QWidget *parent, int /*parts*/,
				                const char *name /* = 0 */)
    : m_stepType   (ADD),
      m_fwdEnabled (true),
      m_revEnabled (true),
      m_widget	   (new QFrame (parent, name)),
      m_layout	   (0),
      m_row        (0),
      m_revMin	   (0),
      m_revLong	   (0),
      m_revSmall   (0),
      m_text       (0),
      m_fwdSmall   (0),
      m_fwdLong    (0),
      m_fwdMax     (0),
      m_slider     (0),
      m_resetChangedValue (0),
      m_editSettingMenu (0),
      m_editSettingMenuIndex (-1),
      m_valueEditable (true),
      m_stepEditable (true),
      m_rangeEditable  (true),
      m_precisionEditable (true),
      m_emitSettingsChanged (true)
{ }

void
IgQtRangeControlCommon::initialize (int parts)
{
    QSizePolicy	expexp (QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (!m_layout)
      m_layout = new QVBoxLayout (m_widget, 1, 0);

    m_widget->setFrameStyle (QFrame::Plain | QFrame::Box);
    m_widget->setFixedHeight (35);

    m_row = new QWidget (m_widget);
    QHBoxLayout	*rowLayout = new QHBoxLayout (m_row, 0, 0);
    m_layout->addWidget (m_row);

    m_editSettingMenu = new QPopupMenu (0, "m_editSettingMenu");
    updateEditSettingConnection (this, SLOT (editSettings (void)));
    
    // Add backward buttons
    if (parts & StepperPart)
    {
	m_revMin   = new IgQtPixButton (gotoMin_xpm, m_row);
	m_revMin->setPopup (m_editSettingMenu);
	m_revLong  = new IgQtPixButton (fastRev_xpm, m_row);
	m_revLong->setPopup (m_editSettingMenu);
	m_revSmall = new IgQtPixButton (rev_xpm, m_row);
	m_revSmall->setPopup (m_editSettingMenu);

	rowLayout->addWidget (m_revMin);
	rowLayout->addWidget (m_revLong);
	rowLayout->addWidget (m_revSmall);

	connect (m_revMin,   SIGNAL (clicked ()), this, SLOT (revMin ()));
	connect (m_revLong,  SIGNAL (clicked ()), this, SLOT (revLong ()));
	connect (m_revSmall, SIGNAL (clicked ()), this, SLOT (revSmall ()));
	
	// Adding What's this help messages
	const char * text_m_revMin =
	  "Click this to move the value to minimum.";
	const char * text_m_revLong =
	  "Click this to make a long step in reverse direction. Keep the mouse "
	  "clicked on this if you want to keep on making long steps in reverse "
	  "direction.";
	const char * text_m_revSmall =
	  "Click this to make a small step in reverse direction. Keep the mouse "
	  "clicked on this if you want to keep on making small steps in reverse "
	  "direction.";
	
	QWhatsThis::add(m_revMin, text_m_revMin);
	QWhatsThis::add(m_revLong, text_m_revLong);
	QWhatsThis::add(m_revSmall, text_m_revSmall);
    }

    // Add text edit for the value (FIXME: optional label vs. text)
    if (parts & ValuePart)
    {
	m_text = new IgQtLineEdit (m_row);
	m_text->setSizePolicy (expexp);
	m_text->setReadOnly (false);
	m_text->setFocusPolicy (QWidget::StrongFocus);
	m_text->setAlignment (AlignRight);
	m_text->setFixedWidth (75);
	m_text->setText ("0");

	rowLayout->addWidget (m_text);
	connect (m_text, SIGNAL (valueChanged (const QString&)),
		 this,   SLOT (textChanged (const QString&)));
	
	// Adding What's this help messages
	const char * text_m_text =
	  "This text box displays the current value of the control. One can "
	  "also edit value here and hit the \"Enter/Return Key\" to make "
	  "that value effective.";
	  
	QWhatsThis::add(m_text, text_m_text);
    }

    // Add forward buttons
    if (parts & StepperPart)
    {
	m_fwdSmall = new IgQtPixButton (fwd_xpm, m_row);
	m_fwdSmall->setPopup (m_editSettingMenu);
	m_fwdLong  = new IgQtPixButton (fastFwd_xpm, m_row);
	m_fwdLong->setPopup (m_editSettingMenu);
	m_fwdMax   = new IgQtPixButton (gotoMax_xpm, m_row);
	m_fwdMax->setPopup (m_editSettingMenu);

	rowLayout->addWidget (m_fwdSmall);
	rowLayout->addWidget (m_fwdLong);
	rowLayout->addWidget (m_fwdMax);

	connect (m_fwdSmall, SIGNAL (clicked ()), this, SLOT (fwdSmall ()));
	connect (m_fwdLong,  SIGNAL (clicked ()), this, SLOT (fwdLong ()));
	connect (m_fwdMax,   SIGNAL (clicked ()), this, SLOT (fwdMax ()));
	
	// Adding What's this help messages
	const char * text_m_fwdMax =
	  "Click this to move the value to maximum.";
	const char * text_m_fwdLong =
	  "Click this to make a long step in forward direction. Keep the "
	  "mouse clicked on this if you want to keep on making long steps in "
	  "forward direction.";
	const char * text_m_fwdSmall =
	  "Click this to make a small step in forward direction. Keep the "
	  "mouse clicked on this if you want to keep on making small steps in "
	  "forward direction.";
	
	QWhatsThis::add(m_fwdMax, text_m_fwdMax);
	QWhatsThis::add(m_fwdLong, text_m_fwdLong);
	QWhatsThis::add(m_fwdSmall, text_m_fwdSmall);
    }

    // Add slider below the row
    if (parts & SliderPart)
    {
	m_slider = new QSlider (Horizontal, m_widget);
	m_slider->setSizePolicy (expexp);
	m_slider->setFixedHeight (8);
	m_slider->setLineStep (1);
	m_layout->addWidget (m_slider);

	connect (m_slider, SIGNAL (valueChanged (int)),
		 this,     SLOT (sliderChanged (int)));
    
	// Adding What's this help messages
	const char * text_m_slider =
	  "Click on this slider to make a small step in either reverse or "
	  "forward direction.";
	  
	QWhatsThis::add(m_slider, text_m_slider);
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/** Set the stepping fashion to @a value.  */
void
IgQtRangeControlCommon::setStepType (StepType value)
{ m_stepType = value; }

/** Get the current way stepping is done.  */
IgQtRangeControlCommon::StepType
IgQtRangeControlCommon::getStepType (void) const
{ return m_stepType; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/** Slider has changed to position @a value small steps from the
    minimum.  Adjust current control value. */
void
IgQtRangeControlCommon::sliderChanged (int /*value*/)
{ } 

void
IgQtRangeControlCommon::resetChangedValue (bool value)
{ m_resetChangedValue = value; }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeControlCommon::revMin (void)
{ }

void
IgQtRangeControlCommon::revLong (void)
{ }

void
IgQtRangeControlCommon::revSmall (void)
{ }

void
IgQtRangeControlCommon::fwdSmall (void)
{ }

void
IgQtRangeControlCommon::fwdLong (void)
{ }

void
IgQtRangeControlCommon::fwdMax (void)
{ }

void
IgQtRangeControlCommon::emitSignals (void)
{ }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
IgQtRangeControlCommon::textChanged (const QString& /*newval*/)
{ }

void
IgQtRangeControlCommon::setDirty (bool dirty)
{ IgControlItem::setDirty (dirty); }

void
IgQtRangeControlCommon::fwdEnable (bool value)
{
    if (m_fwdSmall && m_fwdEnabled != value)
    {
	m_fwdSmall->setEnabled (value);
	m_fwdLong->setEnabled (value);
	m_fwdMax->setEnabled (value);
	m_fwdEnabled = value;
    }
}

void
IgQtRangeControlCommon::revEnable (bool value)
{
    if (m_revSmall && m_revEnabled != value)
    {
	m_revSmall->setEnabled (value);
	m_revLong->setEnabled (value);
	m_revMin->setEnabled (value);
	m_revEnabled = value;
    }
}

void
IgQtRangeControlCommon::addFwdButton (QWidget *w)
{
    addFwdWidget (w);
    connect (w, SIGNAL(clicked()), this, SLOT (fwdSmall ()));
}

void
IgQtRangeControlCommon::addRevButton (QWidget *w)
{
    addRevWidget (w);
    connect (w, SIGNAL(clicked()), this, SLOT (revSmall ()));
}

void
IgQtRangeControlCommon::addFwdWidget (QWidget *w)
{ insertWidget (w, -1); }

void
IgQtRangeControlCommon::addRevWidget (QWidget *w)
{ insertWidget (w, 0); }

void
IgQtRangeControlCommon::insertWidget (QWidget *w, int index /* = -1 */)
{
    w->reparent (m_row, QPoint (0, 0));
    ((QBoxLayout *) m_row->layout ())->insertWidget (index, w);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
QWidget *
IgQtRangeControlCommon::widget (void) const
{ return m_widget; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeControlCommon::show (void)
{ m_widget->show (); }

void
IgQtRangeControlCommon::hide (void)
{ m_widget->hide (); }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeControlCommon::editSettingsChanged (void)
{ if (isEmitSettingsChanged ()) emit settingsChanged (); }

void
IgQtRangeControlCommon::editSettings (void)
{ }

QPopupMenu*
IgQtRangeControlCommon::popupMenu (void) const
{ return m_editSettingMenu; }

void
IgQtRangeControlCommon::updateEditSettingConnection (const QObject * receiver,
                                               const char * member)
{
    if (m_editSettingMenuIndex != -1)
      m_editSettingMenu->removeItemAt (m_editSettingMenuIndex);
    int id = m_editSettingMenu->insertItem ("&Edit",
                                            receiver ,
                                            member,
					    0, -1, m_editSettingMenuIndex);
    m_editSettingMenuIndex = m_editSettingMenu->indexOf (id);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgQtRangeControlCommon::valueEditable (bool editable /*= true*/)
{ m_valueEditable = editable; }
    
void
IgQtRangeControlCommon::rangeEditable (bool editable /*= true*/)
{ m_rangeEditable = editable; }
    
void
IgQtRangeControlCommon::stepEditable (bool editable /*= true*/)
{ m_stepEditable = editable; }

void
IgQtRangeControlCommon::precisionEditable (bool editable /*= true*/)
{ m_precisionEditable = editable; }

bool
IgQtRangeControlCommon::isValueEditable (void) const
{ return m_valueEditable; }
    
bool
IgQtRangeControlCommon::isRangeEditable (void) const
{ return m_rangeEditable; }
    
bool
IgQtRangeControlCommon::isStepEditable (void) const
{ return m_stepEditable; }

bool
IgQtRangeControlCommon::isPrecisionEditable (void) const
{ return m_precisionEditable; }

bool
IgQtRangeControlCommon::isEmitSettingsChanged (void) const
{ return m_emitSettingsChanged; }

void
IgQtRangeControlCommon::setEmitSettingsChanged (bool value /*=true*/)
{ m_emitSettingsChanged = value; }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
QString
IgQtRangeControlCommon::getSettingItem (QString &setting)
{
    QString item ("");
    int index = setting.find(":");
    if (index == -1)
    {
      item = setting;
      setting = "";
    }
    else
    {
      item = setting.left(index);
      setting = setting.right (setting.length()-index-1);
    }
    return item;
}

void
IgQtRangeControlCommon::addSettingItem (const QString &item, QString &setting)
{
    if (setting.isEmpty ())
      setting = item;
    else
      setting += ":" + item;
}
