#ifndef IG_3D_BASE_WINDOW_IG_GL_RENDER_CATEGORY_H
# define IG_3D_BASE_WINDOW_IG_GL_RENDER_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DBaseWindow/interface/IgCommonViewCategory.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QCheckBox;
class QComboBox;
class IgQtRangeControlUInt;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_BASE_WINDOW_API IgGLRenderCategory
    : public IgCommonViewCategory,
      public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgGLRenderCategory);    
    Q_OBJECT
public:
    IgGLRenderCategory (IgState *state);
    ~IgGLRenderCategory ();

    virtual void		browserChanged 	(Ig3DBaseBrowser *browser);    

private slots:
    //GL Render Action
    virtual void		setTransparencyType	(int type);
    virtual void		updateTransparencyType  (int type);
    virtual void		setRedrawPriority	(unsigned int priority);
    virtual void		setAntialiasingPasses	(unsigned int passes);
    virtual void		setAntialiasing		(bool option);
    virtual void		setAutoRedraw		(bool option);
    virtual void		setPassUpdate		(bool option);
    
private:
    void			addGLRenderTab 		(void);
    void			updateTransparencyTypeHelp(void);
    
    QComboBox			*m_transparencyType;
    IgQtRangeControlUInt	*m_redrawPriority;
    IgQtRangeControlUInt	*m_antialiasingPasses;
    QCheckBox			*m_antialiasing;
    QCheckBox			*m_autoRedraw;
    QCheckBox			*m_passUpdate;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_BASE_WINDOW_IG_GL_RENDER_CATEGORY_H
