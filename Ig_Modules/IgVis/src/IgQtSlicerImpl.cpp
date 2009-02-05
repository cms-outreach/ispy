#include "Ig_Modules/IgVis/interface/IgQtSlicerImpl.h"

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoUnits.h>

#include "Ig_Extensions/IgQt/interface/IgSoQtThumbWheel.h"

#include <qcheckbox.h>
#include <qvalidator.h>
#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qbutton.h>

IgQtSlicerImpl::IgQtSlicerImpl( SoGroup *topNode, QWidget *parent )
    : IgQtSlicer(parent), root(topNode), centerPosition(0.F), thickness(0.5F)
{
	//defaults 
	int maxHalfRange = 5;

	SliceCenterThumbwheel->setRange(-maxHalfRange,maxHalfRange); // +/- in meters
	SliceCenterThumbwheel->setValue(centerPosition);
	SliceCenterThumbwheel->setOrientation(IgSoQtThumbWheel::Horizontal);
	SliceCenterThumbwheel->setRangeBoundaryHandling(IgSoQtThumbWheel::ACCUMULATE);

	SliceThicknessThumbwheel->setRange(0,2*maxHalfRange);
	SliceThicknessThumbwheel->setValue(thickness);
	SliceThicknessThumbwheel->setOrientation(IgSoQtThumbWheel::Horizontal);
	SliceThicknessThumbwheel->setRangeBoundaryHandling(IgSoQtThumbWheel::ACCUMULATE);

//  Note: originally put in validators and range to enforce reasonable behaviour, but they 
//  don't appear to be working as intended. Behaviour is acceptable, so we're not going
//  to mess with this for now, even if we do have negative slice thickness.
	QString s;
	centerValidator = new QDoubleValidator(-maxHalfRange,maxHalfRange,2,0);

	SliceCenterText->setValidator(centerValidator);
	s.setNum(centerPosition,'f',2);
	SliceCenterText->validateAndSet(s,0,0,0);

	thicknessValidator = new QDoubleValidator(0.f,2*maxHalfRange,2,0);
	SliceThicknessText->setValidator(thicknessValidator);
	s.setNum(1.0F,'f',2);
	SliceThicknessText->validateAndSet(s,0,0,0);


	front = new SoClipPlane;
	front->on = false;
	back = new SoClipPlane;
	back->on = false;

	topNode->addChild(front);
	topNode->addChild(back);

	SliceAxisGroup->setButton(SliceAxisGroup->id(zAxisButton)); // default slice is on z axis
	axisChanged(SliceAxisGroup->id(zAxisButton));
}

/*  
 *  Destroys the object and frees any allocated resources
 */
IgQtSlicerImpl::~IgQtSlicerImpl()
{
    // no need to delete child widgets, Qt does it all for us
	// OIV takes care of So children.
	delete thicknessValidator;
	delete centerValidator;
}

/* 
 * public slot
 */
void IgQtSlicerImpl::centerPositionSettingSlot(float z)
{
	centerPosition = z;
	QString s;
	s.setNum(centerPosition,'f',2);
	SliceCenterText->validateAndSet(s,0,0,0);
    resetLocations(); 
}
/* 
 * public slot
 */
void IgQtSlicerImpl::centerPositionTextChangedSlot()
{
	QString s = SliceCenterText->text();
	bool check;
	float z = s.toFloat(&check);
	if (check)
	{
		centerPosition = z;
		SliceCenterThumbwheel->setValue(centerPosition);
        resetLocations();
	}
}
/* 
 * public slot
 */
void IgQtSlicerImpl::sliceOnSlot(bool set)
{
	front->on = set;
	back->on = set;
}
/* 
 * public slot
 */
void IgQtSlicerImpl::thicknessTextChangedSlot()
{
	QString s = SliceThicknessText->text();
	bool check;
	float z = s.toFloat(&check);
	if (check)
	{
		thickness = z;
		SliceThicknessThumbwheel->setValue(thickness);
        resetLocations();
	}
}
/* 
 * public slot
 */
void IgQtSlicerImpl::thicknessSettingSlot(float z)
{
    thickness = z;
	QString s;
	s.setNum(thickness,'f',2);
	SliceThicknessText->validateAndSet(s,0,0,0);
    resetLocations(); 
}

void IgQtSlicerImpl::resetLocations()
{
	front->plane = SbPlane(axis,-centerPosition-thickness/2);
	back->plane = SbPlane(-axis,centerPosition-thickness/2);
}

/* 
 * public slot
 */
void IgQtSlicerImpl::axisChanged(int axisDirection)
{
	// IDs follow order of definition in GUI builder (which might change) so can't use case
	// negative due to orientation of clip planes
	if (axisDirection == SliceAxisGroup->id(xAxisButton)) {
		axis = SbVec3f(-1.F,0.F,0.F); }
	else if (axisDirection == SliceAxisGroup->id(yAxisButton)) {
		axis = SbVec3f(0.F,-1.F,0.F);}
	else if (axisDirection == SliceAxisGroup->id(zAxisButton)) {
		axis = SbVec3f(0.F,0.F,-1.F);}
	else {
//		assert(false); //should never occur
	}
#if 0
 	switch (axisDirection)
	{
	case 0: //x
		{axis = SbVec3f(-1.F,0.F,0.F);}
		break;
	case 1: //y
		{axis = SbVec3f(0.F,-1.F,0.F);}
		break;
	case 2: //z
		{axis = SbVec3f(0.F,0.F,-1.F);}
		break;
	}
#endif
	resetLocations();
}
			
