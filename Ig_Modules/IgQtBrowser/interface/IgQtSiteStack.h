#ifndef IG_QT_BROWSER_IG_QT_SITE_STACK_H
# define IG_QT_BROWSER_IG_QT_SITE_STACK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"
# include <qstring.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQtSiteStack : public IgQtSite
{
public:
    IgQtSiteStack (void);
    QString currentName (void);    
    IgQtSiteStack	*hostWithName (const QString &name);    
    virtual void	tile (void);
    virtual void	cascade (void);    
private:
    QString 		m_currentName;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_SITE_STACK_H
