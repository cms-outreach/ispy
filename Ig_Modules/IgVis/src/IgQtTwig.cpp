/* --------------------------------------------------------------------------*/
/*                                                                           */
/* Class:            IgQtTwig                                                */
/*                                                                           */
/* Description:      OIV / Motif tree structure to control visibility, etc.  */
/*                                                                           */
/* Authors:          G. Alverson, L. Taylor, T.Paul       20 Oct 1998        */
/*                                                                           */
/* Modified by:                                                              */
/*                   I. Gaponenko 28 Oct 1999                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/

/*!
 \class IgQtTwig Ig_Modules/IgVis/interface/IgQtTwig.h

 \brief	The IgQtTwig class serves as an organisational tool for
	OpenInventor scene graphs.  In addition, it allows for control
	of the visibility of sub-graphs and for supplying a
	centralised dispatch for selection.  */

#include "Ig_Modules/IgVis/interface/IgQtTwig.h"
#include "Ig_Modules/IgVis/interface/IgQtTwigCallbackStruct.h"
#include "Ig_Modules/IgVis/interface/IgQtTwigController.h"
#include "Ig_Modules/IgVis/interface/IgQtTwigListViewItem.h"
#include "Ig_Modules/IgVis/interface/IgQtResourceMgr.h"
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/SoSceneManager.h>
#include <Inventor/SoPath.h>
#ifdef TGS_VERSION
# include <Inventor/actions/SoReorganizeAction.h>
#endif

#include <iomanip>
#include <algorithm>

#include <qapplication.h>
#include <qnamespace.h>
#include <qstring.h>
#include <qstatusbar.h>
#include <qregexp.h>
#include <qdatetime.h>
#include <qdom.h>

// init static items in file scope
SbString *		IgQtTwig::leaders = NULL;
IgQtTwigSeparator *	IgQtTwig::theCurrentIvNode = NULL;
QStatusBar *            IgQtTwig::statusBar = NULL;

/*! The base constructor takes the name of the string, a bool to
    indicate the initial visibility state of the Twig, and another
    bool to indicate whether the Widget representing the Twig should
    reveal or close lower-lying members.
 */
IgQtTwig::IgQtTwig (SbString name, bool visibility, bool cascade)
: theOstream (&std::cerr)
{
#if 0 //debug
    *theOstream << "IgQtTwig: Creating Twig "<< name.getString() << std::endl; 
#endif    
    baseConstructor(name,visibility,cascade);
}
IgQtTwig::IgQtTwig (IgQtTwig *tParent, SbString name, bool visibility, bool cascade)
: theOstream (&std::cerr)
{
#if 0 //debug
    *theOstream << "IgQtTwig: Creating Twig "<< name.getString() << std::endl; 
#endif
    baseConstructor(name,visibility,cascade);
    if (tParent != NULL) {
      tParent->addChild(this);
#if 0 //debug
    *theOstream << "IgQtTwig: Created Twig "<< name.getString() << std::endl; 
#endif
    }
    else {
      *theOstream << "IgQtTwig: Error creating twig. Bad parent parameter." << std::endl;
    }
}
void IgQtTwig::baseConstructor(SbString name, bool visibility,bool cascade)    
{
    ////////////////////////////////////////////////////////////////////////
    //
    //  Constructor
    //
    ////////////////////////////////////////////////////////////////////////


    nameOfTwig       = name;
    ion              = visibility;
    icascade         = cascade;


    theLVItem        = NULL;
    parentTwig       = NULL;
    parentIvNode     = NULL;
    
    theController    = NULL;
    
    id               = 0;

    pickedPoint      = NULL;
	
	optimisable      = true;


    //   Create an OIV separator
    //   -----------------------
    oiv_node = new SoSeparator; // NOTE: IgQtTwigSeparator is an SoGroup, but by default
	                            // we use a separator


    oiv_node->setName( validateName(name) );
    
#if 0 //debug    

    *theOstream << "IgQtTwig::IgQtTwig   Creating new twig called : "
	 << name.getString() << std::endl;

#endif //debug    
    

}

void 
IgQtTwig::removeChild(IgQtTwig *child)
{
	List_of_Twigs::iterator it = find(twigs.begin(),twigs.end(),child);
	if (it != twigs.end())
	{
			child->destroyTree();
			oiv_node->removeChild(child->oiv_node);
			twigs.erase(it);
	}		
}
/* Destroys all of the children of this Twig!  Also destroys
   TwigSeparators.  */
