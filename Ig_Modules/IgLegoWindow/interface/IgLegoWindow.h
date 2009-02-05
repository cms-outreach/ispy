#ifndef IG_LEGO_WINDOW_IG_LEGO_WINDOW_H
# define IG_LEGO_WINDOW_IG_LEGO_WINDOW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgLegoWindow/interface/config.h"
# include "Ig_Modules/Ig3DBaseWindow/interface/Ig3DBaseWindow.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;
class IgLegoViewPropertiesCategory;
class Ig3DLightsCategory;
class IgQtObjectMenuMessage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_LEGO_WINDOW_API IgLegoWindow : public Ig3DBaseWindow
{
    Q_OBJECT
public:
    IgLegoWindow (IgPage *page);
    ~IgLegoWindow (void);
    // implicit copy constructor
    // implicit assignment operator

    static const int    MENU_LEGO_MANIP	= 117;

    virtual void		initCategories (void);
    virtual void 		initMenu (void);
    virtual void 		initToolBar (void);    
    virtual void 		focusIn (void);
    virtual void 		focusOut (void);

private slots:

    void			showLights (void);
    void	 		showLegoViewProperties (void);    

private:
    void			repMenu (IgQtObjectMenuMessage message);

    IgLegoViewPropertiesCategory *m_legoViewPropertiesCategory;
    Ig3DLightsCategory	 *m_lightsCategory;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_LEGO_WINDOW_IG_LEGO_WINDOW_H
