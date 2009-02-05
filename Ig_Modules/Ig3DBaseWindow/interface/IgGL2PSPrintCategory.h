#ifndef IG_3D_BASE_WINDOW_IG_GL2PS_PRINT_CATEGORY_H
# define IG_3D_BASE_WINDOW_IG_GL2PS_PRINT_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DBaseWindow/interface/IgCommonViewCategory.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QCheckBox;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_BASE_WINDOW_API IgGL2PSPrintCategory
    : public IgCommonViewCategory,
      public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgGL2PSPrintCategory);    
    Q_OBJECT
public:
    IgGL2PSPrintCategory (IgState *state);
    ~IgGL2PSPrintCategory ();

    virtual void		browserChanged 	(Ig3DBaseBrowser *browser);
  
private slots:
    virtual void		updateGL2PSOptions	(void);
    virtual void		setGL2PSOptions		(void);

private:
    void			addGL2PSOptionTab 	(void);

    QCheckBox  			*m_gl2psSimpleLineOffset;
    QCheckBox  			*m_gl2psBestRoot;
    QCheckBox  			*m_gl2psOcclusionCulling;
    QCheckBox 			*m_gl2psBackGround;   
    QCheckBox  			*m_gl2psLandScape;
    static bool			updated;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_BASE_WINDOW_IG_GL2PS_PRINT_CATEGORY_H