void
IgQtTwig::destroyTree (void)
{
    // Remove twig List
    destroyTwig();

    // Remove TwigSeparators
    if (oiv_node->getNumChildren() >= 1)
        oiv_node->removeAllChildren();
}


/*! Destroy the twig and the List of children twigs for the twig
    (recursive).  */
void
IgQtTwig::destroyTwig (void)
{
    for (List_of_Twigs::iterator i = twigs.begin (); i != twigs.end (); i++)
    {
	(*i)->destroyTwig();
	delete *i;
    }
    twigs.clear();
}

/*! Return the number of child twigs.  */
int
IgQtTwig::numberOfChildren (void) const
{
    return static_cast<int>(twigs.size());
}

/*! Return pointer to i'th child.  */
IgQtTwig *
IgQtTwig::getChildByIndex(size_t iChild) const
{
    IgQtTwig *tempTwig = NULL;

    if (! twigs.empty() && iChild <= twigs.size ())
    {
        tempTwig = twigs[iChild];
    }
    else
    {
        *theOstream << "Twig::getChildByIndex>  Error for Twig = "
	     << nameOfTwig.getString() << ",  iChild = " << static_cast<int>(iChild) << std::endl;
    }

    return tempTwig;
}

/*! Add a child twig (i.e. *t) to this twig (i.e. *this).  */
void
IgQtTwig::addChild (IgQtTwig *t)
{
    //   tell the child twig (t) that this twig is its parent
    twigs.push_back(t);
    t->parentTwig = this;
    //   tell this IV node about the child IV node
    if (t->getIvNode ())
    {
	oiv_node->addChild(t->getIvNode ());
    }

	// set the output stream of the child to be that of the parent
	t->theOstream = theOstream;

}

/*!  Walks down the tree drawing contents of twigs, sub-twigs, etc.  */
void
IgQtTwig::drawTree (void)
{
    preDrawMe();

    for (List_of_Twigs::iterator i = twigs.begin (); i != twigs.end (); i++)
	(*i)->drawTree();
}

/*!  Draw a twig (recursive) each derived class should have its own
     drawMe member

     This base method takes whatever is presently in the Twig tree and
     unrefs the highest Twig oiv_node with ion set false (which
     prevents the rest of that branch from being rendered).

     If the branch ion is set true, it makes sure that the oiv node is
     (re-)attached.  */
/*!  Exists only to be sub-classed */
void
IgQtTwig::drawMe (void)
{
}
void
IgQtTwig::preDrawMe (void)
{
    // Use theCurrentIvNode as "global place" where e.g. GEANT3
    // drawing routines can find out the IV node on which to hang
    // their So objects (yeah - we know it's a kludge)

    IgQtTwig::theCurrentIvNode = oiv_node;

    if (ion)
    {
	if (parentTwig != NULL)
	{
	    // check if this oiv node is already attached to the oiv node of the parent twig, 
	    // otherwise attach it
	    if ((parentTwig->getIvNode())->findChild(oiv_node) < 0)
	    {
		// attaching: is there a placeholder?
		placeholderMap::iterator place = twigPlaceholder.find(oiv_node);
		if (place != twigPlaceholder.end() && (*place).second != NULL )
		{
		    IgQtTwigSeparator *dummy = (*place).second;
		    (parentTwig->getIvNode())->replaceChild(dummy,oiv_node);
		}
		else
		{					
		    (parentTwig->getIvNode())->addChild(oiv_node); 
		}
		oiv_node->unref(); //decrement count previously bumped
	    }
	}
    }
    else
    {
	// off: make sure this oiv node is detached from the parent Twig/IvNode
	// modified to check for already removed, 19 Dec 2001, GOA
	IgQtTwigSeparator *root = NULL;
	if (oiv_node != NULL && parentTwig != NULL && !(parentTwig->getIvNode()->findChild(oiv_node)<0))
	{
	    root = parentTwig->getIvNode();
	}
	if (root) 
	{
	    // don't lose the node when we detach it; make sure we swap it with a placeholder
	    oiv_node->ref();
	    IgQtTwigSeparator *dummy = new IgQtTwigSeparator;
	    std::pair<placeholderMap::iterator, bool> itpair = 
		twigPlaceholder.insert(placeholderMap::value_type(oiv_node,dummy));
	    if (!itpair.second) (*itpair.first).second = dummy;
	    root->replaceChild(oiv_node,dummy);
	}
    }
    if (okToRedraw()) {
	QApplication::setOverrideCursor( Qt::waitCursor );
	if (IgQtTwig::statusBar != NULL ) 
	    IgQtTwig::statusBar->message(QString(nameOfTwig.getString())+QString(" building..."));
	drawMe();
	if (IgQtTwig::statusBar != NULL ) IgQtTwig::statusBar->clear();
	QApplication::restoreOverrideCursor();
    }
}

