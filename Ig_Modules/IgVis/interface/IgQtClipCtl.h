#ifndef IG_CMSCAN_IG_QT_CLIPCTL_H
#define IG_CMSCAN_IG_QT_CLIPCTL_H
//---------------------------------------------------
//
//  Class to create viewer options panel widget
//
//  G. Alverson   14/07/2000
//---------------------------------------------------
#ifdef WIN32
#pragma warning( once : 4251 ) 
#pragma warning( disable : 4786 )       
#endif

#include "Ig_Modules/IgVis/interface/config.h"
#include <qpopupmenu.h>
#include <map>
#include <functional>
#include <Inventor/nodes/SoGroup.h>


class SoClipPlane;
class SoClipPlaneManip;


/*!  \class IgQtClipCtl IgQtClipCtl.h Ig_Modules/IgVis/interface/IgQtClipCtl.h
Provides a pulldown menu on the menu bar to control additional environmental objects for
a scene.

*/
class IG_VIS_API IgQtClipCtl : public QPopupMenu 
{
    Q_OBJECT

    typedef SoClipPlane CtlObj;
    typedef SoClipPlaneManip CtlObjManip;
public:
    /*! constructor; If provided with \a topNode, will produce a default set of CtlObj's
    and attach them to it.
    */
    explicit IgQtClipCtl(QWidget *parent, SoGroup *topNode=0); 
    virtual ~IgQtClipCtl(); //!< destructor; generic
    void addIvItem(CtlObj *node, const QString &name); //!< add a CtlObj to the control
    void addAllIv(SoNode *root); //!< add all CtlObj's in the scene under root

    /*! Make a default set of CtlObj's and return the SoGroup which holds them.
    \b Note: IgQtClipCtl maintains an internal list of these objects. Ref() or addChild() the
    group right away so that it doesn't get garbage collected by OIV.
    */  
    virtual SoGroup *makeDefaults(); //!< make a default set of objects

protected:

public slots:

private slots:
    void statusChange(int i); //!< slot for check/uncheck actions
    void iconChange(); //!< slot for hide/show icon actions
    void newIgCtlItem(); //!< slot for new one
    void delIgCtlItem(int i); //!< slot for delete actions
private:
    typedef std::map<int, CtlObj*, std::less<int> > itemListIv;
    typedef std::map<int, int, std::less<int> > xRefMap;
    itemListIv ourMap; //!< list of objects we control
    xRefMap xrefMap; //!< list to cross ref ids on different menus
    SoGroup *theIvGroup; //!< group to which they are attached
    int idIconItem; //!< keeps track of the id for the hide/show item
    int idDelObj; //!< id of the delete object popup
    int idNewObj; //!< id of the create new obj item
    QPopupMenu *objDelMenu; //!< popup of deleteable objs
    static const QString showIcons; //!< text string for widget
    static const QString hideIcons; //!< text string for widget

};


#endif // IG_CMSCAN_IG_QT_CLIPCTL_H
