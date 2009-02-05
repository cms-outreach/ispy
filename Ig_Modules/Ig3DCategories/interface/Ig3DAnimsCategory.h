#ifndef IG_3D_CATEGORIES_IG_3D_ANIMS_CATEGORY_H
# define IG_3D_CATEGORIES_IG_3D_ANIMS_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DCategories/interface/config.h"
# include "Ig_Modules/Ig3DCategories/interface/Ig3DObjectCategory.h"
# include "Ig_Extensions/IgOpenInventor/interface/IgSoAnimator.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <qstring.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DBaseRep;
class Ig3DBaseBrowser;
class SoGroup;
class IgSoAnimator;
class QString;

class Ig3DUnitVec3fControl;
class Ig3DFloatControl;
class Ig3DShortControl;
class Ig3DInt32Control;
class Ig3DBoolControl;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_CATEGORIES_API Ig3DAnimsCategory
    : public Ig3DObjectCategory<IgSoAnimator>,
      public IgStateElement
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (Ig3DAnimsCategory);
    
public:
    Ig3DAnimsCategory (IgState *state,
		       Ig3DBaseBrowser *source);
    ~Ig3DAnimsCategory ();

    void		reconstruct 	(void);
    void		browserChanged  (Ig3DBaseBrowser *browser);
    
    virtual void	attach (void);
    virtual void	detach (void);
    virtual void	update (void);
    virtual void	apply (void);
    virtual void	registerBrowser (IgState *state,
                                         Ig3DBaseBrowser *browser);    

    virtual void	setEditable (bool value);

protected:
    virtual void	createNewItem (void);
    virtual void	deleteItem (void);
    virtual void	renameItem (const QString &newName);

private slots:
    virtual void	makeOne 	(QString name = "");
    virtual void 	deleteOne 	(void);

private:
    void		reconstruct 	(SoGroup *from);
    void		buildDefaults 	(void);

    Ig3DBaseBrowser	*m_source;
    
    Ig3DUnitVec3fControl*m_axis;
    Ig3DShortControl	*m_timeMax;
    Ig3DShortControl	*m_timeStep;
    Ig3DFloatControl	*m_timeFreq;
    Ig3DInt32Control	*m_state;
    Ig3DInt32Control	*m_newAxis;	// output signal
    Ig3DBoolControl	*m_complete;	// output signal
    Ig3DBoolControl	*m_on;
    Ig3DBoolControl	*m_reset;

    // undefined semantics
    Ig3DAnimsCategory (const Ig3DAnimsCategory &);
    Ig3DAnimsCategory &operator= (const Ig3DAnimsCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_CATEGORIES_IG_3D_ANIMS_CATEGORY_H
