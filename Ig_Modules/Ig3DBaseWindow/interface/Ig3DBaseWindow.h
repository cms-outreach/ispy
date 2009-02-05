#ifndef IG_3D_BASE_WINDOW_IG_3D_BASE_WINDOW_H
# define IG_3D_BASE_WINDOW_IG_3D_BASE_WINDOW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DBaseWindow/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgView.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgBrowser.h"
# include <qobject.h>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgSite;
class IgPage;
class Ig3DBaseBrowser;
class Ig3DBaseModel;
class IgCommonViewPropertiesCategory;
class IgControlCentre;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// NOTE: This is meant to be a 3D/2D browser + all the control
// category and menu stuff. This is done to decouple the actual
// browser from IgStudio so that it is back in a lower position in the
// dependency tree. The name "Window" is there just because this
// happens to be a window in the MDI View...I dunno how to call it...
// Ig3DBaseBrowserOnSteroids?

class IG_3D_BASE_WINDOW_API Ig3DBaseWindow : public QObject,
					     public IgView,
					     public IgBrowser
{
    Q_OBJECT
public:
    static const int	TOOLBAR_ID_3D2D			= 3000;
    static const int 	TOOLBAR_ID_3D2D_SHOW_AXIS	= 1;
    static const int 	TOOLBAR_ID_3D2D_ZOOM_IN		= 2; 
    static const int 	TOOLBAR_ID_3D2D_ZOOM_OUT 	= 3; 
    static const int 	TOOLBAR_ID_3D2D_PICK		= 4; 
    static const int 	TOOLBAR_ID_3D2D_VIEW		= 5; 
    static const int	TOOLBAR_ID_3D2D_HOME		= 6; 
    static const int 	TOOLBAR_ID_3D2D_SET_HOME	= 7; 
    static const int 	TOOLBAR_ID_3D2D_ALL		= 8; 
    static const int 	TOOLBAR_ID_3D2D_SEEK		= 9; 
    static const int 	TOOLBAR_ID_3D2D_GRID		= 10; 
    static const int 	TOOLBAR_ID_3D2D_YZ_PLANE	= 11;
    static const int 	TOOLBAR_ID_3D2D_YX_PLANE	= 12;
    static const int 	TOOLBAR_ID_3D2D_XZ_PLANE	= 13;
    static const int 	TOOLBAR_ID_3D2D_INVERT		= 14;
    static const int 	TOOLBAR_ID_3D2D_ORTHO		= 15;
    static const int 	TOOLBAR_ID_3D2D_PRINT		= 16;
    static const int 	TOOLBAR_ID_3D2D_SAVE_AS		= 17;
    static const int 	TOOLBAR_ID_3D2D_CONTROL_CENTRE	= 18;
    static const int 	TOOLBAR_ID_3D2D_PRINT_DEFAULT_VIEWS = 19;
    static const int 	TOOLBAR_ID_3D2D_AUTO_PRINT	= 20;
    
    Ig3DBaseWindow (IgPage *page);
    ~Ig3DBaseWindow (void);
    // implicit copy constructor
    // implicit assignment operator
    virtual void 	initCategories (void);
    virtual void 	initMenu (void);
    virtual void	initToolBar (void);    
    virtual void	watchBrowser (Ig3DBaseBrowser *browser);
    virtual void	browse (IgRepresentable *object);
    virtual void	show (void);
    virtual void 	hide (void);
    virtual void 	focusIn (void);
    virtual void 	focusOut (void);
    virtual IgState*	state (void);
    IgControlCentre	*centre (void);
    	
    Ig3DBaseBrowser*	browser (void);
    Ig3DBaseModel*	model (void);

    void 		viewingModeCallback (void);

private slots:
    void		showViewProperties (void);
    void		showToolbar (void);
    void 		toolbarVisibilityChanged(bool visibility);
    void		pick (void);
    void		view (void);
    
private:
    IgState		*m_state;
    IgSite		*m_site;
    Ig3DBaseModel	*m_model;
    Ig3DBaseBrowser	*m_browser;
    IgControlCentre	*m_centre;
    IgCommonViewPropertiesCategory 	*m_viewPropertiesCategory;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_BASE_WINDOW_IG_3D_BASE_WINDOW_H
