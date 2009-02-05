#ifndef IG_QT_BROWSER_IG_QWORKSPACE_SITE_H
# define IG_QT_BROWSER_IG_QWORKSPACE_SITE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/IgQtSiteStack.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgQtWorkspace;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQWorkspaceSite : public IgQtSiteStack
{
public:
    IgQWorkspaceSite (IgState *state, IgSite *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    static const char *	catalogLabel (void);
    virtual void	tile (void);
    virtual void	cascade (void);    
    IgQtSite		*hostWithName (const std::string &name);
protected:
    virtual QWidget *	self (void);
    virtual QWidget *	host (void);
    virtual void	host (QWidget *child);
    
private:
    IgQtWorkspace	*m_widget;
    std::string		m_currentName;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QWORKSPACE_SITE_H
