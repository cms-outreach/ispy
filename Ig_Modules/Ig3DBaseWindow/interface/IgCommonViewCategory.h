#ifndef IG_3D_BASE_WINDOW_IG_COMMON_VIEW_CATEGORY_H
# define IG_3D_BASE_WINDOW_IG_COMMON_VIEW_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DBaseWindow/interface/config.h"
# include "Ig_Modules/IgControlCentre/interface/IgControlCategory.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class Ig3DBaseBrowser;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_BASE_WINDOW_API IgCommonViewCategory
    : public IgControlCategory
{
    Q_OBJECT
public:
    IgCommonViewCategory (IgState *state, const QString& name);

    virtual void		browserChanged 	(Ig3DBaseBrowser *browser);
    virtual void		registerBrowser (IgState *state,
                                                 Ig3DBaseBrowser *browser);    
public slots:
    virtual void        	show 			(void);
  
protected:
    Ig3DBaseBrowser	       	*m_source;
        
private:
    // undefined semantics
    IgCommonViewCategory (const IgCommonViewCategory &);
    IgCommonViewCategory &operator= (const IgCommonViewCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_BASE_WINDOW_IG_COMMON_VIEW_CATEGORY_H
