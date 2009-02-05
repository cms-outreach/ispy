#ifndef IG_VIS_IG_QT_VIEWPOINT_CTL_H
#define IG_VIS_IG_QT_VIEWPOINT_CTL_H

#ifdef WIN32
#pragma warning( once : 4251 ) 
#pragma warning( disable : 4786 )       
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <qpopupmenu.h>
#include <map>


#include "Ig_Modules/IgVis/interface/IgQtViewpoint.h"

/*!    \class IgQtViewpointCtl IgQtViewpointCtl.h Ig_Modules/IgVis/interface/IgQtViewpointCtl.h 
*     
*      \brief Creates  a pop-up menu to control viewpoints within a scene graph
*
*      \author G. Alverson, Northeastern University
*
*      \date 14-Aug-2000
*
*/

class SoQtViewer;
class QDomElement;

class IG_VIS_API IgQtViewpointCtl : public QPopupMenu 
{
    Q_OBJECT

	typedef IgQtViewpoint CtlObj;

public:

    /*! constructor; If provided with \a topNode, will produce a default set of CtlObj's.
    */
    explicit IgQtViewpointCtl( QWidget* parent, SoQtViewer* viewer ); 
    virtual ~IgQtViewpointCtl(); //!< destructor; generic

    bool addIvItem( CtlObj & node, const QString & name ); //!< add a CtlObj to the control; returns true for success
    //! add all CtlObj's in the scene under \arg topNode. \arg topNode will supercede a \arg topNode passed in
    //! the constructor. 
    //! Note: this can produce duplicates if a given viewpoint is already present
    //! and a file with a viewpoint of the same name is read in.
    void addAllIv( SoNode* topNode );
    //! replace the CtlObj with the given name with then new node
    //! Returns true for success.
    bool replaceIvItem( CtlObj & node, const QString & name ); 
    //! save the viewpoint parameters into XML
    QDomElement viewpointToXML( const CtlObj &co ) const;
    //! save all the viewpoints into a QDomElement
    bool saveResources(  const QString &unique, QDomElement &de ) const;
    //! restore previously saved viewpoints
    void restoreResources( const QString &unique, const QDomElement &de);
    //! create new viewpoint from XML-stored parameters
    IgQtViewpoint xmlToViewpoint( const QDomElement &qe);
    //! add the current viewpoint: complements slot with same name.
    //! If replace is true, will over-write an existing item with the same name.
    void newIgCtlItem(const QString & name, bool replace);
private slots:

    void statusChange( int i ); //!< slot for check/uncheck actions
    void newIgCtlItem(); //!< slot for new one
    void delIgCtlItem( int i ); //!< slot for delete actions, arg is index in delete menu

private:

    typedef std::map< int, CtlObj > itemListIv; //!< maintains the CtlObj's
    typedef std::map< int, int > xRefMap;
    itemListIv	ourMap; //!< list of objects we control
    xRefMap	xrefMap; //! list to cross ref ids on different menus
    SoQtViewer*	m_viewer; //!< viewer for which we set/get the viewpoint
    int		idDelObj; //!< id of the delete object popup
    int		idNewObj; //!< id of the create new obj item
    QPopupMenu* objDelMenu; //! popup of deleteable objs 
};

#endif // IG_VIS_IG_QT_VIEWPOINT_CTL_H









