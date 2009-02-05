#include "Ig_Modules/IgVis/interface/IgQtTwigController.h"
#include "Ig_Modules/IgVis/interface/IgQtTwigListViewItem.h"
#include "Ig_Modules/IgVis/interface/IgQtTwig.h"

#include <Inventor/SoPath.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/actions/SoSearchAction.h>

#include <qhbox.h>

IgQtTwigListViewItem *		getNextItem (IgQtTwigListViewItem* it);
QList<IgQtTwigListViewItem> *	getSelectedItems (IgQtTwigController *lv);

////////////////////////////////////////////////////////////////////////
//
//  TwigController Class
//
////////////////////////////////////////////////////////////////////////

/*!	
  \class IgQtTwigController IgQtTwigController.h Ig_Modules/IgVis/interface/IgQtTwigController.h
  \brief Subclassed from AdvListView. The controller for the
  	 IgQtTwigListItems. Creates the basic widget that controls a twig.  

*/
IgQtTwigController::IgQtTwigController (QWidget *theParent,
										const char *label) : AdvListView(theParent, label)
{
    //  Make it look pretty
    setCaption(label);

    addColumn ("Object");
    addColumn ("Visibility");
    // addColumn ("Type");
    setRootIsDecorated (true);
    setCheckPosition (1);

    // disable multiple selections
    setMultiSelection (false);

    // make non-sorting by default
    setSorting(columns()+1,true); //-1 implies non-sorting, set greater than max to require click on bar

    // connect the routine which highlights the oiv objects
    connect(this, SIGNAL (selectionChanged ()),
	    this, SLOT (onSelectionChange ()));


    connect (this, SIGNAL (selectionChanged ()),
	     this, SLOT (slotItemChanged ()));

}
/*! Add the top-level Twig (t) to the list.  */
void
IgQtTwigController::setRootTwig (IgQtTwig *t)
{
    rootTwigs.push_back(t);
}

QList<IgQtTwigListViewItem> *
IgQtTwigController::getCheckedItems(IgQtTwigController *lv)
{
    if (! lv)
	return NULL;

    // Create the list
    QList<IgQtTwigListViewItem> *lst = new QList<IgQtTwigListViewItem>;
    lst->setAutoDelete(false);

    IgQtTwigListViewItem *it = (IgQtTwigListViewItem*) lv->firstChild();

    for(; it; it = getNextItem(it))
    {
        IgQtTwigListViewItem *titem = static_cast<IgQtTwigListViewItem *> (it);
        IgQtTwig *t = titem->parentTwig;
        if (it->isChecked() == true)
	{
	    t->setVisibility( true );
            lst->append(it);
        }
	else
	    t->setVisibility( false );
    }
    return lst;
}

void
IgQtTwigController::slotItemChanged (void)
{
}

void
IgQtTwigController::slotItemRMClicked (void)
{
}

/*! This method gets activated whenever the visibility toggler is
    pushed.  */
void
IgQtTwigController::visibilityToggle (void)
{
    // What's selected?
    QList<IgQtTwigListViewItem> *list = getSelectedItems(this);

    IgQtTwigListViewItem *item;
    for (item = list->first(); item != 0; item=list->next ())
    {
        IgQtTwigListViewItem *titem
	    = static_cast<IgQtTwigListViewItem *>(item);
        IgQtTwig *t = titem->parentTwig;
		t->setVisibility(!t->getVisibility());
    }

    // We've now flipped all the requested visibilities; update the screen
    for (List_of_Twigs::iterator i = rootTwigs.begin ();
	 i != rootTwigs.end (); i++)
        (*i)->drawTree();
}

/*! This method gets activated whenever the visibility toggler is
    pushed.  */
