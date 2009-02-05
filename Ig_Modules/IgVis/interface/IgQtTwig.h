/* --------------------------------------------------------------------------*/
/*                                                                           */
/* Class:            IgQtTwig                                                */
/*                                                                           */
/* Description:      OIV / Qt tree structure to control visibility, etc.     */
/*                                                                           */
/* Authors:          G. Alverson, L. Taylor, T.Paul       20 Oct 1998        */
/*                                                                           */
/* Modified by:      I. Gaponenko 28 Oct 1999                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifndef IG_IG_QT_TWIG_H
# define IG_IG_QT_TWIG_H

# include "Ig_Modules/IgVis/interface/config.h"
# include <Inventor/SbString.h>
# include <Inventor/nodes/SoSeparator.h>
# include <iostream>
# include <vector>
# include <map>

class SoPath;
class SoPickedPoint;

class IgQtTwig;
class IgQtTwigController;
class IgQtTwigListViewItem;

class QStatusBar;
class QDomElement;
class IgQtResourceMgr;

class QWidget;

//!  IgQtTwigSeparator Twigs are separated by SoGroup nodes
typedef SoGroup IgQtTwigSeparator;

// -----------------------------------------------------------------
// OIV / Qt tree structure to control visibility, etc..
// -----------------------------------------------------------------

/*!  \class IgQtTwig IgQtTwig.h Ig_Modules/IgVis/interface/IgQtTwig.h
 *   \brief Qt-based tree structure to control visibility, etc.
 *
 *   An OpenInventor (or other acyclic, directed graph based scene representation
 *   such as PHIGS) scene can be paralleled by a control structure which determines
 *   creation of the scene objects and their visibility. Selecting an object in the
 *   scene highlights its corresponding control element (twig) and vice versa.
 *
 *   \author George Alverson,  Lucas Taylor and Tom Paul, Northeastern University
 *   \date Motif version, 1998, Qt version (with Iana Gaponenko) October 1999
 *
 */
class IG_VIS_API IgQtTwig
{
public:
	//! Use a vector to organize child twigs
    typedef std::vector<IgQtTwig *>	List_of_Twigs;
	//! Use a map to lookup placeholders
	typedef std::map<IgQtTwigSeparator*,IgQtTwigSeparator*> placeholderMap;
    /*! Constructor: Required arguments are the parent twig and name.
	 *  Visibility and cascade set the original appearance in the controller.
	 */
    explicit
    IgQtTwig (IgQtTwig *parent, SbString name, bool visibility = true, bool cascade = true);
    //! Constructor: Pass name of IgQtTwig, on in tree flag, cascade menu flag
    explicit
    IgQtTwig (SbString name, bool visibility = true, bool cascade = true);

    //! Destructor
    virtual ~IgQtTwig (void);

    // TWIG SIDE

    //! add a Child twig
    virtual void		addChild (IgQtTwig *anIgQtTwig);

	//! return the name of this twig
    SbString		getName (void) const;

	//! set the name of this twig
    void		setName (SbString name);

	//! ensure that the oiv node is selectable; currently can only be used by a bare twig
	void		makeSelectable();

    //! Set visibility of twig
    void		setVisibility (bool b);

	//! Get the visibility
    bool		getVisibility (void) const;

	//! Get the cascade status
	bool        getCascade(void) const;

	//! Set the cascade status
	void        setCascade(bool b);

    //! ASCII dump of twig structure
    void		printTree (void);

	//! remove a twig and all descendants
	void removeChild(IgQtTwig *child);

	//! Get the parent twig
	IgQtTwig const *getParentTwig(void) const;


    //! (deprecated: for use with twigs that build their own widgets) destroy the entire twig tree.
    void		destroyTree (void);

	//! add a new Tree created from a scene rooted on nodeToAdd
	static IgQtTwig* addTwigFromScratch(SoNode* nodeToAdd, bool everyNode = false, 
		IgQtTwig *thisTwig = NULL, SoGroup *parentNode=NULL);

	//! specify the output stream for pick/info 
	void		setOstream (std::ostream &os);

