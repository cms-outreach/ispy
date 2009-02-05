#ifndef IG_QT_BROWSER_IG_QT_SITE_H
# define IG_QT_BROWSER_IG_QT_SITE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgSite.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQtSite : public IgSite
{
public:
    static QWidget *	selfFrom (IgSite *site);
    static QWidget *	hostFrom (IgSite *site);
    static void		host (IgSite *site, QWidget *child);

protected:
    virtual QWidget *	self (void) = 0;
    virtual QWidget *	host (void) = 0;
    virtual void	host (QWidget *child) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_SITE_H
