#ifndef IG_QT_BROWSER_IG_QTOOL_BOX_SITE_H
# define IG_QT_BROWSER_IG_QTOOL_BOX_SITE_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Modules/IgQtBrowser/interface/IgQtSiteStack.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class QWidget;
class QToolBox;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQToolBoxSite : public IgQtSiteStack 
{
public:
    IgQToolBoxSite (IgState *state, IgQtSite *parent);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    static const char *	catalogLabel (void);
    IgQtSite		*hostWithName (const std::string &name);

protected:
    virtual QWidget *	self (void);
    virtual QWidget *	host (void);
    virtual void	host (QWidget *child);

private:
    QToolBox * 		m_widget;
    std::string		m_currentName;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QTOOL_BOX_SITE_H
