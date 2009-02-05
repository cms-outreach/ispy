#ifndef IG_3D_CATEGORIES_IG_3D_LIGHTS_CATEGORY_H
# define IG_3D_CATEGORIES_IG_3D_LIGHTS_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DCategories/interface/config.h"
# include "Ig_Modules/Ig3DCategories/interface/Ig3DObjectCategory.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <Inventor/nodes/SoLight.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QComboBox;
class QCheckBox;
class SoLight;

class SoGroup;
class SoDirectionalLight;
class SoPointLight;
class SoSpotLight;

class Ig3DBaseBrowser;

class Ig3DVec3fControl;
class Ig3DUnitVec3fControl;
class Ig3DFloatControl;
class Ig3DAngleControl;
class Ig3DColorControl;
class Ig3DBoolControl;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CATEGORIES_API Ig3DLightsCategory
    : public Ig3DObjectCategory<SoLight>,
      public IgStateElement
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (Ig3DLightsCategory);

public:
    
    enum
    {
	//index of different types of SoLight
	SpotLight = 0,
	PointLight,
	DirectionalLight
    };
    
    Ig3DLightsCategory (IgState *state);
    ~Ig3DLightsCategory (void);

    void		addOne (SoLight *object);
    virtual void	buildDefaults (void);
    virtual void	addExtraLights (SoGroup *group);
    void		reconstruct (void);
    void		browserChanged (Ig3DBaseBrowser *browser);
    
    virtual void	attach (void);
    virtual void	detach (void);
    virtual void	update (void);
    virtual void	apply (void);
    virtual void	registerBrowser (IgState *state, Ig3DBaseBrowser *browser);    

public slots:
    virtual void	manipulators (bool on);

protected:
    virtual void	createNewItem (void);
    virtual void	deleteItem (void);
    virtual void	renameItem (const QString &newName);

private slots:
    virtual void	makeOne 	(void);
    virtual void 	deleteOne 	(void);
    virtual void	trackingChanged (bool mode);
    virtual void	lightTypeChanged(int index);

private:
    void		reconstruct 	(SoGroup *from);
    void		toggleOne	(bool show, SoLight *object);

    SoDirectionalLight 	*asDirectionalLight (SoLight *object);
    SoPointLight 	*asPointLight (SoLight *object);
    SoSpotLight 	*asSpotLight (SoLight *object);
    bool		isManipulatorType (SoLight *object);

    Ig3DBaseBrowser	*m_source;

    QComboBox		*m_lightsType;
    Ig3DVec3fControl	*m_location;	// point + spot
    Ig3DUnitVec3fControl *m_direction;	// directional + spot
    Ig3DFloatControl	*m_dropOffRate;	// spot
    Ig3DAngleControl	*m_cutOffAngle;	// spot
    Ig3DFloatControl	*m_intensity;	// all
    Ig3DColorControl	*m_color;	// all
    Ig3DBoolControl	*m_on;		// all
    QCheckBox	       	*m_manip;

    // OpenGL rendering engines usually have a fixed 
    // maximum number of available light sources which 
    // can be present in the state at the same time.
    int 	       	m_nrlights;	// The maximum number of light sources 
                                        // for OpenGL rendering.

    // undefined semantics
    Ig3DLightsCategory (const Ig3DLightsCategory &);
    Ig3DLightsCategory &operator= (const Ig3DLightsCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CATEGORIES_IG_3D_LIGHTS_CATEGORY_H
