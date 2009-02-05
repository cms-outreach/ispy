//////////////////////////////////////////////////////////////////////
//
// IgQtAnimCtl.cpp: implementation of the IgQtAnimCtl class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtAnimCtl.h"
#include "Ig_Modules/IgVis/interface/IgQtAnim.h"

#include <qinputdialog.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qkeycode.h>

#include <Inventor/SoInput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/fields/SoSFBool.h>

#include <iostream>
#include <fstream>


IgQtAnimCtl::IgQtAnimCtl(QWidget *parent, SoGroup *topNode):
QPopupMenu(parent), idDelObj(0)
{
  insertTearOffHandle();
  setCaption("Animators");

  idNewObj = insertItem("New",this,SLOT(newIgCtlItem()));
  
  insertSeparator();
  theIvGroup = new SoGroup;
  theIvGroup->ref();
  if (topNode != NULL) {
    makeDefaults();
	topNode->addChild(theIvGroup);
  }

}

void IgQtAnimCtl::addIvItem(CtlObj *theItem, const QString &name)
{

  int item = insertItem(name);
  ourMap.insert(itemListIv::value_type(item,theItem));
  connectItem(item,
	  this,SLOT(statusChange(int)));
  setItemEnabled(idNewObj,false); // can only have one Animator

  if (idDelObj==0) {
	  objDelMenu = new QPopupMenu(this,"Delete");
	  idDelObj = insertItem("Delete",objDelMenu,-1,indexOf(idNewObj));
	  //TODO: fix delete
	  setItemEnabled(idDelObj,false); 

  }
  setItemChecked(item,theItem->on().getValue()); 

  int objDelItem = objDelMenu->insertItem(name,this,SLOT(delIgCtlItem(int))); // use same id as in main menu
  xrefMap.insert(xRefMap::value_type(objDelItem,item));
}  
SoGroup* IgQtAnimCtl::makeDefaults()
{
//  create default Animator group

  theIvGroup->setName("Default_Animator_Group");

// 
  CtlObj *anim = new CtlObj;
  theIvGroup->addChild(anim->getNode());
  anim->on() = false; // default is off
  QString animString("Animator_1");
  addIvItem(anim, animString);
  return theIvGroup;
}
void IgQtAnimCtl::newIgCtlItem()
{
	bool ok = false;
	QString myType("IgQtAnimator");
	QString serialNo;
	QString label = myType+QString("_")+serialNo.arg(static_cast<int>(ourMap.size())+1);
	QString newName = QInputDialog::getText( tr( "Name" ), QString::null, QLineEdit::Normal, label, &ok, this );
	if ( ok && !newName.isEmpty() )
	{
		CtlObj *newAnim = new CtlObj; 
		theIvGroup->addChild(newAnim->getNode());
		addIvItem(newAnim, newName);
	}
}
void IgQtAnimCtl::delIgCtlItem(int i)
{
	xRefMap::iterator xIter = xrefMap.find(i);
	itemListIv::iterator theIter = ourMap.find((*xIter).second);
	CtlObj *anim = (*theIter).second;
	theIvGroup->removeChild(anim->getNode());
	ourMap.erase(theIter);
	objDelMenu->removeItem(i);
	removeItem((*xIter).second);
	delete anim;

	if (ourMap.empty()) {
		removeItem(idDelObj);
		idDelObj = 0;
	}
	setItemEnabled(idNewObj,true); //can allow new Animator now

}  

IgQtAnimCtl::~IgQtAnimCtl()
{
  theIvGroup->unref();
}
void IgQtAnimCtl::statusChange(int i)
{
	itemListIv::iterator theIter = ourMap.find(i);
	CtlObj *node = (*theIter).second;
	bool itemOn = isItemChecked(i);
	node->on() = !itemOn;
	setItemChecked(i,!itemOn);
}
IgQtAnimCtl::CtlObj* IgQtAnimCtl::getAnimator()
{
	itemListIv::iterator theIter  = ourMap.begin();
	return (*theIter).second;
}
