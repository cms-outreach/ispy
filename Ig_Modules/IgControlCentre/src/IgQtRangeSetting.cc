//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeSetting.h"
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qsizepolicy.h>
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

IgQtRangeSetting::IgQtRangeSetting( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl ),
      m_valueChanged (false),
      m_rangeChanged (false),
      m_stepChanged (false),
      m_applied (false)
{
    setCaption("Range Controller");
    QVBoxLayout *m_layout1 = new QVBoxLayout( this, 10, 5, "m_layout1");    
    m_layout = new QGridLayout(m_layout1, 1, 1, 11, "m_layout");
    
    QLabel *value = new QLabel (this, "value");
    value->setText (tr ("Value"));
    m_value = new QLineEdit (this, "m_value");
    m_value->setAlignment (AlignRight);
    m_layout->addWidget (value, 0, 0);    
    m_layout->addWidget (m_value, 0, 1);
    
    QLabel *m_precisionLab = new QLabel (this, "precision");
    m_precisionLab->setText (tr ("Precision"));
    m_precision = new QLineEdit (this, "m_precision");
    m_precision->setAlignment (AlignRight);
    m_layout->addWidget (m_precisionLab, 0, 2);
    m_layout->addWidget (m_precision, 0, 3);

    QLabel *min = new QLabel (this, "min");
    min->setText (tr ("Minimum"));
    m_min = new QLineEdit (this, "m_min");
    m_min->setAlignment (AlignRight);
    m_layout->addWidget (min, 1, 0);    
    m_layout->addWidget (m_min, 1, 1);
    
    QLabel *max = new QLabel (this, "max");
    max->setText (tr ("Maximum"));
    m_max = new QLineEdit (this, "m_max");
    m_max->setAlignment (AlignRight);
    m_layout->addWidget (max, 1, 2);    
    m_layout->addWidget (m_max, 1, 3);
    
    QLabel *small = new QLabel (this, "small");
    small->setText (tr ("Small step"));
    m_small = new QLineEdit (this, "m_small");
    m_small->setAlignment (AlignRight);
    m_layout->addWidget (small, 2, 0);    
    m_layout->addWidget (m_small, 2, 1);
    
    QLabel *xlong = new QLabel (this, "long");
    xlong->setText (tr ("long step"));
    m_long = new QLineEdit (this, "m_long");
    m_long->setAlignment (AlignRight);
    m_layout->addWidget (xlong, 2, 2);    
    m_layout->addWidget (m_long, 2, 3);
    
    m_layout1->addItem(new QSpacerItem( 50, 20, QSizePolicy::Minimum,
                                                QSizePolicy::Expanding));
						
    QHBoxLayout *m_layout2 = new QHBoxLayout(m_layout1, 5, "m_layout2");
    m_cancel = new QPushButton ("&Cancel", this, "m_cancel");
    
    m_ok = new QPushButton ("&Ok", this, "m_ok");
    m_ok->setEnabled( true );
    m_ok->setDefault (true);
    
    m_layout2->addItem(new QSpacerItem( 50, 20, QSizePolicy::Expanding,
                                                QSizePolicy::Minimum ));
    m_layout2->addWidget (m_ok);
    m_layout2->addItem(new QSpacerItem( 50, 20, QSizePolicy::Expanding,
                                                QSizePolicy::Minimum ));
    m_layout2->addWidget (m_cancel);
    m_layout2->addItem(new QSpacerItem( 50, 20, QSizePolicy::Expanding,
                                                QSizePolicy::Minimum ));
    
    clear  ();
    enablePrecision (false);
    setModal(true);
    hide ();
    connect (m_cancel, SIGNAL(clicked ()), this, SLOT(cancel ()));
    connect (m_ok, SIGNAL(clicked ()), this, SLOT(apply ()));
    connect (m_value , SIGNAL (textChanged (const QString &)),
             this,     SLOT   (valueChanged(const QString &)));
    connect (m_min , SIGNAL (textChanged (const QString &)),
             this,     SLOT   (rangeChanged(const QString &)));
    connect (m_max , SIGNAL (textChanged (const QString &)),
             this,     SLOT   (rangeChanged(const QString &)));
    connect (m_small , SIGNAL (textChanged (const QString &)),
             this,     SLOT   (stepChanged(const QString &)));
    connect (m_long , SIGNAL (textChanged (const QString &)),
             this,     SLOT   (stepChanged(const QString &)));
    connect (m_precision , SIGNAL (textChanged (const QString &)),
             this,     SLOT   (precisionChanged(const QString &)));
}

