#ifndef IG_3D_CONTROLS_IG_3D_MBOOL_CONTROL_H
# define IG_3D_CONTROLS_IG_3D_MBOOL_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/Ig3DFieldControl.h"
# include <Inventor/SbLinear.h>
# include <Inventor/fields/SoMFBool.h>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QComboBox;
class QCheckBox;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DMBoolControl : public Ig3DFieldControl
{
    Q_OBJECT
public:
    Ig3DMBoolControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    SoMFBool *		field (void) const;
    void		setField (SoMFBool *field);
    const SbBool*	fieldValue (void) const;

    const SbBool*	value (void) const;
    QWidget*		widget(void) const;
    virtual void	resize (unsigned int size);

public slots:
    virtual void	setValue (unsigned int size, const SbBool* newval);
    virtual void	newIndex (int);

protected:
    virtual void	applyValue (void);
    virtual void	updateValue (void);

private slots:
    void		valueChanged (bool newval);

private:
    QWidget		*m_value;
    QCheckBox		*m_checkBox;
    QComboBox		*m_indexs;
    std::vector<SbBool> m_values;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3D_MBOOL_CONTROL_H