/*! In most instances this is the basic test to see if a node can and
    should be drawn. The criteria are
       1) The visibility flag must be set on
       2) The oiv node must exist, otherwise the Twig::drawMe() failed
       3) The node shouldn't have pre-existing children, otherwise we've previously drawn the node  
*/
bool
IgQtTwig::okToRedraw (void)
{
    return ion && oiv_node && oiv_node->getNumChildren() == 0;
}

/*! Removes all children (oiv_nodes, not Twigs) of this oiv_node, then
    recreates the oiv_node structure to match the Twig structure.  */
void
IgQtTwig::clearTree (void)
{
    // First attach all sub-twig separators (not attached if invisible)
    attachChildren();   // recursive, so also does grandchildren, etc.

    // Now that all sub-twig seperators are attached, blow them all
    // away and remake tree
    if (oiv_node->getNumChildren() >= 1)
    {
        oiv_node->removeAllChildren();
    }
    recreateIvTree();
}

/*! Walks down the tree attaching child separators if necessary
    (i.e. the invisible ones which are otherwise not attached)

    Generally used only by a clearTree to make sure everything is
    attached prior to clearing.  */
void
IgQtTwig::attachChildren (void)
{
    for (List_of_Twigs::iterator i = twigs.begin (); i != twigs.end (); i++)
	// child is not visible => it is not attached
	if (! (*i)->getVisibility()) {
	    // re-attach the child
		IgQtTwigSeparator *child = (*i)->getIvNode();
		placeholderMap::iterator place = (*i)->twigPlaceholder.find(child);
		if (place != (*i)->twigPlaceholder.end() && (*place).second != NULL )
		{
			IgQtTwigSeparator *dummy = (*place).second;
			oiv_node->replaceChild(dummy,child);
		}
		else
		{
			oiv_node->addChild(child);
		}
		child->unref(); //decrement reference count
	    // attach grandchildren to child
	    (*i)->attachChildren();
	}
}
void
IgQtTwig::recreateIvNode (void)
{
	oiv_node = new SoSeparator;
	oiv_node->setName(validateName(nameOfTwig));
}
/*! Trace through the Twig tree (starting from this Twig) and create
    oiv_nodes for each subTwig.  This does not check whether the
    existing oiv_nodes point to anything in particular; it just makes
    new ones according to the organization of the Twigs, so this
    function should not be invoked just for the hell of it.  Normally,
    this function will only be invoked by clearTree.  */

void
IgQtTwig::recreateIvTree (void)
{
    for (List_of_Twigs::iterator i = twigs.begin (); i != twigs.end (); i++)
    {
	(*i)->recreateIvNode();
	oiv_node->addChild((*i)->oiv_node);
	(*i)->recreateIvTree();
    }
}

/*! One wants to be able to turn the entire tree on or off, but the
    top Twig has no Twig parent. If the Twig hangs off another OIV
    Separator, it should serve the same purpose.  */
bool
IgQtTwig::setParentIvNode (IgQtTwigSeparator * /* pOIV */)
{
    return false; // obsolete 
}

/*! Return a pointer to the Iv node which is the parent of the IvNode
    of this Twig.  */
IgQtTwigSeparator *
IgQtTwig::getParentIvNode (void)
{
    if (parentTwig)
    {
	return parentTwig->oiv_node;
    }
    else
    {
	return NULL;
    }
}

/*! Optimisation: If this twig allows for optimisation and is visible, it will try to optimise 
 *  any of its child twigs. If there are no child twigs, it will apply the
 *  preferred optimisation process (currently reorganization) to its own SoSeparator
 *
 *  This is currently a work in progress and other types of optimisation may 
 *  be substituted in the future, as well as a more elaborate system
 *  of controlling the type and degree of optimisation.
 *
 *  Note that any twig with an explicit oiv object plus twigs must do its own optimisation
 *  to avoid having the twig separators optimised away.
 */
