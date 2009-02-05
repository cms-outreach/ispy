#ifndef IG_2D_BROWSER_IG_2D_BROWSER_H
# define IG_2D_BROWSER_IG_2D_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig2DBrowser/interface/config.h"
# include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig2DModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_2D_BROWSER_API Ig2DBrowser : public Ig3DBaseBrowser
{
    Q_OBJECT

public:
    Ig2DBrowser (IgState *state, IgSite *site, Ig2DModel *model = 0);

    static const char *		catalogLabel (void);

public slots:
    virtual void 		viewPlaneX (void);
    virtual void 		viewPlaneY (void);
    virtual void 		viewPlaneZ (void);

protected:
    static SbBool 		eventCallback (void *closure, QEvent *event);
    
    virtual void buildDecoration (QWidget *parent);    
    virtual QWidget *buildLeftTrim (QWidget *parent);
    virtual QWidget *buildBottomTrim (QWidget *parent);
    virtual QWidget *buildRightTrim (QWidget *parent);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_2D_BROWSER_IG_2D_BROWSER_H
