#if !defined(AFX_IGQTVIEWPOINT_H__33EC8130_E0BE_4921_8782_86A61903AE4E__INCLUDED_)
#define AFX_IGQTVIEWPOINT_H__33EC8130_E0BE_4921_8782_86A61903AE4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*!	\class IgQtViewpoint IgQtViewpoint.h Ig_Modules/IgVis/interface/IgQtViewpoint.h
 *
 *  \brief Encapsulates all the information needed to save/restore an OIV scene viewpoint
 *
 *  \author G. Alverson, Northeastern University
 *
 *  \date 08-Sep-2000
 */

#include "Ig_Modules/IgVis/interface/config.h"
#include <Inventor/nodes/SoCamera.h>

#include <qstring.h>


class SoVRMLViewpoint;
class SoQtViewer;

class IG_VIS_API IgQtViewpoint  
{
public:
    //! store the viewpoint (location and orientation) of a camera in a viewer
	explicit IgQtViewpoint(SoQtViewer *viewer, const QString name = QString("Viewpoint"));
    //! store the viewpoint (location and orientation) of an SoVRMLViewpoint
	IgQtViewpoint(SoVRMLViewpoint *view);
	//! set the viewpoint for the scene in the viewer
	void setViewpoint(SoQtViewer *viewer);
	//! d-tor
	virtual ~IgQtViewpoint();
//  Expose all the characteristics required to define a viewpoint
	QString name; //!< name of the viewpoint
	QString label; //!< unique label for the viewpoint
	SbVec3f position; //!< camera position
	SbRotation orientation; //!< camera orientation
	SoType type; //!< camera type: SoPerspective or SoOrthogonal
	float heightAngle; //!< height of view or angle of view (depends on camera type)
	float focalDistance; //!< focal distance for clip planes
	float aspectRatio; //!< aspect ratio
	float nearDistance; //!< distance to near clip plane
	float farDistance; //!< distance to fra clip plane
private:
	void setParams(SoCamera *camera);  //!< set the internal params for this camera
};

#endif // !defined(AFX_IGQTVIEWPOINT_H__33EC8130_E0BE_4921_8782_86A61903AE4E__INCLUDED_)
