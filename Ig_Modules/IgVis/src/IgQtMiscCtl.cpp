//////////////////////////////////////////////////////////////////////
//
// IgQtMiscCtl.cpp: implementation of the IgQtMiscCtl class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtMiscCtl.h"
#include <qinputdialog.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qkeycode.h>
#include <qdialog.h>
#include <qcolordialog.h>
#include <qlayout.h>
#include <qtabdialog.h>
#include <qtabwidget.h>
#include <qlabel.h>
#include <qgroupbox.h>

#include <typeinfo>

#include "Ig_Modules/IgVis/interface/IgQtTwig.h"
#include "Ig_Modules/IgVis/interface/IgQtSlicerImpl.h"
#include "Ig_Modules/IgVis/interface/IgQtBrowser.h"
#include "Ig_Modules/IgVis/interface/IgQtColorScale.h"
#include "Ig_Modules/IgVis/interface/IgQtThresholdSliderImpl.h"
#include "Ig_Modules/IgVis/interface/IgQtPhysTabsImpl.h"
#include "Ig_Modules/IgVis/interface/IgQtXyzScaleImpl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IgQtMiscCtl::IgQtMiscCtl(QWidget *parent, SoGroup *topNode, SoQtFullViewer *viewer) :
    QPopupMenu(parent), root(topNode), zDialog(NULL), theBrowser(viewer)
{

    insertItem("Z Slice",this,SLOT(zSliceSlot()));
    zDialog = new IgQtSlicerImpl(root,this);

	insertItem("Scale",this,SLOT(scaleSlot()));
	scaleDialog = new IgQtXyzScaleImpl(root,this);

    insertItem("Background Color",this,SLOT(bkgColorSlot()));
    //creates background color slot in Misc pull down window

    insertItem("Color Scale",this,SLOT(colorScaleSlot()));
    colorScaleDialog = new IgQtColorScale(this);
    //creates color scale slot in Misc pull down window    


	wireframeId = insertItem("Wireframe Statics",this,SLOT(wireframeSlot()));
	setItemChecked(wireframeId,false);
	setItemEnabled(wireframeId,true);

	cutawayId = insertItem("Cutaway Statics",this,SLOT(cutawaySlot()));
	setItemChecked(cutawayId,false);
	setItemEnabled(cutawayId,true);

	insertItem("Optimise Statics",this,SLOT(optimiseSlot()));

	infoId = insertItem("Info",this, SLOT(infoCtlSlot()));
	setItemChecked(infoId,false);
	setItemEnabled(infoId,false);
    if (theBrowser) {
		IgQtBrowser *b;
		try {
			b  = dynamic_cast<IgQtBrowser*>(viewer);
			if (b) setItemEnabled(infoId,true);
		}
		catch (std::bad_cast){
		}
		
    }
}

IgQtMiscCtl::~IgQtMiscCtl()
{
    //	delete zDialog;
}

void IgQtMiscCtl::zSliceSlot()
{
    zDialog->show();
}
void IgQtMiscCtl::scaleSlot()
{
	scaleDialog->show();
}

void IgQtMiscCtl::bkgColorSlot()
// this will connect the background color slot with the qcolordialog box
// and the newly chosen background color
{    
    float redBg;
    float greenBg;
    float blueBg;
    
    theBrowser->getSceneManager()->getBackgroundColor().getValue( redBg, greenBg, blueBg );
    // SceneManager controls the entire window,
    // and thus the background color
    
    QColor old( int( redBg * 255 ), int( greenBg * 255 ), int( blueBg * 255 ));
    // must convert SbColor back into a QColor
    // for use in QColorDialog
  
    QColor c = QColorDialog::getColor( old, this );
    if( c.isValid())
    {
	theBrowser->getSceneManager()->setBackgroundColor
	    ( SbColor( c.red() / 255. , c.green() / 255. , c.blue() / 255. ));
	// this connects the newly chosen background color with the SceneManager,
	// changing the background color
	// the if statement allows you to cancel the dialog without an invalid
	// QColor being returned
    }
}

void IgQtMiscCtl::colorScaleSlot()
{
    colorScaleDialog->show();  
}

void IgQtMiscCtl::infoCtlSlot()
{
    // get and toggle current status
    bool on = !isItemChecked(infoId);
    setItemChecked(infoId,on);
    dynamic_cast<IgQtBrowser*>(theBrowser)->infoOverlay(on);
}


void IgQtMiscCtl::wireframeSlot()
{
	bool on = !isItemChecked(wireframeId);
	setItemChecked(wireframeId,on);
	// emit
	wireframeSignal(on);

}
void IgQtMiscCtl::cutawaySlot()
{
	bool on = !isItemChecked(cutawayId);
	setItemChecked(cutawayId,on);
	// emit
	cutawaySignal(on);

}
void IgQtMiscCtl::optimiseSlot()
{
	// emit
	optimiseSignal(true);

}









