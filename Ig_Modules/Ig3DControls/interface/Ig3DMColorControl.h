#ifndef IG_3D_CONTROLS_IG_3DMCOLOR_CONTROL_H
# define IG_3D_CONTROLS_IG_3DMCOLOR_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/Ig3DFieldControl.h"
# include <Inventor/SbColor.h>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoMFColor;
class QComboBox;
class QSlider;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DMColorControl : public Ig3DFieldControl
{
    Q_OBJECT
public:
    Ig3DMColorControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    SoMFColor *		field (void) const;
    void		setField (SoMFColor *field);
    const SbColor*	fieldValue (void) const;

    const SbColor*	value (void) const;
    QWidget*		widget(void) const;
    virtual void	resize (unsigned int size);

public slots:
    virtual void	setValue (unsigned int size, const SbColor* color);
    virtual void	chooseColor (void);
    virtual void	newIndex (int);

protected:
    virtual void	applyValue (void);
    virtual void	updateValue (void);

private slots:
    virtual void	updateColorWidget (void);
    virtual void	redSliderChanged (int value);
    virtual void	greenSliderChanged (int value);
    virtual void	blueSliderChanged (int value);
    
private:
    void		updateColor (float r, float g, float b);
    QWidget		*m_value;
    QWidget		*m_color;
    QComboBox		*m_indexs;
    std::vector<SbColor> m_colors;
    QSlider		*m_redSlider;
    QSlider		*m_greenSlider;
    QSlider		*m_blueSlider;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DMCOLOR_CONTROL_H
