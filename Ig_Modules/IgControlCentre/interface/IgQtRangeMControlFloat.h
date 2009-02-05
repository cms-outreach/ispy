#ifndef IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_FLOAT_H
# define IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_FLOAT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMValueFloat.h"
# include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControl.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgQtRangeMControlFloat : public IgQtRangeMControl,
                                                  public IgQtRangeMValueFloat
{
    Q_OBJECT
public:
    typedef IgQtRangeMValueFloat inherited;
    
    IgQtRangeMControlFloat (QWidget *parent, int parts,
                       float value = 0.,
		       const char *name = 0);

    IgQtRangeMControlFloat (QWidget *parent, int parts,
                       unsigned int size,
		       const float* values,
		       const char *name = 0);

    IgQtRangeMControlFloat (QWidget *parent, int parts,
                       const rangeMValues& values,
		       const char *name = 0);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void	setPrecision (float precision);

    float	stringToNumber (const QString& val) const
    { return val.toFloat(); }
    
    using IgQtRangeMControl::setDirty;
    void		setDirty (bool dirty);

    void		resize (unsigned int size, float appendedValues = 0);
    
    float		getSmallStep (void) const;
    float		getLongStep (void) const;

    void		setSmallStep (float size);
    void		setLongStep (float size);

    void		setRange (float min, float max);

    virtual void	setValue (float newval);
    virtual void	setValue (unsigned int index, float newval);
    virtual void	setValue (unsigned int size, const float* newvals);
    virtual void	setValue (const rangeMValues& newvals);

    void		revMin (void);
    void		revLong (void);
    void		revSmall (void);
    void		fwdSmall (void);
    void		fwdLong (void);
    void		fwdMax (void);

    void		editSettings (void);

    QString		getSettingString (void) const;
    void		setSettingString (QString &setting);
    
signals:
    void		valueChanged (std::vector<float> delta);
    void		newValue (std::vector<float> newval);

protected:
    virtual void 	emitSignals (void);
    virtual void	initialize (int parts);
    virtual void	adjustSliderSteps (void);
    virtual void	moveValue (float delta, MoveDirection dir);
    virtual void	fwdEnable (bool enable);
    virtual void	revEnable (bool enable);

    void		sliderChanged (int newval);
    void		textChanged (const QString &value);
    void		updateTextValue (void);

    float		m_longStep;
    float		m_smallStep;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_QT_RANGE_MCONTROL_FLOAT_H
