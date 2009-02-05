#ifndef IG_3D_CONTROLS_IG_3D_STRING_CONTROL_H
# define IG_3D_CONTROLS_IG_3D_STRING_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/Ig3DFieldControl.h"
# include <Inventor/SbLinear.h>
# include <Inventor/fields/SoSFString.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgQtLineEdit;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DStringControl : public Ig3DFieldControl
{
    Q_OBJECT
public:
    Ig3DStringControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    SoSFString *	field (void) const;
    void		setField (SoSFString *field);
    SbString		fieldValue (void) const;

    SbString		value (void) const;
    IgQtLineEdit *	widget (void) const;
public slots:
    void		setValue (const SbString& newval); 

protected:
    virtual void	updateValue (void);
    virtual void	applyValue (void);

private slots:
    void		textChanged (const QString& newval); 

private:
    IgQtLineEdit	*m_value;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3D_STRING_CONTROL_H