    // INVENTOR SIDE

    //! Returns OIV seperator node associated to twig
    IgQtTwigSeparator *	getIvNode (void) const;

    //! for use _only_ with a node w/o a Twig parent.
    bool		setParentIvNode (IgQtTwigSeparator *pOIV);

    //! get possible Iv node parent
    IgQtTwigSeparator *		getParentIvNode (void);

	//! adds an Iv node 
	virtual void addIvNode(SoNode* newNode);

		//! (deprecated) for those situations (i.e., using GEANT3) where there is a current twig
    static IgQtTwigSeparator *getCurrentIvNode (void);

    //! invoke drawMe method of subtwigs
    virtual void		drawTree (void);

    //! destroy IgQtTwigSeparators (NOT Twigs) + recreate IgQtTwigSeparators
    virtual void		clearTree (void);

	//! need a static method for OIV callbacks: pick has extra info (SoPickedPoint)
	static SoPath *	pickCB (void *theCBStruct, const SoPickedPoint *pick);

	//! need a static method for OIV callbacks: selection needs path to picked point
    static void		selectionCB (void *theCBStruct, SoPath *selectionPath);

	//! a utility method to ensure that we correctly transform a Twig name into an SbName
	SbString validateName(const SbString suspectName);
	
	//! does this twig allow for optimisation?
	bool isOptimisable() const;

	//! tell this node it is optimisable
	void setOptimisable(const bool optimise);

	//! perform whatever optimisation is available
	virtual void optimise();

	//! number of child twigs
        int	numberOfChildren (void) const;

	//! returns the ith child twig
        IgQtTwig * getChildByIndex (size_t iChild) const;


    // QT SIDE

    //! generate the Widgets/IgQtTwig
    void		createList (IgQtTwigController *parent);

	//! get the TwigController for this twig
    IgQtTwigController *getController (void) const;

	//! get the ListViewItem for this twig
    IgQtTwigListViewItem*getListItem (void);

    // OTHER

    //! extractor for twig
    friend std::ostream &	operator<< (std::ostream &os, const IgQtTwig &twig);

    //! specify the status bar for messages
    static void         setStatusBar (QStatusBar *bar); 

    //! return the output stream
    std::ostream *getOstream(void);


    //! format a twig as XML
    QDomElement twigToXML( void );

    //! de-format XML back to a twig
    bool xmlToTwig( const QDomElement& nq );

protected:

    //! for tracebacks
    void		printMe (void);

	//! method which supplies info about this node
    virtual void	printInfo (std::ostream &ostr, const SoNode *node) const;

	//! used by <<
    virtual void	print (std::ostream &ostr) const;

	//! builds the visual structure for the twig
    virtual void	drawMe (void);

	//! internal method
    void                preDrawMe (void);

    //! basic check of drawability
    bool		okToRedraw (void);

    //! give prior warning before a clearTree
    virtual void clearNotify();

    //!   generate the Widgets/IgQtTwig
    void		createList (IgQtTwigListViewItem *parent);

    //! return the list view item for this twig
    IgQtTwigListViewItem *getTheLVItem (void);

    //! create a list item (for another item)
    IgQtTwigListViewItem *createListItem (IgQtTwigListViewItem *parentLVI);

    //! create a list item (for a controller)
    IgQtTwigListViewItem *createListItem (IgQtTwigController *parentLV);

    //! set the controller for this twig
    void		setController (IgQtTwigController *aController);

    /*! For use when constructing twigs from an already existing tree.
     *  Replaces current IvNode with a new one.
     */
    void setIvNode(IgQtTwigSeparator* node);


    //! returns current picked point
    const SoPickedPoint *getCurrentPick (void) const;

    //! sets the current picked point
    void		setCurrentPick (const SoPickedPoint *pick);

    //! returns index for picked point in a PointSet (-1 for no match)
    int getPickIndex(const SoNode *node) const;

    //! method invoked whenever a child node of this twig is picked
    virtual void picked(SoNode *node, SoPath *path);

