#ifndef IG_3D_CATEGORIES_IG_3D_VIEWPOINTS_CATEGORY_H
# define IG_3D_CATEGORIES_IG_3D_VIEWPOINTS_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DCategories/interface/config.h"
# include "Ig_Modules/Ig3DCategories/interface/Ig3DObjectCategory.h"
# include "Ig_Extensions/IgOpenInventor/interface/IgSoViewpoint.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <qstring.h>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DBaseRep;
class Ig3DBaseBrowser;
class SoGroup;
class Ig3DCameraRotControl;
class Ig3DFloatControl;
class QCheckBox;
class IgQtAxisAlignment;

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CATEGORIES_API Ig3DViewpointsCategory
    : public Ig3DObjectCategory<IgSoViewpoint>,
      public IgStateElement
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (Ig3DViewpointsCategory);    
public:
    Ig3DViewpointsCategory (IgState *state,
                            Ig3DBaseBrowser *source);
    ~Ig3DViewpointsCategory ();

    void		reconstruct (SoGroup *from);
    void		browserChanged  (Ig3DBaseBrowser *browser);
    
    virtual void	attach (void);
    virtual void	detach (void);

    virtual void	update (void);
    virtual void	apply (void);

    virtual void	show (void);
    virtual void 	focusIn (void);
    virtual void 	focusOut (void);
    virtual void	newCamera (void);

    virtual void	registerBrowser (IgState *state,
                                         Ig3DBaseBrowser *browser);
    static void		setViewpoint (Ig3DBaseBrowser *browser,
                                      IgSoViewpoint *view);
    static void		setCamera    (Ig3DBaseBrowser *browser,
                                      IgSoViewpoint *view);
protected slots:
    virtual void	zoom (void);
    virtual void	autoClipping (void);

protected:
    virtual void	itemChanged (int index);
    virtual void	createNewItem (void);
    virtual void	deleteItem (void);
    virtual void	renameItem (const QString &newName);

private slots:
    virtual void	makeOne 	(QString name = "");
    virtual void 	deleteOne 	(void);
    virtual void 	updateCameraPosition (bool tracking);
    virtual void 	updateCameraPosition (void);
    virtual void	alignToAxis (void);
    virtual void	save (void);    
    virtual void	load (void);    

private:
    bool		save (QString file, bool binary);    
    void		connectViewpointFromCamera	(IgSoViewpoint *obj);
    void		disconnectViewpoint 		(IgSoViewpoint *obj);
    void		connectCameraFromViewpoint	(IgSoViewpoint *obj);
    void		disconnectCamera 		(void);
    
    Ig3DBaseBrowser	*m_source;
    SoGroup		*m_group;

    int			m_previous;
    Ig3DCameraRotControl*m_orientation;
    IgQtAxisAlignment	*m_alignment;
    Ig3DFloatControl	*m_focalDistance;
    Ig3DFloatControl	*m_aspectRatio;
    Ig3DFloatControl	*m_nearDistance;
    Ig3DFloatControl	*m_farDistance;
    Ig3DFloatControl	*m_zoom;
    QCheckBox		*m_autoClipping;
    
    std::vector<bool>	m_autoClippings;
    
    // undefined semantics
    Ig3DViewpointsCategory (const Ig3DViewpointsCategory &);
    Ig3DViewpointsCategory &operator= (const Ig3DViewpointsCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CATEGORIES_IG_3D_VIEWPOINTS_CATEGORY_H
