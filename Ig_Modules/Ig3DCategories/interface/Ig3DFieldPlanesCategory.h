#ifndef IG_3D_CATEGORIES_IG_3D_FIELD_PLANES_CATEGORY_H
# define IG_3D_CATEGORIES_IG_3D_FIELD_PLANES_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DCategories/interface/config.h"
# include "Ig_Modules/Ig3DCategories/interface/Ig3DObjectCategory.h"
# include "Ig_Extensions/IgOpenInventor/interface/IgSoFieldPlane.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <qstring.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgSoFieldPlane;
class IgState;

class Ig3DFloatControl;
class Ig3DPlaneControl;
class Ig3DUInt32Control;
class Ig3DBoolControl;
class Ig3DEnumControl;
class QComboBox;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CATEGORIES_API Ig3DFieldPlanesCategory
    : public Ig3DObjectCategory<IgSoFieldPlane>,
      public IgStateElement
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (Ig3DFieldPlanesCategory);    
public:
    Ig3DFieldPlanesCategory (IgState *state);

    virtual void	attach (void);
    virtual void	detach (void);
    virtual void	update (void);
    virtual void	apply (void);

    virtual void	addOne (IgSoFieldPlane *fieldPlane);
    virtual void	deleteOne (IgSoFieldPlane *fieldPlane);

protected:
    virtual void	renameItem (const QString &newName);

private slots:
    virtual void	colorMapChanged (int index);
    virtual void	trackingChanged (bool mode);
    virtual void	applyColorMap   (void);
    
private:    
    Ig3DPlaneControl	*m_plane;
    Ig3DUInt32Control	*m_mapDensityX;
    Ig3DUInt32Control	*m_mapDensityZ;
    Ig3DUInt32Control	*m_segRatioX;
    Ig3DUInt32Control	*m_segRatioZ;
    Ig3DFloatControl	*m_maxvalue;
    Ig3DUInt32Control	*m_invisible;
    Ig3DUInt32Control	*m_alpha;
    Ig3DEnumControl	*m_component;
    QComboBox		*m_colorMap;
    Ig3DBoolControl	*m_show;
    Ig3DBoolControl	*m_showMap;
    Ig3DBoolControl	*m_showSegments;
    Ig3DBoolControl	*m_manip;
    Ig3DBoolControl	*m_gradientScale;
    
    bool		m_colorMapDirty;

    // undefined semantics
    Ig3DFieldPlanesCategory (const Ig3DFieldPlanesCategory &);
    Ig3DFieldPlanesCategory &operator= (const Ig3DFieldPlanesCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CATEGORIES_IG_3D_FIELD_PLANES_CATEGORY_H
