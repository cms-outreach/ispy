#ifndef IGQTSLICERIMPL_H
#define IGQTSLICERIMPL_H
#include "IgQtSlicer.h"

#include <Inventor/SbLinear.h>

class SoGroup;
class SoClipPlane;
class QDoubleValidator;

/*!  \class IgQtSlicerImpl IgQtSlicerImpl.h  IgQtSlicerImpl.h
 *   \brief Implements the IgQtSlicer class (which was machine produced using Qt Designer).
 *
 *   Provides a working slice dialog and slicer based on clip planes.
 *
 *   \author George Alverson, Northeastern University
 *   \date  22 November 2000
 */
class IG_VIS_API IgQtSlicerImpl : public IgQtSlicer
{ 
    Q_OBJECT

public:
	//! c-tor requires an SoNode on which to hang the slicers
    IgQtSlicerImpl(SoGroup *topNode, QWidget *parent=0);
	//! generic
    ~IgQtSlicerImpl();

public slots:
	//! slot for the center of the slice
    void centerPositionSettingSlot(float);
	//! slot for change of string version of position
    void centerPositionTextChangedSlot();
	//! slot for change of slice axis
    void axisChanged(int);
	//! enable/disable slot for slice
    void sliceOnSlot(bool);
	//! slot for change of string version of thickness
    void thicknessTextChangedSlot();
	//! slot to set the thickness of the slice
    void thicknessSettingSlot(float);

protected:
	//! reset the locations of the planes
	void resetLocations();

private:
	//! node to hang slicers off of
	SoGroup *root;
	//! near clip
	SoClipPlane *front;
	//! back clip
	SoClipPlane *back;
	//! validator to ensure good edit on the center position text
    QDoubleValidator *centerValidator;
	//! validator to ensure good edit on the thickness text
    QDoubleValidator *thicknessValidator;
	//! center of slice
	float centerPosition;
	//! half-thickness of slice
	float thickness;
	//! axis of slice
	SbVec3f axis;
};

#endif // IGQTSLICERIMPL_H