void IgQtTwig::optimise()
{
	if (ion)
	{
		for(List_of_Twigs::iterator it = twigs.begin(); it != twigs.end(); it++ ) {
			(*it)->optimise();
		}
		if (optimisable && twigs.empty() ) // protect against optimising away twig separators
		{
#if TGS_VERSION
			SoReorganizeAction simplify;
			QTime timer;
			timer.start();
#if 0 // report to the user
			*theOstream << "Starting reorganisation of twig " << getName().getString() << std::endl;
#endif
			QApplication::setOverrideCursor( Qt::waitCursor );
			if (IgQtTwig::statusBar != NULL ) 
				IgQtTwig::statusBar->message(QString(nameOfTwig.getString())+QString(" optimising..."));
			simplify.apply(oiv_node);
			IgQtTwigSeparator *new_root = new IgQtTwigSeparator;
			new_root->ref();
			// retain the old name
			new_root->setName(oiv_node->getName());
			new_root->addChild(simplify.getSimplifiedSceneGraph());
			if (parentTwig)
			{
				parentTwig->oiv_node->replaceChild(oiv_node,new_root);
				oiv_node = new_root;
			}
			new_root->unref();
			optimisable = false; // can only optimise once with a given scheme
			if (IgQtTwig::statusBar != NULL ) 
				IgQtTwig::statusBar->message(QString(nameOfTwig.getString())+QString(" optimised in %1 s").arg(timer.elapsed()/1000.F),2000);
			QApplication::restoreOverrideCursor();
#if 0 //report
			*theOstream << "Completed reorganisation in " << timer.elapsed()/1000.F << " seconds" << std::endl;
#endif
#endif
		}
		
	}
}
////////////////////////////////////////////////////////////////////////
//
//                    Printing Related Items
//
////////////////////////////////////////////////////////////////////////

/*! Sets the output stream to use to receive pick information.  Propagates down the twig. */
void
IgQtTwig::setOstream (std::ostream& os)
{
	theOstream->flush();
    theOstream = &os;
	for (List_of_Twigs::iterator i = twigs.begin (); i != twigs.end (); i++)
    {
		(*i)->setOstream(os);
    }


}

/*! Use with overloaded << to allow cout-type operations on a twig.  */
void
IgQtTwig::print (std::ostream &os) const
{
    os << nameOfTwig.getString();
}

/*! Walks down the tree printing contents of twigs, sub-twigs, etc.  */
void
IgQtTwig::printTree (void)
{
    leaders = new SbString("Twig >  ");
    *theOstream << std::endl << std::endl;

    printMe();

    *theOstream << std::endl << std::endl;
    //      delete leaders;
    leaders->makeEmpty(true);
}

/*! Print a twig (recursive).  */
void IgQtTwig::printMe()
{
    *theOstream << leaders->getString() << " |---> " << getName().getString()
	 << (ion ? " (ON)" : " (OFF)") << std::endl;

    *leaders += "       ";
    for (List_of_Twigs::iterator i = twigs.begin(); i != twigs.end (); i++)
	(*i)->printMe();

    leaders->deleteSubString((*leaders).getLength()-7,-1);
}

///////////////////////////////////////////////
//
//  Picking
//
///////////////////////////////////////////////
void IgQtTwig::picked(SoNode *node, SoPath * /*path*/)
{
	printInfo( *theOstream, node );
}

/*! Return the most recently-picked point */
const SoPickedPoint *
IgQtTwig::getCurrentPick (void) const
{
    return pickedPoint;
}

/*! Set the picked point. */
void
IgQtTwig::setCurrentPick (const SoPickedPoint *pick)
{
    pickedPoint = pick;
}

/*! A pick as opposed to a selection (see selectionCB) is used when
    either (1) more information than just the path to the object is
    required, eg, the SoPointDetail info, or (2) the path needs to be
    edited before the selection callback is fired.  */
SoPath *
IgQtTwig::pickCB (void *theCBStruct, const SoPickedPoint *pick)
{
    // default behavior is to save the pick information and fire the
    // selectionCB routines
	
    // Pull the items from the Callback structure
    IgQtTwig *twig = ((IgQtTwigCallbackStruct*) theCBStruct)->theTwig;
	
    twig->setCurrentPick(pick);
	return pick->getPath();
}