void
IgQtTwigController::checkSelected (void)
{
    // What's selected?
    QList<IgQtTwigListViewItem> *list = getSelectedItems(this);
    IgQtTwigListViewItem *	 item;

    for (item = list->first (); item != 0; item = list->next ())
    {
        IgQtTwigListViewItem *titem
	    = static_cast<IgQtTwigListViewItem *>(item);
        IgQtTwig *t = titem->parentTwig;

	t->setVisibility(true);
	item->setChecked(true);
    }

    // We've now flipped all the requested visibilities; update the screen
    for (List_of_Twigs::iterator i = rootTwigs.begin ();
	 i != rootTwigs.end (); i++)
        (*i)->drawTree();
}

/*! This method gets activated whenever the visibility toggler is
    pushed.  */
void
IgQtTwigController::updateTree (void)
{
    // What's selected?
    QList<IgQtTwigListViewItem> *list = getCheckedItems( this );
    IgQtTwigListViewItem *	 item;
    for (item = list->first (); item != 0; item = list->next ())
    {
        IgQtTwigListViewItem *titem
			= static_cast< IgQtTwigListViewItem* >(item);
        IgQtTwig *t = titem->parentTwig;
		if (item->isChecked ())
			t->setVisibility (true);
		else {
			t->setVisibility (false);
			item->setChecked (false);
		}
    }
    printTrees ();
	
    //  We've now flipped all the requested visibilities; update the screen
    for (List_of_Twigs::iterator i = rootTwigs.begin (); i != rootTwigs.end(); i++)
        (*i)->drawTree();
}

/*! Print out the status of all the Twigs attached to this
    controller.  */
void
IgQtTwigController::printTrees (void)
{
    for (List_of_Twigs::iterator i = rootTwigs.begin ();
	 i != rootTwigs.end(); i++)
        (*i)->printTree();
}

/* Slot for changes in the list of selected TwigListItems. Creates an
   OIV selection list to match the selected items.  */
void
IgQtTwigController::onSelectionChange (void)
{
#if 0 //DEBUG
#ifdef WIN32_TGS
	char buf[128];
	char *pbuf = reinterpret_cast<char*>(&buf);
	sprintf(pbuf,"In onSelectionChange\n");
	OutputDebugString(pbuf);
#else
    std::cout << "onSelectionChange" << std::endl;
#endif //WIN32 or not
#endif  //DEBUG

    // basic sanity check. See if parent Twig (of Controller, ie,
    // top-level twig) has a parent IvNode (the root node) which can
    // manage selections
	
	// since we have an alternate style where the top-level twig is the SoSelection,
	// we have to check for both possibilities
	
    const IgQtTwig * parentTwig = NULL;
	
    if (! rootTwigs.empty ())
	parentTwig = *rootTwigs.begin();

    SoSelection *topSelection = NULL; // this will be the top of the selection path
    SoNode *tempNode;

    while (parentTwig) //walk up the twigs looking for an SoSelection
    {
	tempNode = parentTwig->getIvNode();
	if (tempNode && tempNode->getTypeId().isDerivedFrom(SoSelection::getClassTypeId()))
	{
	    topSelection = static_cast<SoSelection*>(tempNode);
	    parentTwig = 0; //stop the loop
	}
	else
	{
	    parentTwig = parentTwig->getParentTwig();
	}
    }
    if (!topSelection) return;
/*
	else if ((tempNode = parentTwig->getIvNode()) != NULL && tempNode->getTypeId().isDerivedFrom(SoSelection::getClassTypeId()))
	{
		topSelection = static_cast<SoSelection*>(tempNode); 
	}
	else if (parentTwig->getParentTwig() != NULL && (tempNode = parentTwig->getParentTwig()->getIvNode()) != NULL && tempNode->getTypeId().isDerivedFrom(SoSelection::getClassTypeId()))
	{
		topSelection = static_cast<SoSelection*>(tempNode); 
	}
	else 
	{
		return;
	}
*/	
#if 0 //debug
    std::cout << "onSelectionChange: have an SoSelection" << std::endl;
#endif  //debug
    SoPathList *		selectedNodes
		= const_cast<SoPathList *>(topSelection->getList ());
	
    //  What's selected?
    QList<IgQtTwigListViewItem> *list = getSelectedItems(this);
    IgQtTwigListViewItem *	item;
    int				iPath;
    bool			alreadySelected;
#if 0 //debug
    std::cout << "onSelectionChange: looping over selected item list" << std::endl;
#endif  //debug
    // Loop over all the selected list items
    for (item = list->first (); item != 0; item = list->next ())
    {
        IgQtTwigListViewItem *titem = static_cast<IgQtTwigListViewItem *>(item);
        IgQtTwig *t = titem->parentTwig;
		SoNode *aNode = t->getIvNode();
		// Check if this selected list item corresponds to an
		// UNselected OIV node
        for (iPath = 0, alreadySelected = FALSE; !alreadySelected && iPath<selectedNodes->getLength(); iPath++)
			alreadySelected = (*selectedNodes)[iPath]->containsNode(aNode);
		
		if (!alreadySelected)
		{
			// find a path from the selection node to the selected node
			SoSearchAction finder;
			finder.setNode(aNode);
			finder.setFind(SoSearchAction::NODE);
			finder.apply(topSelection);
			
			if (finder.getPath() != NULL )
			{
#if 0 //debug
				std::cout << "onSelectionChange: selecting object" << std::endl;
#endif  //debug
				// Set the node as selected. Standard selection
				// callbacks will fire on exit.
#if 0 //DEBUG
#ifdef WIN32_TGS
				sprintf(pbuf,"In onSelectionChange, firing callback\n");
				OutputDebugString(pbuf);
#endif //WIN32
#endif //DEBUG
				
				topSelection->deselectAll(); // select ignores current selection policy so clear manually
				topSelection->select(finder.getPath());
			}
		}
    }
}

