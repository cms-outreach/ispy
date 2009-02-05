//////////////////////////////////////////////////////////////////////
//
// IgQtViewpoint.cpp: implementation of the IgQtViewpoint class.
//
//////////////////////////////////////////////////////////////////////

#include "Ig_Modules/IgVis/interface/IgQtViewpoint.h"
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/VRMLnodes/SoVRMLViewpoint.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/Qt/viewers/SoQtViewer.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IgQtViewpoint::IgQtViewpoint(SoQtViewer* viewer, const QString vp_name) : name (vp_name)
{
	setParams(viewer->getCamera());
}
IgQtViewpoint::IgQtViewpoint(SoVRMLViewpoint* view)
{
	if (view == NULL) return;

	name = view->description.getValue().getString();
	orientation = view->orientation.getValue();
	position = view->position.getValue();
	type = SoPerspectiveCamera::getClassTypeId();
	focalDistance = 1.0f; // default
	aspectRatio = SO_ASPECT_SQUARE;
	heightAngle = view->fieldOfView.getValue();
	nearDistance = farDistance = 0.0f; // flag to not use

}
void IgQtViewpoint::setViewpoint(SoQtViewer *viewer) 
{

	// make sure we have the same type of camera as we had previously
	if (!viewer->getCameraType().isDerivedFrom(type) ) 
	{
		viewer->setCameraType(type);
	}
	SoCamera *camera = viewer->getCamera();

	// set the camera parameters
	camera->orientation.setValue(orientation);
	camera->position.setValue(position);
	camera->focalDistance.setValue(focalDistance);
	camera->aspectRatio.setValue(aspectRatio);
	if (nearDistance != farDistance) // SoVRMLViewpoint doesn't supply clip locations
	{
	    camera->nearDistance.setValue(nearDistance);
	    camera->farDistance.setValue(farDistance);
	}
#if TGS_VERSION >= 2500 || COIN_MAJOR_VERSION >= 2 // protect against  COIN
	if (type.isDerivedFrom(SoOrthographicCamera::getClassTypeId())) {
		static_cast<SoOrthographicCamera*>(camera)->height.setValue(heightAngle);
	}
	else {
		static_cast<SoPerspectiveCamera*>(camera)->heightAngle.setValue(heightAngle);
	}
#endif
	
}
void IgQtViewpoint::setParams(SoCamera *camera)
{
	if (!camera) {
		return;
	}

	orientation = camera->orientation.getValue();
	position = camera->position.getValue();
	focalDistance = camera->focalDistance.getValue();
	nearDistance = camera->nearDistance.getValue();
	farDistance = camera->farDistance.getValue();
	aspectRatio = camera->aspectRatio.getValue();
	type = camera->getTypeId();
#if TGS_VERSION >= 2500 || COIN_MAJOR_VERSION >= 2 // protect against  COIN

	if (type.isDerivedFrom(SoOrthographicCamera::getClassTypeId())) {
		heightAngle = static_cast<SoOrthographicCamera*>(camera)->height.getValue();}
	else if (type.isDerivedFrom(SoPerspectiveCamera::getClassTypeId())) {
		heightAngle = static_cast<SoPerspectiveCamera*>(camera)->heightAngle.getValue(); }
	else {
		heightAngle = 0.f; // unknown camera type!
	}
#endif
}

IgQtViewpoint::~IgQtViewpoint()
{
}