/*! Callback routine for OIV pick.  Searches for a Twig to match this
OIV node and lets it do the work.  */
void
IgQtTwig::selectionCB (void *theCBStruct, SoPath *selectionPath)
{
    // Pull the items from the Callback structure
	IgQtTwigCallbackStruct *ourCBS = static_cast<IgQtTwigCallbackStruct*>(theCBStruct);
	IgQtTwig *twig = ourCBS->theTwig;
	if (IgQtTwig *matchTwig = twig->findTwigInPath(selectionPath))
	{
		//   highlight widget, if present
		if (matchTwig->theLVItem != NULL) {
			IgQtTwigListViewItem *matchLVI = matchTwig->theLVItem;
			if (! matchLVI->isSelected ()) {
				//try and avoid an infinite loop here...
				//				(matchTwig->theController)->setSelected(matchLVI, true);
				(matchTwig->theController)->setCurrentItem(matchLVI);
			}
		}
		//   SoQt doesn't refresh on a select so we do it manually here...
		SoSceneManager *theManager =
			(static_cast<IgQtTwigCallbackStruct*>(theCBStruct))->theManager;
		
		if (theManager)
			theManager->scheduleRedraw();
		
		//   print any info
		*twig->theOstream << "Found a match in tree " << (twig->getName()).getString()
			<< ": " << std::endl;
		*twig->theOstream << "in Twig " << (matchTwig->getName()).getString()
			<< ": " << std::endl;
		
		// set pick info if present
		matchTwig->setCurrentPick (twig->getCurrentPick());
		
		// invoke pick routine for twig (generally printout)
		matchTwig->picked(selectionPath->getTail(), selectionPath);
		
	}	
}
/*!	work up the path looking for an SoSeparator which is the
 *	separator matching a Twig
 */
IgQtTwig* IgQtTwig::findTwigInPath(const SoPath *selectionPath)
{
	const int	pathNumberNodes = selectionPath->getLength();
	
	int		pathIndex = 0;
	bool	found = false;
	IgQtTwig *	matchTwig = NULL;
	
	while (pathIndex < pathNumberNodes && !found) // double check this line
	{
		SoNode *aNode = selectionPath->getNodeFromTail(pathIndex);
		// jump over any node not a separator
		while (! aNode->isOfType (IgQtTwigSeparator::getClassTypeId ())
			&& pathIndex < pathNumberNodes)
			aNode = selectionPath->getNodeFromTail(++pathIndex);
		
		// have a separator; look for it in the Twig tree
		if ((matchTwig = findTwigNodeMatch(aNode)) != NULL)
			found = true;
		else
			pathIndex++;
	}
	return matchTwig;
}

/*! Search for a particular SoNode in the Twig tree returns pointer to
same or null.  */
IgQtTwig *
IgQtTwig::findTwigNodeMatch (const SoNode *node)
{
    IgQtTwig *found = NULL;
	
    // Is this node it? Note that we're only looking for a match for
    // TwigSeparators, not just any separator, so we don't have to
    // look down from the oiv_node.
    if (oiv_node == node)
		found = this;
    else
    {
		List_of_Twigs::iterator i = twigs.begin ();
		while (i != twigs.end() && !(found = (*i)->findTwigNodeMatch (node))) 
			i++;
    }
	
    return found;
}

/*! Prints any information available concerning an OIV node attached
    to the Twig.  */
void
IgQtTwig::printInfo (std::ostream &ostr, const SoNode * /*node*/) const
{
    ostr << "IgQtTwig " << getName().getString() <<": Selected" << std::endl;
}

////////////////////////////////////////////////////////////////////////
//
//                  Graphical Interface Items
//
////////////////////////////////////////////////////////////////////////


/*! Walk down the tree creating the widgets

    This routine _only_ does the top twig.  All others use
    createList(AdvListViewItem *). */
void
IgQtTwig::createList (IgQtTwigController *parentLV)
{
    //  do this one
    if (theLVItem == NULL)
    {
	theLVItem = createListItem (parentLV);
	theController = parentLV;
	theController->setRootTwig(this);
    }

    //  now do any children
    for (List_of_Twigs::iterator i = twigs.begin (); i != twigs.end (); i++)
        (*i)->createList(theLVItem);

	parentLV->show();
}

