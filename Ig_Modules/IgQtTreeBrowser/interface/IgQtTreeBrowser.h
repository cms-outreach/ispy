#ifndef IG_QT_TREE_BROWSER_IG_QT_TREE_BROWSER_H
# define IG_QT_TREE_BROWSER_IG_QT_TREE_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtTreeBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgBrowser.h"
# include <qlistview.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgSite;
class IgState;
class IgQtTreeRep;
class QListViewItem;
class QPoint;
class IgSelectionMessage;
class QResizeEvent;
class QMouseEvent;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_TREE_BROWSER_API IgQtTreeBrowser : public QListView,
					       public IgModel,
					       public IgBrowser
{
    Q_OBJECT
public:
    IgQtTreeBrowser (IgState *state, IgSite *site);
    ~IgQtTreeBrowser (void);
    
    virtual void	browse (IgRepresentable *object);
    
    virtual void	select (IgQtTreeRep *rep);

    static const char *	catalogLabel (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
public slots:
    // virtual void	checkSelected (void);
    // virtual void	updateTree (void);
    // virtual void	printTrees (void);
    virtual void	onSelectionChange (void);
    virtual void	slotItemChanged (void);
    virtual void	slotItemRMClicked (void);
    virtual void	contextMenu (QListViewItem *item, 
				     const QPoint &pos,
				     int col);

protected:
    // Browser messages
    virtual void	selectMessage (IgSelectionMessage message);

    virtual void	resizeEvent (QResizeEvent *e);
    virtual void	contentsMouseDoubleClickEvent (QMouseEvent *e);
    virtual void	contentsMousePressEvent (QMouseEvent *e);
    virtual void	contentsMouseReleaseEvent (QMouseEvent *e);


private:
    QListViewItem *	getNextUpItem (QListViewItem *item);
    QListViewItem *	getNextItem (QListViewItem *item);
    QList<IgQtTreeRep> *getSelectedItems (void);

    IgState		*m_state;
    bool		m_discardRelease;

    // undefined semantics
    IgQtTreeBrowser (const IgQtTreeBrowser &);
    IgQtTreeBrowser &operator= (const IgQtTreeBrowser &);
    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_TREE_BROWSER_IG_QT_TREE_BROWSER_H
