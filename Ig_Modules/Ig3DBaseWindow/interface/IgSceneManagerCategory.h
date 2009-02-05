#ifndef IG_3D_BASE_WINDOW_IG_SCENE_MANAGER_CATEGORY_H
# define IG_3D_BASE_WINDOW_IG_SCENE_MANAGER_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DBaseWindow/interface/IgCommonViewCategory.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <qcolor.h>
# include <Inventor/SbColor.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class Ig3DBaseBrowser;
class QCheckBox;
class QFrame;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_BASE_WINDOW_API IgSceneManagerCategory
    : public IgCommonViewCategory,
      public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgSceneManagerCategory);    
    Q_OBJECT
public:
    IgSceneManagerCategory (IgState *state);
    ~IgSceneManagerCategory ();

    virtual void		browserChanged 	(Ig3DBaseBrowser *browser);    
  
private slots:
    virtual void		setBackgroundColor	(void);
    virtual void		setBackgroundColor	(const SbColor& color);
    virtual void		setRGBMode		(bool option);
    virtual void		setRealTimeUpdate	(bool option);
    
private:
    void			addSceneManagerTab 	(void);
    
    QFrame			*m_background;    
    QCheckBox			*m_rgbMode;
    QCheckBox			*m_realTimeUpdate;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_BASE_WINDOW_IG_SCENE_MANAGER_CATEGORY_H
