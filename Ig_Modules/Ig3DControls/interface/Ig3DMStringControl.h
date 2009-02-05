#ifndef IG_3D_CONTROLS_IG_3D_MSTRING_CONTROL_H
# define IG_3D_CONTROLS_IG_3D_MSTRING_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/Ig3DFieldControl.h"
# include <Inventor/SbLinear.h>
# include <Inventor/fields/SoMFString.h>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QComboBox;
class IgQtLineEdit;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DMStringControl : public Ig3DFieldControl
{
    Q_OBJECT
public:
    Ig3DMStringControl (IgControlCategory *pane, QString label);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    SoMFString *	field (void) const;
    void		setField (SoMFString *field);
    const SbString*	fieldValue (void) const;

    const SbString*	value (void) const;
    QWidget*		widget(void) const;
    virtual void	resize (unsigned int size);

public slots:
    virtual void	setValue (unsigned int size, const SbString* newval);
    virtual void	newIndex (int);

protected:
    virtual void	applyValue (void);
    virtual void	updateValue (void);

private slots:
    
    void		textChanged (const QString& newvalue);

private:
    QWidget		*m_value;
    IgQtLineEdit	*m_lineEdit;
    QComboBox		*m_indexs;
    std::vector<SbString> m_strings;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3D_MSTRING_CONTROL_H
