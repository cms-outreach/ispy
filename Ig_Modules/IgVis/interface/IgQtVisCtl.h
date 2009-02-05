#ifndef IG_VIS_IG_QT_VIS_CTL_H
#define IG_VIS_IG_QT_VIS_CTL_H

#ifdef WIN32
#pragma warning( once : 4251 ) 
#pragma warning( disable : 4786 )       
#endif

#include "Ig_Modules/IgVis/interface/config.h"
#include <qpopupmenu.h>
#include <map>
#include <functional>
#include <Inventor/nodes/SoGroup.h>
 
class SoLight;
class SoDirectionalLightManip;
class QDomElement;


/*! \class IgQtVisCtl IgQtVisCtl.h Ig_Modules/IgVis/interface/IgQtVisCtl.h
 *
 *  \brief Provides a pulldown menu on the menu bar to control additional environmental objects for
 *	   a scene.
 *
 *  \author G. Alverson
 *  \date 14-Jul-2000
 *
 */
class IG_VIS_API IgQtVisCtl : public QPopupMenu 
{
    Q_OBJECT

public:

    typedef SoLight CtlObj;
    typedef SoLight CtlObjManip;
    typedef std::pair<QString, CtlObj*> CtlPair;
    
    /*! constructor; If provided with \a topNode, will produce a default set of CtlObj's
      and attach them to it.
    */
    explicit IgQtVisCtl( QWidget* parent, SoGroup* topNode = 0 ); 
    virtual ~IgQtVisCtl(); //!< destructor; generic

    bool addIvItem( CtlObj* node, const QString & name ); //!< add a CtlObj to the control
    void addAllIv( SoNode* root ); //!< add all CtlObj's in the scene under root
    void allOff(void ); //!< turn off all lights

    /*! Make a default set of CtlObj's and return the SoGroup which holds them.
      \b Note: IgQtVisCtl maintains an internal list of these objects. Ref() or addChild() the
      group right away so that it doesn't get garbage collected by OIV.
    */
  
    virtual SoGroup* makeDefaults(); //!< make a default set of objects

    //! save the CtlObj parameters into XML
    QDomElement ctlPairToXML( const CtlPair &co ) const;
    //! create new CtlObj from XML-stored parameters
    CtlPair* xmlToCtlPair( const QDomElement &qe);
    //! save all the CtlObj settings into a QDomElement
    bool saveResources(  const QString &unique, QDomElement &de ) const;
    //! restore previously saved settings
    void restoreResources( const QString &unique, const QDomElement &de);

private slots:

    void statusChange( int i ); //!< slot for check/uncheck actions
    void iconChange(); //!< slot for hide/show icon actions
    void newIgCtlItem( int i ); //!< slot for new one
    void delIgCtlItem( int i ); //!< slot for delete actions

private:


    typedef std::map< int, CtlObj* , std::less< int > > itemListIv;
    typedef std::map< int, int, std::less< int > > xRefMap;


    itemListIv	ourMap; //!< list of objects we control
    xRefMap	xrefMap; //! list to cross ref ids on different menus
    SoGroup*	theIvGroup; //!< group to which they are attached
    int		idIconItem; //!< keeps track of the id for the hide/show item
    int		idDelObj; //!< id of the delete object popup
    int		idNewObj; //!< id of the create new obj item
    QPopupMenu*	objDelMenu; //! popup of deleteable objs
    int		idDir; //! for Directional lights
    int		idPoint; //! for Point lights
    int		idSpot; //! for Spot lights
    static const QString showIcons;
    static const QString hideIcons;
 
};

#endif // IG_VIS_IG_QT_VIS_CTL_H





