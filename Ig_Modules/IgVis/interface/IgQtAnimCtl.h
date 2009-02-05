#if !defined(AFX_IGQTANIMCTL_H__0475BF20_47F2_4FEA_9641_14B6B7390C7D__INCLUDED_)
#define AFX_IGQTANIMCTL_H__0475BF20_47F2_4FEA_9641_14B6B7390C7D__INCLUDED_

//////////////////////////////////////////////////////////////////////
//
// IgQtAnimCtl.h: interface for the IgQtAnimCtl class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//---------------------------------------------------
//
//  Class to create an Animator on demand
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
#include <Inventor/nodes/SoGroup.h>
 
class IgQtAnim;
class SoNode;



/*!  \class IgQtAnimCtl IgQtAnim.h Ig_Modules/IgVis/interface/IgQtAnim.h
     Provides a pulldown menu on the menu bar to control an Animator for
	 a scene.

 */
class IG_VIS_API IgQtAnimCtl : public QPopupMenu 
{
  Q_OBJECT

  typedef IgQtAnim CtlObj;
public:
/*! constructor; If provided with \a topNode, will produce a default CtlObj
  and attach it.
 */
  explicit IgQtAnimCtl(QWidget *parent, SoGroup *topNode=0); 
  virtual ~IgQtAnimCtl(); //!< destructor; generic
  void addIvItem(CtlObj *node, const QString &name); //!< add a CtlObj to the control
  CtlObj* getAnimator(); //!< return the current animator

/*! Make a default set of CtlObj's and return the SoGroup which holds them.

  \b Note: the control maintains an internal list of these objects. Ref() or addChild() the
  group right away so that it doesn't get garbage collected by OIV.

  \b Note 2: The default state is off. Turn animation on through the menu item.

 */ 
  virtual SoGroup *makeDefaults(); //!< make a default set of objects

protected:

public slots:

private slots:
	void newIgCtlItem(); //!< slot for new one
	void delIgCtlItem(int i); //!< slot for delete actions
	void statusChange(int i); //!< slot for check/uncheck actions
private:
	typedef std::map<int, CtlObj*, std::less< int > > itemListIv;
	typedef std::map<int, int, std::less< int > > xRefMap;
	itemListIv ourMap; //!< list of objects we control
	xRefMap xrefMap; //!< list to cross ref ids on different menus
	SoGroup *theIvGroup; //!< group to which they are attached
	int idIconItem; //!< keeps track of the id for the hide/show item
	int idDelObj; //!< id of the delete object popup
	int idNewObj; //!< id of the create new obj item
	QPopupMenu *objDelMenu; //!< popup of deleteable objs
 
};


#endif // !defined(AFX_IGQTANIMCTL_H__0475BF20_47F2_4FEA_9641_14B6B7390C7D__INCLUDED_)
