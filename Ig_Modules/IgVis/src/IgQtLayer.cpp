#include "Ig_Modules/IgVis/interface/IgQtLayer.h"
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include "qtextstream.h"

const float IgQtLayer::defaultLayerSpacing = 1.0F;
SoTranslation* IgQtLayer::interlayerSpace = NULL;
IgQtLayer::IgQtLayer(SoSeparator *sep)
{
	if (sep)
	{
	    root = sep;
	}
	else
	{
	    root = new SoSeparator;
	}
	if (!interlayerSpace)
	{
		interlayerSpace = new SoTranslation;
		interlayerSpace->translation = SbVec3f(0.F,0.F,defaultLayerSpacing);
	}
	interlayerSpace->ref();

	layerTranslation = new SoTranslation;
	layerTranslation->translation.setValue(0.F,0.F,defaultLayerSpacing); //starting translation
	layerScale = new SoScale;
	layerScale->scaleFactor.setValue(1.F,1.F,1.F); //starting scale factor
	root->addChild(layerScale);
	root->addChild(layerTranslation);
}
IgQtLayer::~IgQtLayer()
{
	if (interlayerSpace)
	{
		interlayerSpace->unref();
	}
}
void IgQtLayer::addLayer(IgQtLayer *layer)
{
	root->addChild(interlayerSpace);
	root->addChild(layer->root);
	list_of_layers.push_back(layer);
}
void IgQtLayer::add2dChild(SoNode* newNode)
{
	root->addChild(interlayerSpace);
	root->addChild(newNode);
}
void IgQtLayer::resize()
{
    // make sure all child layers are ok first
    for (std::vector<IgQtLayer*>::iterator it = list_of_layers.begin(); it != list_of_layers.end(); it++)
    {
	(*it)->resize();
    }
    // now adjust overall depth of layer 
    root->ref(); // SoNode/Path must be ref'd before applying an action
    SbViewportRegion vr;
    SoGetBoundingBoxAction bb(vr);
    bb.apply(this->root);
    SbBox3f bbox = bb.getBoundingBox();

    float dMin, dMax, span; //min/max distance along direction
    if (!bbox.isEmpty())
    {
	bbox.getSpan(SbVec3f(0.F,0.F,1.F), dMin, dMax);
	span = dMax - dMin;
    }
    else
    {
	span = 1.0F;
	dMin = 0.F;
	dMax = 1.F;
    }

    SbVec3f translation = layerTranslation->translation.getValue();
    layerTranslation->translation.setValue(translation +  SbVec3f(0.F,0.F,defaultLayerSpacing -dMin));
    if ( span > defaultLayerSpacing )
    {
	SbVec3f scale = layerScale->scaleFactor.getValue();
	scale[2] /= span; // reset only z
	layerScale->scaleFactor.setValue(scale);
    }
    root->unref();
}
 void IgQtLayer::addChild(SoNode *node)
{
    root->addChild(node);
}
