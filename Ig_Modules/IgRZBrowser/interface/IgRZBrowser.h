#ifndef IG_RZ_BROWSER_IG_RZ_BROWSER_H
# define IG_RZ_BROWSER_IG_RZ_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgRZBrowser/interface/config.h"
# include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRZModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_RZ_BROWSER_API IgRZBrowser : public Ig3DBaseBrowser
{
    Q_OBJECT
public:
    IgRZBrowser (IgState *state, IgSite *site, IgRZModel *model = 0);

    static const char *		catalogLabel (void);

public slots:
    virtual void 		viewPlaneX (void);
    virtual void 		viewPlaneY (void);
    virtual void 		viewPlaneZ (void);    
    virtual void 		toggleCameraType (void);
    virtual void		invertCamera (void);    

protected:
    static SbBool 		eventCallback (void *closure, QEvent *event);
    
    virtual void 		buildDecoration (QWidget *parent);    
    virtual QWidget 		*buildLeftTrim (QWidget *parent);
    virtual QWidget 		*buildBottomTrim (QWidget *parent);
    virtual QWidget 		*buildRightTrim (QWidget *parent);

private:
    virtual void		init (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_RZ_BROWSER_IG_RZ_BROWSER_H