/*!  Walk down the tree creating the widgets.  */
void
IgQtTwig::createList (IgQtTwigListViewItem *parentLV)
{
    //  do this one
    if (theLVItem == NULL)
    {
	theLVItem = createListItem(parentLV);
	theController = parentTwig->getController();
    }

    //  now do any children
    for (List_of_Twigs::iterator i = twigs.begin (); i != twigs.end (); i++)
        (*i)->createList(theLVItem);
}

/*! \class TwigListViewItem Twig.h

    \brief Each Twig has an associated TwigListViewItem which controls
    its characteristics.  */

/*! Create the basic visual that represents this twig.  */
IgQtTwigListViewItem *
IgQtTwig::createListItem (IgQtTwigListViewItem *qLVI)
{

    IgQtTwigListViewItem *listItem
	= new IgQtTwigListViewItem(this, qLVI, nameOfTwig.getString());
    listItem->setParentTwig(this);
    listItem->setCheckable(true);
	listItem->setChecked(ion);
	listItem->setOpen(icascade);
    return listItem;
}

/*! Create the basic visual that represents this twig.  */
IgQtTwigListViewItem *
IgQtTwig::createListItem (IgQtTwigController *qLV)
{

    IgQtTwigListViewItem *listItem
	= new IgQtTwigListViewItem(this,qLV, nameOfTwig.getString());
    listItem->setParentTwig(this);
    listItem->setBold(true);
    listItem->setCheckable(true);
	listItem->setChecked(ion);
    return listItem;
}

/*! Return the TwigListViewItem pointer for this Twig.  */
IgQtTwigListViewItem *
IgQtTwig::getListItem (void)
{
    return theLVItem;
}

/*! Return the TwigListViewItem pointer for this Twig.  */
void
IgQtTwig::setController (IgQtTwigController *aController)
{
    theController = aController;
}

/*! Return the TwigListViewItem pointer for this Twig */
IgQtTwigController *
IgQtTwig::getController (void) const
{
    return theController;
}

