#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_SETTING_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_SETTING_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/config.h"
# include <qdialog.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QLineEdit;
class QPushButton;
class QGridLayout;
class QLabel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeSetting : public QDialog
{
    Q_OBJECT
public:

    IgQtRangeSetting (QWidget* parent = 0, 
		     const char* name = 0, 
		     bool modal = FALSE, 
		     WFlags fl = 0);
    
    bool 	     isValueChanged (void) const;
    bool 	     isRangeChanged (void) const;
    bool 	     isStepChanged (void) const;
    bool 	     isPrecisionChanged (void) const;
    bool	     isApplied (void) const;

    QString   	     getValue (void) const;
    QString   	     getMin (void) const;
    QString   	     getMax (void) const;
    QString   	     getSmall (void) const;
    QString   	     getLong (void) const;
    QString   	     getPrecision (void) const;

    void 	     setValue (const QString&);
    void 	     setRange (const QString& min,
                               const QString& max);
    void 	     setMin (const QString&);
    void 	     setMax (const QString&);
    void 	     setStep (const QString& smallStep,
                              const QString& longStep);
    void 	     setSmall (const QString&);
    void 	     setLong (const QString&);
    void 	     setPrecision (const QString&);
    
    void	     clear (void);
    
public slots:
    void 	     valueChanged (const QString&);
    void 	     rangeChanged (const QString&);
    void 	     stepChanged (const QString&);
    void 	     precisionChanged (const QString&);
    
    void	     enablePrecision (bool enable=true);
    void	     enableValue     (bool enable=true);
    void	     enableStep      (bool enable=true);
    void	     enableRange     (bool enable=true);
    virtual void     apply ();
    virtual void     cancel ();

protected:
    QGridLayout *m_layout;
    QLineEdit	*m_value;
    QLineEdit	*m_min;
    QLineEdit	*m_max;
    QLineEdit	*m_small;
    QLineEdit	*m_long;
    QLineEdit	*m_precision;
    QPushButton *m_cancel;
    QPushButton *m_ok;
    bool	m_valueChanged;
    bool	m_rangeChanged;
    bool	m_stepChanged;
    bool	m_precisionChanged;
    bool	m_applied;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_SETTING_H
