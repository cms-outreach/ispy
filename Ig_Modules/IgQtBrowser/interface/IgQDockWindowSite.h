#ifndef IG_QT_BROWSER_IG_QDOCK_WINDOW_SITE_H
# define IG_QT_BROWSER_IG_QDOCK_WINDOW_SITE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;
class IgSite;
class QDockWindow;
class QGridLayout;


//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQDockWindowSite : public IgQtSite
{
public:
    IgQDockWindowSite (IgState *state, IgSite *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected:
    IgQDockWindowSite (IgSite *parent, QDockWindow *widget);

    virtual QWidget *	self (void);
    virtual QWidget *	host (void);
    virtual void	host (QWidget *child);


    static const char *	catalogLabel (void);
private:
    QDockWindow		*m_widget;    
    QGridLayout		*m_layout;    
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QDOCK_WINDOW_SITE_H
