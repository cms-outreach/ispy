#ifndef IG_LEGO_BROWSER_IG_LEGO_BROWSER_H
# define IG_LEGO_BROWSER_IG_LEGO_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgLegoBrowser/interface/config.h"
# include "Ig_Modules/Ig3DBaseBrowser/interface/Ig3DBaseBrowser.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgLegoModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_LEGO_BROWSER_API IgLegoBrowser : public Ig3DBaseBrowser
{
    Q_OBJECT
public:
    IgLegoBrowser (IgState *state, IgSite *site, IgLegoModel *model = 0);

    static const char *		catalogLabel (void);

public slots:
    virtual void 		viewPlaneX (void);
    virtual void 		viewPlaneY (void);
    virtual void 		viewPlaneZ (void);    
    virtual void 		toggleCameraType (void);
    virtual void		invertCamera (void);    

protected:
    virtual void 		buildDecoration (QWidget *parent);    
    virtual QWidget 		*buildLeftTrim (QWidget *parent);
    virtual QWidget 		*buildBottomTrim (QWidget *parent);
    virtual QWidget 		*buildRightTrim (QWidget *parent);

private:
    virtual void		init (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_LEGO_BROWSER_IG_LEGO_BROWSER_H
