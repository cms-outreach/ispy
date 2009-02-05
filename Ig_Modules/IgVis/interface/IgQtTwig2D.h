// IgQtTwig2D.h: interface for the IgQtTwig2D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGQTTWIG2D_H__33CF3408_504F_494F_970F_1DC61342BB64__INCLUDED_)
#define AFX_IGQTTWIG2D_H__33CF3408_504F_494F_970F_1DC61342BB64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Modules/IgVis/interface/config.h"
#include "Ig_Modules/IgVis/interface/IgQtTwig.h"

/*!
 * \class IgQtTwig2D IgQtTwig2D.h Ig_Modules/IgVis/interface/IgQtTwig2D.h
 *
 * \brief A twig which manages twigs which are two-dimensional. Emulates the 
 * basic functionality of typical 2D drawing packages.
 *
 * \author G. Alverson, Northeastern University Experimental Particle Physics
 * \date 6 December 2001
 *
 */

class IgQtLayer;

class IG_VIS_API IgQtTwig2D : public IgQtTwig  
{
public:
	//! c-tors same as for IgQtTwig
	explicit IgQtTwig2D(IgQtTwig *parent, SbString name, bool visibility = true, bool cascade = true);
	//! alternate c-tor
	explicit IgQtTwig2D(SbString name, bool visibility = true, bool cascade = true);
	//! d-tor
	virtual ~IgQtTwig2D();
	//! add a (2D) layer to the scene managed by this twig.
	void addLayer(IgQtLayer* layer);
	//! Add a 3D child. Will add a resizing adapter ahead of the twig.
	virtual void addChild (IgQtTwig *anIgQtTwig);
	//! add a 2D child in normal stacking order.
	void addChild (IgQtTwig2D *anIgQtTwig);
	//! adds an Iv node while stacking in z
	void add2dIvNode(SoNode* newNode);
	//! resize all child twigs and layers
	void resize(void);
	//! over-ride base class to avoid losing extra 2D stuff on change of selection node
	void makeSelectable(void);
	//! over-ride base class to avoid losing extra 2D stuff
	virtual void recreateIvNode(void);
protected:
	//! over-ridden base class so as to invoke a resize operation prior to the draw
	//! If nodes are added during twig construction, rather than through drawMe, this
	//! will not be invoked. Mixing twigs which have content added in the constructor with
	//! those built with drawMe's is a bad idea as a dynamic change can not propagate through
	//! a node with static content.
	virtual void drawMe (void);
private:
	IgQtLayer *theLayer;
};

#endif // !defined(AFX_IGQTTWIG2D_H__33CF3408_504F_494F_970F_1DC61342BB64__INCLUDED_)
