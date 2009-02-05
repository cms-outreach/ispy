#ifndef IG_QT_BROWSER_IG_QWIDGET_STACK_SITE_H
# define IG_QT_BROWSER_IG_QWIDGET_STACK_SITE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include "Ig_Modules/IgQtBrowser/interface/IgQtSiteStack.h"
# include <qobject.h>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;
class QWidgetStack;
class QVBox;
class QComboBox;
class QLayout;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQWidgetStackSite : public QObject, public IgQtSiteStack 
{
    Q_OBJECT
public:
    IgQWidgetStackSite (IgState *state, IgSite *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    static const char *	catalogLabel (void);
    IgSite 		*hostWithName (const std::string &name);    
protected:
    virtual QWidget *	self (void);
    virtual QWidget *	host (void);
    virtual void	host (QWidget *child);
    virtual void	cascade (void);
    virtual void 	tile (void);    
private:
    QVBox		*m_box;
    QComboBox		*m_combo;    
    QWidgetStack	*m_widget;
    QLayout		*m_layout;    
    std::map<int, int>	m_comboIndexToWidgetID;
    std::string		m_currentName;    
    IgSite *		m_parent;
public slots:
// FIXME: this is a kludge to allow correct behaviour when someone
// selects a widget from the combo box.
void		switchToWidget (int id);    

};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QWIDGET_STACK_SITE_H
