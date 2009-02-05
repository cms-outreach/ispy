#ifndef IG_STUDIO_IG_PAGE_H
# define IG_STUDIO_IG_PAGE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include "Ig_Modules/IgQtBrowser/interface/IgQHSplitterSite.h"
# include <qobject.h>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgDocument;
class QGridLayout;
class IgView;
class IgQtSiteStack;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_STUDIO_API IgPage : public IgQHSplitterSite
{
public:
    static const int MENU_ID_FILE_NEW = 150;
    static const int MENU_ID_WINDOW = 5000;    
    static const int MENU_ID_WINDOW_TILE = 5001;    
    static const int MENU_ID_WINDOW_CASCADE = 5002;    
    static const int MENU_ID_WINDOW_LIST_OFFSET = 6000;
        
    IgPage (IgDocument *doc);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    IgQtSiteStack	*workspace (void);
    IgQtSite		*leftDockArea (void);    
    IgQtSite		*rightDockArea (void);    
    IgQtSite		*bottomDockArea (void);    
    IgQtSiteStack	*stackArea (void);
    
    virtual IgState *	state (void) const;
    void		registerView (IgView *view, 
				      QWidget *selectableWidget);
private:
    IgDocument		*m_document;
    QGridLayout		*m_layout;
    IgQtSite		*m_leftDockArea;
    IgQtSiteStack	*m_stackArea;
    IgQtSite		*m_centralSplitter;    
    IgQtSiteStack	*m_workspace;    
    IgQtSite		*m_bottomDockArea;
    IgQtSite		*m_rightDockArea;
    
    std::map<QWidget *, IgView*>	m_views;
    int 		m_numRegisteredWindows;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_STUDIO_IG_PAGE_H
