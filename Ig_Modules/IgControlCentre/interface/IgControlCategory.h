#ifndef IG_CONTROL_CENTRE_IG_CONTROL_CATEGORY_H
# define IG_CONTROL_CENTRE_IG_CONTROL_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgControlCentre/interface/IgControlItem.h"
# include <qpixmap.h>
# include <vector>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class QFrame;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QCheckBox;
class QToolButton;
class QComboBox;
class QWidget;
class IgQtScrollView;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONTROL_CENTRE_API IgControlCategory : public IgControlItem
{
    Q_OBJECT
public:
    enum {
	/** Constructor flag to indicate this control category manages
	    multiple objects, and should have a combobox in the
	    #topArea() to choose among them.  The pane in the
	    #bodyArea() below the combo box shows the properties of
	    the currently selected object. */
	MultiItem = 1,
	
	/** Constructor flag to indicate that a #MultiItem category
	    also wants an interface to create, delete and rename the
	    items.  */
	ManageItems = 2,

	/** Constructor flag to indicate that this control category
	    provides an option to automatically track changes made in
	    the GUI and to apply them automatically.  This causes a
	    checkbox to be added to the #bottomArea().  Toggling the
	    checkbox causes #tracking() signal to be sent; subclasses
	    should connect that signal to the slots of the relevant
	    control items (see #IgControlBase::track()).  Creating the
	    connection causes the #IgControlBase::apply() methods to
	    be invoked for control items whose values are changing
	    while change monitoring is on.  */
	MonitorChanges = 4,
	
	/** Constructor flag to indicate that this control category
	   provides a splitter area. */
	SplitterArea = 8
    };
	    
    IgControlCategory (IgState *state, const QString& name, int flags = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    IgState *state (void) {return m_state;}
    const QString&	  getHelpURL (void) const;
    const QPixmap&	  getIcon (void) const;
    
    // Management for multi-item panes
    virtual void	  clearCategory (void);
    virtual void	  clearBodyArea (void);
    virtual void	  clearLeftArea (void);
    virtual int		  items (void) const;
    virtual int		  currentItem (void) const;
    virtual const QString currentText (void) const;
    virtual void	  setCurrentItem (int index);
    virtual void	  addItem (QString label, bool editable = true);
    virtual void	  removeItem (int index);
    virtual bool	  isItemEditable (int index) const;
    virtual void	  setItemEditable (int index, bool editable = true);    

    // Body content management
    virtual QWidget *	panel (void);
    virtual QWidget *	topArea (void);
    virtual QWidget *	bodyArea (void);
    virtual QWidget *	leftArea (void);
    virtual QWidget *	rightArea (void);
    virtual QWidget *	bottomArea (void);

    virtual QHBoxLayout *	panelLayout (void);
    virtual QVBoxLayout *	topLayout (void);
    virtual QGridLayout *	bodyLayout (void);
    virtual QVBoxLayout *	leftLayout (void);
    virtual QVBoxLayout *	rightLayout (void);
    virtual QVBoxLayout *	bottomLayout (void);


    // Body sub-structure management
    QWidget *		makeIndentedLabel (QString label, QWidget *area); 
    void		pushIndent (void);
    void		popIndent (void);
    
    void 		removeableItem (bool option);    
    
    // State methods
    virtual bool	isTracking (void) const;
    virtual void	setTracking (bool);
    
    void		hideBodyScrollArea (void) const;
    void		showBodyScrollArea (void) const;
    
    // Register to context service
    virtual void 	registerMe (IgState *state);    

    using IgControlItem::setDirty;

public slots:
    virtual void	setDirty (bool value);
    virtual void	track (bool value);

    virtual void	attach (void);
    virtual void	detach (void);

    virtual void	update (void);
    virtual void	apply (void);
    virtual void	revert (void);

    virtual void	setShown (bool show);
    virtual void	show (void);    
    virtual void	hide (void);
    virtual void	focusIn (void);
    virtual void	focusOut (void);

signals:
    void		tracking (bool mode);
    void		empty    (bool status);

protected slots:
    virtual void	itemChanged (int newItem);
    virtual void	createNewItem (void);
    virtual void	deleteItem (void);
    virtual void	renameItem (const QString &newName);

protected:
    void		setHelpURL (const QString &url);
    void		setIcon (const QPixmap& icon);
    
private:
    IgState		*m_state;    
    
    QFrame		*m_panel;
    QWidget		*m_leftArea;
    QWidget		*m_rightArea;
    QWidget		*m_bodyArea;
    IgQtScrollView		*m_rightScrollArea;
    
    QVBoxLayout		*m_topLayout;
    QGridLayout		*m_bodyLayout;
    QVBoxLayout		*m_bottomLayout;
    
    QCheckBox		*m_tracker;
    QComboBox		*m_items;
    QToolButton		*m_newItem;
    QToolButton		*m_deleteItem;
    int			m_current;
    std::vector<bool>	m_editable;
    int			m_indentLevel;
    int			m_flags;
    QString		m_helpURL;
    QPixmap		m_icon;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONTROL_CENTRE_IG_CONTROL_CATEGORY_H
