#ifndef IG_3D_CONTROLS_IG_3DCOLOR_CONTROL_H
# define IG_3D_CONTROLS_IG_3DCOLOR_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/Ig3DFieldControl.h"
# include <Inventor/SbLinear.h>
# include <Inventor/fields/SoSFColor.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoSFColor;
class QSlider;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DColorControl : public Ig3DFieldControl
{
    Q_OBJECT
public:
    Ig3DColorControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    SoSFColor *		field (void) const;
    void		setField (SoSFColor *field);
    SbVec3f		fieldValue (void) const;

    SbVec3f		value (void) const;
    QWidget*		widget(void) const;

public slots:
    virtual void	setValue (const SbVec3f &rgb);
    virtual void	setValue (float r, float g, float b);
    virtual void	chooseColor (void);

protected:
    virtual void	applyValue (void);
    virtual void	updateValue (void);

private slots:
    virtual void	updateColorWidget (void);
    virtual void	redSliderChanged (int value);
    virtual void	greenSliderChanged (int value);
    virtual void	blueSliderChanged (int value);
    
private:
    QWidget		*m_value;
    QWidget		*m_color;
    QSlider		*m_redSlider;
    QSlider		*m_greenSlider;
    QSlider		*m_blueSlider;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DCOLOR_CONTROL_H