void IgQtTwig::setIvNode(IgQtTwigSeparator *node)
{
    // might want to check to see if there is a parent Twig and if so, attach this OIV node
    // to it. here we assume that we are assembling a set of twigs from a previously existing network.

#if 0// debug
	if (oiv_node->getChildren() !=0) {
		*theOstream << "IgQtTwig::setIvNode... current Twig OIV node has children!" << std::endl;
	}
	if (parentTwig != NULL ) {
		*theOstream << "IgQtTwig::setIvNode... current Twig OIV node has a parent!" << std::endl;
	} 
#endif
	// retain the old name if a new one isn't supplied
	if (node->getName() == SbName("")){
		node->setName(oiv_node->getName());
	}
	oiv_node=node;
}
IgQtTwig* IgQtTwig::addTwigFromScratch(SoNode* nodeToAdd, bool everyNode, IgQtTwig *thisTwig,
									   SoGroup* parentNode)
{

	// Check to see if this new node is of same type as TwigSeparator
	// (must be at least an SoGroup to do much...).
	// As currently coded, pretty much assumes that TwigSeparator _is_ an SoGroup.
	bool good_sep = false;
	IgQtTwigSeparator* nodeSep = NULL;
	if (nodeToAdd->getTypeId().isDerivedFrom(IgQtTwigSeparator::getClassTypeId())) {
		nodeSep = static_cast<IgQtTwigSeparator*>(nodeToAdd);
		good_sep = true;
	}

	// if the node has a name, use it as the label; else take the node type
	SbName name = nodeToAdd->getName();
	if (name == "") {
		name = nodeToAdd->getTypeId().getName();
	}
	IgQtTwig *newTwig = new IgQtTwig(name.getString(),true,false);
	// addChild would add oiv node as well; perform only essential operations
	if (thisTwig != NULL) {
		thisTwig->twigs.push_back(newTwig);
		newTwig->parentTwig = thisTwig;
	}
	// if we have the appropriate separator, use it as the twig iv node; else just add
	if (good_sep) {
		newTwig->setIvNode(nodeSep);
	}
	else if (everyNode) {
		newTwig->getIvNode()->addChild(nodeToAdd);
		if (parentNode) 
		{
			parentNode->removeChild(nodeToAdd);
		}
	}

	if (good_sep) {
		for (int i=0; i < nodeSep->getNumChildren(); i++ ){
			if ( everyNode ) {
				IgQtTwig::addTwigFromScratch(nodeSep->getChild(i), everyNode, newTwig, nodeSep);
			}
			else {
				SoNode* tempNode = nodeSep->getChild(i);
				if (tempNode->getTypeId().isDerivedFrom(IgQtTwigSeparator::getClassTypeId()))
				{
					IgQtTwig::addTwigFromScratch(tempNode, everyNode, newTwig, nodeSep); 
				}
			}
		}
	}
	return newTwig;
}
void IgQtTwig::makeSelectable()
{
	setIvNode(new SoSelection); 
}
SbString IgQtTwig::validateName(const SbString suspectName)
{
	// Object names must not begin with a digit, and must not contain spaces or control 
	// characters, periods, single or double quote characters, backslashes, curly braces 
	// or the plus character. 
	QString nodeName = QString(suspectName.getString());
	nodeName.replace(QRegExp("\\+"),"_plus_"); // + is too useful to replace with the catch-all symbol
	QRegExp forbiddenCharacters("[\\[\\]\\{\\}\\s\\.\\\'\"]"); //as of Qt3, curly braces need escape
	nodeName.replace(forbiddenCharacters,"_");

	nodeName.replace(QRegExp("^\\d"),"_");

	return SbString( nodeName.latin1() );

}
void IgQtTwig::addIvNode(SoNode *newNode)
{
	oiv_node->addChild(newNode);
}
void IgQtTwig::clearNotify()
{
    for (List_of_Twigs::iterator i = twigs.begin (); i != twigs.end (); i++)
	{
		(*i)->clearNotify();
        }
}
int IgQtTwig::getPickIndex(const SoNode* node) const
{
	int picked_point_ndx = -1;
	// check to see that we really have a picked point and node is a point set
	const SoPickedPoint *pick = getCurrentPick();
	if ( pick != NULL && node->isOfType (SoPointSet::getClassTypeId ())) 
	{
		const SoPointSet *p_PS = 
			static_cast <const SoPointSet*>(node);	
		const SoDetail *pickDetail = pick->getDetail(p_PS);
		if (pickDetail != NULL ){
			const SoPointDetail *p_Detail = static_cast <const SoPointDetail *>
				(pickDetail);
			picked_point_ndx = p_Detail->getCoordinateIndex();
		}
	}
	return picked_point_ndx;
}
QDomElement IgQtTwig::twigToXML( void )
{
    QDomElement t;
    IgQtResourceMgr &theMgr = IgQtResourceMgr::getInstance();
	QDomDocument &theDoc = theMgr.getDoc(true);
	t = theDoc.createElement("twig");
	if (theLVItem)
	{
		t.setAttribute("cascade",theLVItem->isOpen());
	}
	else
	{
		t.setAttribute("cascade",getCascade());
	}
	t.setAttribute("visibility",getVisibility());
	t.setAttribute("name",getName().getString());
	for ( List_of_Twigs::const_iterator it = twigs.begin(); it != twigs.end(); it++ )
	{
	    t.appendChild((*it)->twigToXML());
	}
    return t;
}
bool IgQtTwig::xmlToTwig( const QDomElement &nq )
{
    bool result = false;

    if ( nq.tagName() != "twig" ) return result;


    if (nq.attribute("name") == getName().getString()) {
	result = true;
	setVisibility(nq.attribute("visibility").toInt());
	setCascade(nq.attribute("cascade").toInt());
	if (theLVItem)
	{
		theLVItem->setChecked(getVisibility());
	    theLVItem->setOpen(getCascade());
	}
	// step over twig resources at this level in the tree. 
	// Since Twigs can be dynamic, should check that the number of children 
	// and the name is the same for both the twig and the resource. At present this check
	// is only performed at the top level by the calling routine. After that point the tree is just followed
	// down to the bottom.
	int nc = numberOfChildren();
	int j=0;
	for ( QDomNode n = nq.firstChild(); !n.isNull() && (j < nc); n = n.nextSibling().toElement(), j++ ) 
	{
	    if ( n.isElement() )
	    {
		getChildByIndex(j)->xmlToTwig( n.toElement() );
	    }
	}
    }
    return result;
}
