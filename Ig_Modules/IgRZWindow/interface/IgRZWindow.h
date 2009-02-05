#ifndef IG_RZ_WINDOW_IG_RZ_WINDOW_H
# define IG_RZ_WINDOW_IG_RZ_WINDOW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgRZWindow/interface/config.h"
# include "Ig_Modules/Ig3DBaseWindow/interface/Ig3DBaseWindow.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;
class IgRZViewPropertiesCategory;
class Ig3DLightsCategory;
class IgQtObjectMenuMessage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_RZ_WINDOW_API IgRZWindow : public Ig3DBaseWindow
{
    Q_OBJECT
public:
    IgRZWindow (IgPage *page);
    ~IgRZWindow (void);
    // implicit copy constructor
    // implicit assignment operator

    static const int    MENU_RZ_MANIP	= 119;

    virtual void		initCategories (void);
    virtual void 		initMenu (void);
    virtual void 		initToolBar (void);    
    virtual void 		focusIn (void);
    virtual void 		focusOut (void);

private slots:

    void			showLights (void);
    void	 		showRZViewProperties (void);    

private:
    void			repMenu (IgQtObjectMenuMessage message);

    IgRZViewPropertiesCategory *m_rzViewPropertiesCategory;
    Ig3DLightsCategory	 *m_lightsCategory;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_RZ_WINDOW_IG_RZ_WINDOW_H
