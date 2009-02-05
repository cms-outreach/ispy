#ifndef IG_3D_CONTROLS_IG_3DVECTOR_BASE_CONTROL_H
# define IG_3D_CONTROLS_IG_3DVECTOR_BASE_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/Ig3DFieldControl.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgQtRangeControlFloat;
class QPixmap;
class IgQtAxisAlignment;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DVectorBaseControl : public Ig3DFieldControl
{
    Q_OBJECT
public:
    enum Style {
	RotationStyle,
	DirectionStyle,
	PositionStyle,
	PositionWOAStyle,
	Vector
    };

    Ig3DVectorBaseControl (IgControlCategory	*pane,
		       QString			label,
		       Style			style,
		       unsigned int 		size,
		       const QString	        &colSepLabels);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    // Control value and change access
    float		value (unsigned int index) const;
    std::vector<float>	values (void) const;

    bool		isChanged (void) const;
    float		change (unsigned int index) const;
    std::vector<float>	changes (void) const;

    // Axis alignment options
    float		minAxisAlign (void) const;
    float		maxAxisAlign (void) const;
    void		setMinAxisAlign (float newval);
    void		setMaxAxisAlign (float newval);
    
    // Access control
    IgQtRangeControlFloat *	control (unsigned int index) const;
    std::vector<IgQtRangeControlFloat *> controls  (void) const;

    QString		getSettingString (void) const;
    void		setSettingString (QString &setting);

    using IgControlItem::setDirty;
public slots:
    virtual void	setValues (const std::vector<float> &values);
    virtual void	setValues (const float* values, unsigned int size);
    virtual void	setValue (float value, unsigned int index);
    virtual void	setDirty (bool value);
    
protected:
    virtual void	updateValue (void);
    virtual void	applyValue (void);

private slots:
    void		alignToAxis (void);
    void		syncSettings1 (void);
    void		syncSettings2 (void);
    
private:
    enum Parts {
	RotatorButtonPart	= 1,
	RotatorPart		= 2,
	ValuePart		= 4,
	AlignmentPart		= 8,
	AllParts		= (RotatorPart | RotatorButtonPart
				   | ValuePart | AlignmentPart)
    };

    void	syncSettings (unsigned int start, unsigned int end);
    void	makeControl (IgControlCategory *pane, QString label,
			     int parts, int ctlparts,
			     QPixmap npix, QPixmap ppix);

    std::vector<IgQtRangeControlFloat*> m_controls;
    IgQtAxisAlignment			*m_alignment;
    unsigned int			m_offset;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DVECTOR_BASE_CONTROL_H
