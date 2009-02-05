#ifndef IG_RPHI_WINDOW_IG_RPHI_WINDOW_H
# define IG_RPHI_WINDOW_IG_RPHI_WINDOW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgRPhiWindow/interface/config.h"
# include "Ig_Modules/Ig3DBaseWindow/interface/Ig3DBaseWindow.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;
class IgRPhiViewPropertiesCategory;
class Ig3DLightsCategory;
class IgQtObjectMenuMessage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_RPHI_WINDOW_API IgRPhiWindow : public Ig3DBaseWindow
{
    Q_OBJECT
public:
    IgRPhiWindow (IgPage *page);
    ~IgRPhiWindow (void);
    // implicit copy constructor
    // implicit assignment operator

    static const int    MENU_RPHI_MANIP	= 118;

    virtual void		initCategories (void);
    virtual void 		initMenu (void);
    virtual void 		initToolBar (void);    
    virtual void 		focusIn (void);
    virtual void 		focusOut (void);

private slots:

    void			showLights (void);
    void	 		showRPhiViewProperties (void);    

private:
    void			repMenu (IgQtObjectMenuMessage message);

    IgRPhiViewPropertiesCategory *m_rphiViewPropertiesCategory;
    Ig3DLightsCategory	 	*m_lightsCategory;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_RPHI_WINDOW_IG_RPHI_WINDOW_H
