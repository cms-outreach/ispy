#ifndef IG_QT_TWIG_BROWSER_IG_QT_TWIG_BROWSER_H
# define IG_QT_TWIG_BROWSER_IG_QT_TWIG_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtTwigBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgBrowser.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
# include <qlistview.h>
# include <qlist.h>
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgSite;
class IgSelectionMessage;
class IgQtTwigRep;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_TWIG_BROWSER_API IgQtTwigBrowser : public QListView,
					       public IgBrowser,
					       public IgModel
{
    Q_OBJECT
public:
    IgQtTwigBrowser (IgState *state, IgSite *site);
    ~IgQtTwigBrowser (void);

    virtual void	browse (IgRepresentable *object);

    // FIXME: need this to show only partial twig tree?
    // virtual void	setRootTwig (IgQtTwig *twig);

    virtual void	select (IgQtTwigRep *rep);

    static const char *	catalogLabel (void);

    //! Return the list of currently checked twigs
    //QList<IgQtTwigRep*> checkedItems (void);
public slots:
    // virtual void	checkSelected (void);
    // virtual void	updateTree (void);
    // virtual void	printTrees (void);
    virtual void	onSelectionChange (void);
    virtual void	slotItemChanged (void);
    virtual void	slotItemRMClicked (void);
    virtual void	contextMenu (QListViewItem *item, const QPoint &pos,
				     int col);

protected:
    // Browser messages
    virtual void	selectMessage (IgSelectionMessage message);

    virtual void	resizeEvent (QResizeEvent *e);
    virtual void	contentsMouseDoubleClickEvent (QMouseEvent *e);
    virtual void	contentsMousePressEvent (QMouseEvent *e);
    virtual void	contentsMouseReleaseEvent (QMouseEvent *e);

    int			isInsideChecker (int x);

private:
    QListViewItem *	getNextUpItem (QListViewItem *item);
    QListViewItem *	getNextItem (QListViewItem *item);
    QList<IgQtTwigRep> *getSelectedItems (void);
    QList<IgQtTwigRep> *getCheckedItems (int column);

    IgState		*m_state;
    bool		m_discardRelease;

    // undefined semantics
    IgQtTwigBrowser (const IgQtTwigBrowser &);
    IgQtTwigBrowser &operator= (const IgQtTwigBrowser &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_TWIG_BROWSER_IG_QT_TWIG_BROWSER_H
