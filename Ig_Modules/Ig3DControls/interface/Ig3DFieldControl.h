#ifndef IG_3D_CONTROLS_IG_3DFIELD_CONTROL_H
# define IG_3D_CONTROLS_IG_3DFIELD_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DControls/interface/config.h"
# include "Ig_Modules/IgControlCentre/interface/IgControlBase.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoField;
class SoFieldSensor;
class SoSensor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CONTROLS_API Ig3DFieldControl : public IgControlBase
{
    Q_OBJECT
public:
    Ig3DFieldControl (IgControlCategory *pane);
    ~Ig3DFieldControl (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    void		clearField (void);
    virtual void	attach (void);
    virtual void	detach (void);

public slots:
    virtual void	fieldSettingsChanged (void);
    
signals:
    void		settingsChanged (SoField*);
    void		settingsChanged (void);
    
protected:
    virtual bool	hasValue (void) const;

    void		setField (SoField *field);
    SoField *		field (void) const;

protected:
    static void		fieldChanged (void *data, SoSensor *sensor);

    SoField		*m_field;
    SoFieldSensor	*m_sensor;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CONTROLS_IG_3DFIELD_CONTROL_H
