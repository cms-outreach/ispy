// IgQtTwig2D.cpp: implementation of the IgQtTwig2D class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtTwig2D.h"
#include "Ig_Modules/IgVis/interface/IgQtLayer.h"

#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoTranslation.h>



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IgQtTwig2D::~IgQtTwig2D()
{
	delete theLayer;
}
IgQtTwig2D::IgQtTwig2D(IgQtTwig *parent, 
					   SbString name, bool visibility, 
					   bool cascade) : 
IgQtTwig(parent, name, visibility, cascade)
{
	theLayer = new IgQtLayer;
	setIvNode(theLayer->root);
}
IgQtTwig2D::IgQtTwig2D(SbString name, 
					   bool visibility, 
					   bool cascade) :
IgQtTwig(name, visibility, cascade)
{
	theLayer = new IgQtLayer(dynamic_cast<SoSeparator*>(getIvNode()));
}
void IgQtTwig2D::addLayer(IgQtLayer *layer)
{
	theLayer->addLayer(layer);
}
void IgQtTwig2D::addChild (IgQtTwig *t)
{
	IgQtTwig2D *adapter = new IgQtTwig2D("Adapter");
	addChild(adapter);
	adapter->IgQtTwig::addChild(t);
}
void IgQtTwig2D::addChild (IgQtTwig2D *anIgQtTwig)
{
        theLayer->root->addChild(IgQtLayer::interlayerSpace);;
	IgQtTwig::addChild(anIgQtTwig);
	theLayer->list_of_layers.push_back(anIgQtTwig->theLayer);
}
void IgQtTwig2D::add2dIvNode(SoNode* newNode)
{
	theLayer->add2dChild(newNode);
}
void IgQtTwig2D::drawMe(void)
{
	// Ensure that a resize operation is performed before a draw. 
	if (getVisibility()) resize();
	IgQtTwig::drawMe();

}
void IgQtTwig2D::resize(void)
{
    // have removed test for dirtySize flag (now removed). Just go through the layer and adjust regardless.
	theLayer->resize();
}
void IgQtTwig2D::makeSelectable(void)
{
    // can't leave normal method floating around as it will remove the 2D info
    SoSelection *select = new SoSelection;
    for (int i=0; i < theLayer->root->getNumChildren(); i++)
    {
	select->addChild(theLayer->root->getChild(i));
    }
    theLayer->root->removeAllChildren();
    theLayer->root = select;
    setIvNode(select);
    // old root should get collected unless it's been ref'd, which it shouldn't have been
}
void
IgQtTwig2D::recreateIvNode (void)
{
    delete theLayer;
    theLayer = new IgQtLayer;
    theLayer->root->setName(validateName(getName()));
    setIvNode(theLayer->root);

}