    /*! Walk down the tree attaching child separators if necessary
     * (i.e. the invisible ones which are otherwise not attached)
     *
     * Generally used only by a clearTree to make sure everything is
     * attached prior to clearing. 
	 */
    void		attachChildren (void);

	/*! Trace through the Twig tree (starting from this Twig) and create
    oiv_nodes for each subTwig.  This does not check whether the
    existing oiv_nodes point to anything in particular; it just makes
    new ones according to the organization of the Twigs, so this
    function should not be invoked just for the hell of it.  Normally,
    this function will only be invoked by clearTree.  
	 */
    virtual void		recreateIvTree (void);
    /*! regenerate the iv basis (typically the SoSeparator) for a twig */
    virtual void		recreateIvNode (void);

    //! (deprecated: only for use by twigs that build sub-widgets) destroy an entry in twigs list
    void		destroyTwig (void);

	//! search for a twig containing node in path
	IgQtTwig *	findTwigInPath(const SoPath *path);
	//! search for a twig containing node: utilitiy for findTwigInPath
    IgQtTwig *	findTwigNodeMatch (const SoNode *node);

    
    //! does construction for declared constructors
    void baseConstructor(SbString name, bool visibility, bool cascade);

    //! children
    List_of_Twigs	twigs;

private:

    //! name of the twig
    SbString		nameOfTwig;

    //! display flag
    bool		ion;

    //! flag for cascading toggle buttons
    bool		icascade;

	//! will allow optimisation
	bool        optimisable;

    //! id of mirror graphics object
    int			id;

	//! lookup for twig placeholders
	placeholderMap twigPlaceholder;


    //! parent twig
    IgQtTwig *		parentTwig;


    //! QListViewItem that represents this twig
    IgQtTwigListViewItem *theLVItem;

    //! First level twigs are attached directly to the controller
    IgQtTwigController *theController;

    //! OpenInventor separator node
    IgQtTwigSeparator *	oiv_node;

    //! where to route output
	std::ostream *		theOstream;

    //! pick routines store the point here
    const SoPickedPoint *pickedPoint;

    //! for use only as a placeholder in Twigs w/o a parent
    IgQtTwigSeparator *	parentIvNode;


// static     

    //! holder for (optional) QStatusBar
    static QStatusBar *statusBar;

    //! marks active node in draw sequence
    static IgQtTwigSeparator *theCurrentIvNode;

    //! leaders string for diagnostic printout
    static SbString *	leaders;
};

//! Inline defs
inline
IgQtTwig::~IgQtTwig (void)
{; }

inline IgQtTwigSeparator *
IgQtTwig::getIvNode (void) const
{ return oiv_node; }

inline void
IgQtTwig::setVisibility (bool b)
{ ion = b; }

inline bool
IgQtTwig::getVisibility (void) const
{ return ion; }

inline bool
IgQtTwig::getCascade (void) const
{ return icascade; }

inline void
IgQtTwig::setCascade (bool c)
{ icascade = c; }

inline SbString
IgQtTwig::getName (void) const
{ return nameOfTwig; }

inline bool IgQtTwig::isOptimisable() const 
{return optimisable;}

inline void IgQtTwig::setOptimisable(const bool optimise) 
{optimisable = optimise;}

inline IgQtTwigSeparator *
IgQtTwig::getCurrentIvNode (void)
{ return IgQtTwig::theCurrentIvNode; }

inline std::ostream &
operator<< (std::ostream &os, const IgQtTwig &twig)
{
    twig.print (os);
    return os;
}

inline void
IgQtTwig::setStatusBar(QStatusBar *bar)
{ IgQtTwig::statusBar = bar; }

inline std::ostream* 
IgQtTwig::getOstream(void)
{return theOstream; }

inline IgQtTwigListViewItem *
IgQtTwig::getTheLVItem (void)
{return theLVItem; }

inline IgQtTwig const *
IgQtTwig::getParentTwig (void) const
{return parentTwig; }


#endif // IG_CMSCAN_IG_QT_TWIG_H
