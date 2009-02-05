#ifndef IG_3D_WINDOW_IG_3D_WINDOW_H
# define IG_3D_WINDOW_IG_3D_WINDOW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DWindow/interface/config.h"
# include "Ig_Modules/Ig3DBaseWindow/interface/Ig3DBaseWindow.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;
class Ig3DSlicersCategory;
class Ig3DClipsCategory;
class Ig3DLightsCategory;
class IgSoMaterialCategory;
class Ig3DAnimsCategory;
class Ig3DViewpointsCategory;
class Ig3DNodeCategory;
class IgQtObjectMenuMessage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// QUESTION: this is not a view anymore, but a browser on steroids. Should 
// it be renamed?

class IG_3D_WINDOW_API Ig3DWindow : public Ig3DBaseWindow
{
    Q_OBJECT
public:
    
    Ig3DWindow (IgPage *page);
    ~Ig3DWindow (void);
    // implicit copy constructor
    // implicit assignment operator
    
    virtual void		initCategories (void);
    virtual void		initMenu (void);
    virtual void		initToolBar (void);
    virtual void 		focusIn (void);
    virtual void 		focusOut (void);
    
public slots:
    void			printViews(void);

private slots:
    
    void			showClips (void);
    void			showSlicers (void);
    void			showLights (void);
    void			showAnims (void);
    void			showViewpoints (void);
    void			showNodeManip (void);
    void			repSlice (void);
    void			repClip  (void);
    void			cameraToggled(void);

private:
    void			repMenu (IgQtObjectMenuMessage message);

    Ig3DSlicersCategory		*m_slicersCategory;
    Ig3DClipsCategory		*m_clipsCategory;
    Ig3DLightsCategory		*m_lightsCategory;
    IgSoMaterialCategory	*m_materialsCategory;
    Ig3DAnimsCategory		*m_animsCategory;
    Ig3DViewpointsCategory	*m_viewpointsCategory;
    Ig3DNodeCategory		*m_nodeCategory;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_WINDOW_IG_3D_WINDOW_H
