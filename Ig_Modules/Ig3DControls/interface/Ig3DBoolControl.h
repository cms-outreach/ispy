#ifndef IG_3D_CONTROLS_IG_3DBOOL_CONTROL_H
# define IG_3D_CONTROLS_IG_3DBOOL_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/Ig3DFieldControl.h"
# include <Inventor/fields/SoSFBool.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoSFBool;
class QCheckBox;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DBoolControl : public Ig3DFieldControl
{
    Q_OBJECT
public:
    Ig3DBoolControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    SoSFBool *		field (void) const;
    void		setField (SoSFBool *field);
    bool		fieldValue (void) const;

    bool		value (void) const;
    QCheckBox *		widget (void) const;
public slots:
    void		setValue (bool newval); 

protected:
    virtual void	updateValue (void);
    virtual void	applyValue (void);

private:
    QCheckBox		*m_value;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DBOOL_CONTROL_H
