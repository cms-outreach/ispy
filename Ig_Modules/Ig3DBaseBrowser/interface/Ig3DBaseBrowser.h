#ifndef IG_3D_BASE_BROWSER_IG3D_BROWSER_H
# define IG_3D_BASE_BROWSER_IG3D_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DBaseBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgBrowser.h"
# include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
# include <qobject.h>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgSite;
class IgSelectionMessage;
class IgQtObjectMenuMessage;

class Ig3DViewpoints;

class Ig3DBaseModel;
class Ig3DBaseModelEvent;
class Ig3DBaseRep;

class SoPickedPoint;
class SoPath;
class SoSelection;
class SoGroup;
class SoFieldSensor;

class QString;
class QPopupMenu;
class QEvent;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_BASE_BROWSER_API Ig3DBaseBrowser : public QObject,
					       public SoQtExaminerViewer,
					       public IgBrowser
{
    Q_OBJECT

public:
    static const int	MENU_ID_3D		= 3000;
    static const int	MENU_ID_3D_FILESEP	= 3003;
    static const int	MENU_ID_3D_TOOLBAR	= 3004;
    static const int	MENU_ID_3D_TOOLBAR_SEP	= 3005;
    static const int	MENU_ID_3D_VIEW_PROPERTIES= 3006;
    static const int	MENU_ID_3D_MISCSEP	= 3007;
    static const int	MENU_ID_3D_Z_SLICE	= 3008;
    static const int	MENU_ID_3D_CLIPS	= 3009;
    static const int	MENU_ID_3D_LIGHTS	= 3010;
    static const int	MENU_ID_3D_LGHT_MANIP	= 3101;
    static const int	MENU_ID_3D_LGHT_NEW	= 3102;
    static const int	MENU_ID_3D_LGHT_NEW_DIR	= 3103;
    static const int	MENU_ID_3D_LGHT_NEW_PNT	= 3104;
    static const int	MENU_ID_3D_LGHT_NEW_SPT	= 3105;
    static const int	MENU_ID_3D_LGHT_SEP	= 3110;
    static const int	MENU_ID_3D_LGHT_BASE	= 3113;
    static const int	MENU_ID_3D_ANIMS	= 3011;
    static const int 	MENU_ID_3D_2D_VIEW_PROPERTIES = 3014;
    static const int	MENU_ID_3D_VIEWPOINTS	= 3012;
    static const int	MENU_ID_3D_FIELDPLANES	= 3015;
    static const int	MENU_ID_3D_NODEMANIP	= 3016;
    static const int	MENU_ID_3D_VP_NEW	= 3201;
    static const int	MENU_ID_3D_VP_SEP	= 3202;
    static const int	MENU_ID_3D_VP_BASE	= 3203;
    static const int 	MENU_ID_3D_LEGO_VIEW_PROPERTIES = 3112;
    static const int 	MENU_ID_3D_RZ_VIEW_PROPERTIES = 3113;
    static const int 	MENU_ID_3D_RPHI_VIEW_PROPERTIES = 3114;

    static const int	MENU_3D_OPS		= 100;
    static const int	MENU_SEEKTO		= 101;
    static const int	MENU_SLICE		= 102;
    static const int	MENU_CLIP		= 103;


    Ig3DBaseBrowser (IgState *state, IgSite *site, Ig3DBaseModel *model);
    ~Ig3DBaseBrowser (void);

    void  			setAutoClipping (SbBool enable);

    virtual void		browse (IgRepresentable *object);

    virtual void		select (Ig3DBaseRep *rep);
    virtual Ig3DBaseRep *	getCurrentPick (void) const;
    virtual Ig3DBaseRep *	getSelection (void) const;
    virtual const SoPickedPoint *getCurrentPickPoint (void) const;

    virtual Ig3DBaseModel *	model (void) const;
    virtual IgState *		state (void) const;

    virtual void		printBitmap (QString file, float ppi,
    					     float dpi, QString format);
    virtual void		printVector (QString file, QString format, int level);
    Ig3DBaseRep			*menuRep (void);
    void			repMenu (IgQtObjectMenuMessage message);
    int				getGL2PSOptions (void);
    bool                        isGridVisible (void);
    bool			isWhatsThisPicking (void);
    virtual void		focusIn (void);
    virtual void		focusOut (void);
    
    static bool			saveNode (SoNode *node, const QString& title,
                                          QWidget *parent = 0, const char* file = 0);
    static bool			writeNode (SoNode *node, const QString& file, bool binary,
                                          QWidget *parent = 0);
    static SoNode*              openNode (const QString& nodeName, const QString& title,
                                          QWidget *parent = 0, const char* file = 0);
    static SoNode*              findGroup (SoNode *node, const char* name);

public slots:
    void                        zoomIn (void);
    void                        zoomOut (void);
    void                        zoom (const float diffvalue);
    void			resetToHomePosition (void);
    void			saveHomePosition (void);
    void			viewAll (void);
    void			seek (void);    
    void                        setFeedbackVisibility (bool enable);
    virtual void                setGridVisibility (bool enable);
    void			view (void);
    void 			pick (void);
    void			setWhatsThisPicking (bool enable = true);
    void			setGL2PSOptions (int options);
    
    virtual void                autoPrint (void);
    virtual void		viewPlaneX (void);
    virtual void		viewPlaneY (void);
    virtual void		viewPlaneZ (void);
    virtual void		toggleCameraType (void);
    virtual void		invertCamera (void);    
    
protected:
    
    virtual void		initWidget (IgSite *site);
        
    virtual void		selectMessage (IgSelectionMessage message);
    virtual void		modelChanged (Ig3DBaseModelEvent event);
    virtual void		setCurrentPick (Ig3DBaseRep *rep);
    virtual void		setCurrentPickPoint (const SoPickedPoint *pick);
    //! Overloaded from parent class to add the tool tips
    virtual void 		createViewerButtons (QWidget* parent, SbPList* buttonlist);
    virtual QWidget *		buildLeftTrim (QWidget *parent);
    virtual QWidget *		buildBottomTrim (QWidget *parent);
    virtual QWidget *		buildRightTrim (QWidget *parent);

private slots:
    void			save (void);
    void			print (void);

    void			repSeekTo (void);

    void			leftWheelPressed (void);
    void			leftWheelChanged (float by);
    void			leftWheelReleased (void);

signals:
    void			cameraToggled (void);

protected:
    static QWidget *		initialise (IgState *state, IgSite *site);
    static SbBool 		eventCallback (void *closure, QEvent *event);

private:

    static void			onStartPick (void *cb, SoQtViewer *viewer);
    static void			onFinishPick (void *cb, SoQtViewer *viewer);
    static SoPath *		onPick (void *cb, const SoPickedPoint *pick);
    static void			onSelect (void *cb, SoPath *selection);
    static void			onDeselect (void *cb, SoPath *selection);
    
    //FIXME: remove these Callback method once the SoQt fixes the continoius
    //rendering problem
    static void			farDistanceSensorCB (void *me, SoSensor *sensor);
    static void			nearDistanceSensorCB (void *me, SoSensor *sensor);

    virtual void                drawGrid (const bool enable);

    IgState		       	*m_state;
    IgSite			*m_site;
    Ig3DBaseModel		*m_model;
    QPopupMenu			*m_menu;
    Ig3DBaseRep			*m_selection;
    Ig3DBaseRep			*m_menuRep;
    const SoPickedPoint		*m_pick;
    bool			 m_first_time;
    bool			 m_grid;
    int				 m_gl2psOptions;
    Ig3DBaseRep		 	*m_currentPick;
    bool			m_whatsThisPicking;
    bool			m_oldView;
    bool			m_oldSeek;
    int				m_gl2psFBBufferSize;
    
    //FIXME: remove these two sensors once the SoQt fixes the continoius
    //rendering problem
    SoFieldSensor		*m_farDistanceSensor;
    SoFieldSensor		*m_nearDistanceSensor;
    
    // undefined semantics
    Ig3DBaseBrowser (const Ig3DBaseBrowser &);
    Ig3DBaseBrowser &operator= (const Ig3DBaseBrowser &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_BASE_BROWSER_IG3D_BROWSER_H
