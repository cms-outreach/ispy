//////////////////////////////////////////////////////////////////////////////
//
//        IgQtClipCtl
//
///////////////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtClipCtl.h"
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/manips/SoClipPlaneManip.h>
#include <Inventor/SbLinear.h>
#include <Inventor/manips/SoJackManip.h>
#include <Inventor/actions/SoSearchAction.h>
#include <assert.h>

#include <qinputdialog.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qkeycode.h>

#include <Inventor/SoInput.h>
#include <Inventor/actions/SoWriteAction.h>

#include <iostream>
#include <fstream>

//storage for statics
const QString IgQtClipCtl::showIcons = QString("Show Icons");
const QString IgQtClipCtl::hideIcons = QString("Hide Icons");

IgQtClipCtl::IgQtClipCtl(QWidget *parent, SoGroup *topNode):
QPopupMenu(parent), idDelObj(0)

{
  insertTearOffHandle();
  QString myType(CtlObj::getClassTypeId().getName().getString());
  setCaption(myType);

  idIconItem = insertItem("Show Icons",this,SLOT(iconChange()));
  idNewObj = insertItem("New",this,SLOT(newIgCtlItem()));
  
  insertSeparator();
  theIvGroup = new SoGroup;

  if (topNode != NULL) {
    makeDefaults();
	topNode->addChild(theIvGroup);
  }
  theIvGroup->ref();

}
IgQtClipCtl::~IgQtClipCtl()
{
  theIvGroup->unref();

}

void IgQtClipCtl::addIvItem(CtlObj *theItem, const QString &name)
{

  int item = insertItem(name);
  ourMap.insert(itemListIv::value_type(item,theItem));
  connectItem(item,
	  this,SLOT(statusChange(int)));
  setItemEnabled(item,true);
  setItemChecked(item,theItem->on.getValue());

  if (idDelObj==0) {
	  objDelMenu = new QPopupMenu(this,"Delete");
	  idDelObj = insertItem("Delete",objDelMenu,-1,indexOf(idNewObj));

  }
  int objDelItem = objDelMenu->insertItem(name,this,SLOT(delIgCtlItem(int))); // use same id as in main menu
  xrefMap.insert(xRefMap::value_type(objDelItem,item));
}  
void IgQtClipCtl::statusChange(int i)
{
	itemListIv::iterator theIter = ourMap.find(i);
	CtlObj *node = (*theIter).second;
	bool itemOn = isItemChecked(i);
	node->on=!itemOn;
	setItemChecked(i,!itemOn);
}
void IgQtClipCtl::iconChange()
{
	SoSearchAction findIt;
    if (text(idIconItem)==hideIcons)
	{
		changeItem(idIconItem,showIcons);
		for (itemListIv::iterator iter = ourMap.begin(); iter != ourMap.end(); iter++)
		{
			CtlObjManip* node = static_cast<CtlObjManip*>((*iter).second);
			findIt.setNode(node);
			findIt.apply(theIvGroup);
			SoPath *aPath = findIt.getPath();
			assert( aPath != NULL ); // it'll die if the path isn't found, ie, if not child of IvGroup
			CtlObj *newObj = new CtlObj;
			newObj->on = node->on;
			newObj->plane = node->plane;
			node->replaceManip(aPath,newObj);
			iter->second = newObj;
		}
	}
	else
	{
		changeItem(idIconItem,hideIcons);
		for (itemListIv::iterator iter = ourMap.begin(); iter != ourMap.end(); iter++)
		{
			CtlObj* node = (*iter).second;
			findIt.setNode(node);
			findIt.apply(theIvGroup);
			SoPath *aPath = findIt.getPath();
			assert( aPath != NULL ); // it'll die if the path isn't found
			CtlObjManip *newObj = new CtlObjManip;
#if 0 //debug
			SoOutput myOutput;
			myOutput.openFile("dumped.iv");
			SoWriteAction writeOut(&myOutput);
			writeOut.apply(aPath);

			std::fstream outFS("dumped.txt",std::ios::out);
			const int len = aPath->getLength();
			for (int i = 0; i < len; i++) {
				outFS << "Path index " << i << std::endl;
				SoNode *node = aPath->getNode(i);
				SoType typeId = node->getTypeId();
				outFS << "    Node of type " << typeId.getName().getString() << std::endl;
			}
			outFS.close();
#endif //debug

			newObj->replaceNode(aPath);
			iter->second = newObj;
		}
	}
}
SoGroup* IgQtClipCtl::makeDefaults()
{
//  create default ClipPlane group

  theIvGroup->setName("Default_Clip_Group");

// use the -ve x direction to make a slice
  CtlObj *wClip = new CtlObj;
  wClip->plane.setValue(SbPlane(SbVec3f(-1.f,0.f,0.f),0.f));
  theIvGroup->addChild(wClip);
  QString clipString("ClipPlane_1");
  addIvItem(wClip, clipString);
  // over-ride to make dis-abled; requires only one CtlObj in menu
  wClip->on = false;
  itemListIv::iterator anIter = ourMap.begin();
  int item = anIter->first;
  setItemChecked(item,false);


  return theIvGroup;
}
void IgQtClipCtl::newIgCtlItem()
{
  bool ok = false;
  QString myType(CtlObj::getClassTypeId().getName().getString());
  QString serialNo("%1");
  QString label = myType+QString("_")+serialNo.arg(static_cast<int>(ourMap.size())+1);
	QString newName = QInputDialog::getText( tr( "Name" ), QString::null, QLineEdit::Normal, label, &ok, this );
  if ( ok && !newName.isEmpty() )
  {
    CtlObj *newClip; 
    if (text(idIconItem)==hideIcons) // check if we want manipulator version or plain
	{
      newClip = new CtlObjManip;
	}
	else
	{
      newClip = new CtlObj;
	}
	theIvGroup->addChild(newClip);
#if 0 //debug
	theIvGroup->removeChild(newClip);
#endif //debug
	addIvItem(newClip, newName);
  }
}
void IgQtClipCtl::delIgCtlItem(int i)
{
	xRefMap::iterator xIter = xrefMap.find(i);
	itemListIv::iterator theIter = ourMap.find((*xIter).second);
	CtlObj *node = (*theIter).second;
	theIvGroup->removeChild(node);
	ourMap.erase(theIter);
	objDelMenu->removeItem(i);
	removeItem((*xIter).second);

	if (ourMap.empty()) {
		removeItem(idDelObj);
		idDelObj = 0;
	}


}
void IgQtClipCtl::addAllIv(SoNode *root)
{
	// Search for ctl's already present. This code from SoQtViewer...
	SoSearchAction search;
	search.setType(CtlObj::getClassTypeId());
	search.setInterest(SoSearchAction::ALL);
	search.apply(root);

	SoPathList paths = search.getPaths();
	QString name;
	for (int i =0; i < paths.getLength(); i++) {
		CtlObj *obj = static_cast<CtlObj*>(paths[i]->getTail());
		if (!obj->getName()){
			name = "Clip_Plane";
		}
		else {
			name = obj->getName().getString();
		}

		addIvItem(obj,name);
	}
}



  
  
