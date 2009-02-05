#ifndef IG_3D_CATEGORIES_IG_3D_BASE_SLICERS_CATEGORY_H
# define IG_3D_CATEGORIES_IG_3D_BASE_SLICERS_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DCategories/interface/config.h"
# include "Ig_Modules/Ig3DCategories/interface/Ig3DObjectCategory.h"
# include "Ig_Extensions/IgOpenInventor/interface/IgSoSlicer.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DBaseRep;
class Ig3DBaseBrowser;
class SbVec3f;
class SoGroup;
class SoSwitch;
class Ig3DSlicersCategory;
class Ig3DPlaneControl;
class Ig3DFloatControl;
class Ig3DBoolControl;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CATEGORIES_API Ig3DSlicersCategory
    : public Ig3DObjectCategory<IgSoSlicer>,
      public IgStateElement
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (Ig3DSlicersCategory);    
public:
    Ig3DSlicersCategory (IgState *state,
		         Ig3DBaseBrowser *source);
    // implicit destructor

    void		makeOne 	(Ig3DBaseRep *rep);
    void		reconstruct 	(void);
    void		browserChanged  (Ig3DBaseBrowser *browser);
    
    virtual void	attach (void);
    virtual void	detach (void);
    virtual void	update (void);
    virtual void	apply (void);
    virtual void	registerBrowser (IgState *state,
                                         Ig3DBaseBrowser *browser);    

protected:
    virtual void	createNewItem (void);
    virtual void	deleteItem (void);
    virtual void	renameItem (const QString &newName);

private slots:
    virtual void	makeOne 	(void);
    virtual void 	deleteOne 	(void);

private:
    void		reconstruct 	(SoGroup *from);
    void		buildDefaults 	(void);
    SoSwitch *		repSlicers (SoGroup	*magic,
				    bool	make,
				    SoGroup	**objects,
				    SoGroup	**active);
    SoGroup *		repActiveSlicers (Ig3DBaseRep *rep, bool make);
    SoGroup *		repAllSlicers (Ig3DBaseRep *rep, bool make);
    IgSoSlicer *	makeSlicer (QString name, bool on, SbVec3f center);

    // virtual float	getThickness (void);
    // virtual void	setThickness (float);
    // void		setValue (float distance, float thickness);

    Ig3DBaseBrowser	*m_source;
    
    Ig3DPlaneControl	*m_plane;
    Ig3DFloatControl	*m_thickness;
    Ig3DBoolControl	*m_on;
    Ig3DBoolControl	*m_manip;

    static const char	*s_group;
    static const char	*s_all;
    static const char	*s_active;

    // undefined semantics
    Ig3DSlicersCategory (const Ig3DSlicersCategory &);
    Ig3DSlicersCategory &operator= (const Ig3DSlicersCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CATEGORIES_IG_3D_BASE_SLICERS_CATEGORY_H
