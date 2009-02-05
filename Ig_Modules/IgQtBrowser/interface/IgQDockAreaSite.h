#ifndef IG_QT_BROWSER_IG_QDOCK_AREA_SITE_H
# define IG_QT_BROWSER_IG_QDOCK_AREA_SITE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class QDockArea;
class QWidget;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQDockAreaSite : public IgQtSite
{
public:
    IgQDockAreaSite (IgState *state, IgSite *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected:
    IgQDockAreaSite (IgSite *parent, QDockArea *widget);

    virtual QWidget *	self (void);
    virtual QWidget *	host (void);
    virtual void	host (QWidget *child);


    static const char *	catalogLabel (void);
private:
    QDockArea		*m_widget;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QDOCK_AREA_SITE_H
