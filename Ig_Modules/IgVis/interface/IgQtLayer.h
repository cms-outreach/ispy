#if !defined(IG_QT_LAYER_INCLUDED_)
#define IG_QT_LAYER_INCLUDED_


#include "Ig_Modules/IgVis/interface/config.h"
#include <Inventor/nodes/SoSeparator.h>
#include <vector>
#include "qstring.h"

class SoTranslation;
class SoScale;
class IgQtTwig2D;


/*
 *	\class IgQtLayer IgQtLayer.h Ig_Modules/IgVis/interface/IgQtLayer.h
 *  \brief An object which manages 2D layers, each of which is scaled and translated
 *   so as to not interpenetrate and to be at distinct z positions to avoid z-buffer
 *   problems. As objects or additional layers are added the height of the layer shrinks to match.
 *
 *   Developed as full-fledged class after discussions with Todd Adams of FSU
 *
 *
 *   Structure of a layer:
 *
 *         SoSeparator
 *            |
 *        ------------------------------------------------------------------------------------------------------------ 
 *        |                                    |                        |               [   |                         | ...]
 *      SoScale (to fit in standard size)   SoTranslation (in z)    (Optional Content)    SoTranslation (in z)     SoSeparator (add'l layers)
 *
 *  \author G. Alverson
 *  \date 15-Jan-2002
 *
 */

class IG_VIS_API IgQtLayer
{
public:
	//! constructor takes optional SoSeparator argument
        //! for when an SoSelection is desired instead of the default
	explicit IgQtLayer(SoSeparator *sep=NULL);
	virtual ~IgQtLayer();
	//! add a sub-layer to this one
	virtual void addLayer(IgQtLayer *layer);
	//! add some SoObject which requires spacing (otherwise use addChild)
	virtual void add2dChild(SoNode *node);
	//! recalculate scale and offset
	void resize();
	//! makes a layer look like a sep
	void addChild(SoNode* node);
protected:
	//! IgQtTwig2D needs to make some special tweaks
	friend class IgQtTwig2D;
	//! the interlayer spacer
	static SoTranslation *interlayerSpace;
private:
	//! the root node of the layer, normally a separator
	SoSeparator *root;
	//! the spacing (before scaling) between layers (must be consistent between instances)
	static const float defaultLayerSpacing;
	//! translation between adjacent layers
	SoTranslation *layerTranslation; 
	//! overall scale factor to fit all the layers within the defaultLayerSpacing
	SoScale *layerScale;
	//! layer name
	QString layerName;
	//! the layers attached to this one
	std::vector<IgQtLayer*> list_of_layers;
};
#endif // IG_QT_LAYER_INCLUDED_