///////////

// This is a local free-floating function so we don't have to expose
// it.  See the Qt documentation for AdvListViewItemIterator for the
// original
IgQtTwigListViewItem *
getNextUpItem (IgQtTwigListViewItem *it)
{
    IgQtTwigListViewItem *newItem = NULL;
    IgQtTwigListViewItem *parent = (IgQtTwigListViewItem *) it->parent ();

    if (parent)
    {
	newItem = (IgQtTwigListViewItem*) parent->nextSibling ();
	if (! newItem)
	    newItem = getNextUpItem (parent);
    }

    return newItem;
}

IgQtTwigListViewItem *
getNextItem (IgQtTwigListViewItem *it)
{
    //  Replacement for AdvListViewItemIterator
    IgQtTwigListViewItem *newItem = (IgQtTwigListViewItem*) it->firstChild ();

    if (! newItem)
    {
	newItem = (IgQtTwigListViewItem*) it->nextSibling ();
	if (! newItem)
	    newItem = getNextUpItem(it);
    }

    return newItem;
}

QList<IgQtTwigListViewItem> *
getSelectedItems(IgQtTwigController *lv)
{
    if (! lv)
	return NULL;

    // Create the list
    QList<IgQtTwigListViewItem> *lst = new QList<IgQtTwigListViewItem>;
    lst->setAutoDelete(false);


    IgQtTwigListViewItem *it = (IgQtTwigListViewItem*) lv->firstChild ();
    for (; it; it = getNextItem (it))
	if (it->isSelected ())
	    lst->append(it);

    return lst;
}

void
IgQtTwigController::contentsMousePressEvent (QMouseEvent *e)
{
    QPoint			vp = contentsToViewport (e->pos ());
    IgQtTwigListViewItem *	item
	= static_cast<IgQtTwigListViewItem*>(itemAt( vp ));

    if (item && item->isCheckable () && isInsideChecker (e->x ()))
    {
		IgQtTwig* t = item->parentTwig;
		t->setVisibility(!item->isChecked());

		for (List_of_Twigs::iterator i = rootTwigs.begin ();
	     i != rootTwigs.end (); i++)
	    (*i)->drawTree();
    }

    AdvListView::contentsMousePressEvent(e);
}