void
IgQtRangeSetting::clear (void)
{
    m_valueChanged = false;
    m_rangeChanged = false;
    m_stepChanged = false;
    m_precisionChanged = false;
    m_applied = false;
}

void
IgQtRangeSetting::apply (void)
{
    m_applied = true;
    close ();
}

void
IgQtRangeSetting::cancel (void)
{
    clear ();
    close ();
}

void
IgQtRangeSetting::enablePrecision (bool enable /*=true*/)
{ m_precision->setEnabled (enable); }

void
IgQtRangeSetting::enableStep (bool enable /*=true*/)
{ m_small->setEnabled (enable); m_long->setEnabled (enable); }

void
IgQtRangeSetting::enableRange (bool enable /*=true*/)
{ m_min->setEnabled (enable); m_max->setEnabled (enable); }

void
IgQtRangeSetting::enableValue (bool enable /*=true*/)
{ m_value->setEnabled (enable); }

void
IgQtRangeSetting::valueChanged (const QString & /*newvalue*/)
{ m_valueChanged = true; }

void
IgQtRangeSetting::rangeChanged (const QString & /*newvalue*/)
{ m_rangeChanged = true; }

void
IgQtRangeSetting::stepChanged (const QString & /*newvalue*/)
{ m_stepChanged = true; }

void
IgQtRangeSetting::precisionChanged (const QString & /*newvalue*/)
{ m_precisionChanged = true; }

bool
IgQtRangeSetting::isValueChanged (void) const
{ return m_valueChanged; }

bool
IgQtRangeSetting::isRangeChanged (void) const
{ return m_rangeChanged; }

bool
IgQtRangeSetting::isStepChanged (void) const
{ return m_stepChanged; }

bool
IgQtRangeSetting::isPrecisionChanged (void) const
{ return m_precisionChanged; }

bool
IgQtRangeSetting::isApplied (void) const
{ return m_applied; }

QString
IgQtRangeSetting::getValue (void) const
{ return m_value->text (); }

QString
IgQtRangeSetting::getMin (void) const
{ return m_min->text (); }

QString
IgQtRangeSetting::getMax (void) const
{ return m_max->text (); }

QString
IgQtRangeSetting::getSmall (void) const
{ return m_small->text (); }

QString
IgQtRangeSetting::getLong (void) const
{ return m_long->text (); }

QString
IgQtRangeSetting::getPrecision (void) const
{ return m_precision->text (); }

void
IgQtRangeSetting::setValue (const QString& val)
{ m_value->setText (val); }

void
IgQtRangeSetting::setRange (const QString& min, const QString& max)
{ setMin (min); setMax (max); }

void
IgQtRangeSetting::setMin (const QString& val)
{ m_min->setText (val); }

void
IgQtRangeSetting::setMax (const QString& val)
{ m_max->setText (val); }

void
IgQtRangeSetting::setStep (const QString& smallStep, const QString& longStep)
{ setSmall (smallStep); setLong (longStep); }

void
IgQtRangeSetting::setSmall (const QString& val)
{ m_small->setText (val); }

void
IgQtRangeSetting::setLong (const QString& val)
{ m_long->setText (val); }

void
IgQtRangeSetting::setPrecision (const QString& val)
{ m_precision->setText (val); }

