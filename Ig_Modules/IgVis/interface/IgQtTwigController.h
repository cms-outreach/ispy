#ifndef IG_CMSCAN_IG_QT_TWIG_CONTROLLER_H
# define IG_CMSCAN_IG_QT_TWIG_CONTROLLER_H

#include "Ig_Modules/IgVis/interface/config.h"
# include "Ig_Extensions/IgQt/interface/AdvListView.h"
# include <qlist.h>
# include <vector>

class IgQtTwig;
class IgQtTwigListViewItem;

/*!	\class IgQtTwigController IgQtTwigController.h Ig_Modules/IgVis/interface/IgQtTwigController.h
 *
 *  \brief Provides a graphical control for the IgQtTwig class.
 *
 *  \author I. Gaponenko
 */

class IG_VIS_API IgQtTwigController : public AdvListView
{
    typedef std::vector<IgQtTwig *>	List_of_Twigs;
    Q_OBJECT

public:
	//! creates a twig controller with widget as Qt parent and name label
    explicit IgQtTwigController (QWidget *widget, const char *label=NULL);
	//! adds a (root) twig to the list
    void		setRootTwig (IgQtTwig *t);
	//! which items are currently checked
    QList<IgQtTwigListViewItem>* getCheckedItems (IgQtTwigController *lv);

public slots:

    void		visibilityToggle (void);
    void		updateTree (void);
    void		onSelectionChange (void);
    void		printTrees (void);
    void		slotItemChanged (void);
    void		checkSelected (void);
    void		slotItemRMClicked (void);

protected:
    //! Reimplemented from AdvListView
    void		contentsMousePressEvent (QMouseEvent *e);

private:
    //!starting top-level Twigs for this controller
    List_of_Twigs	rootTwigs;
};

#endif // IG_CMSCAN_IG_QT_TWIG_CONTROLLER_H
