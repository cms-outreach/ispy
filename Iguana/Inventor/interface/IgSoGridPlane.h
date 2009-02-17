#ifndef IG_OPEN_INVENTOR_IG_SO_GRIDPLANE_H
# define IG_OPEN_INVENTOR_IG_SO_GRIDPLANE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include <Inventor/nodes/SoGroup.h>
# include <Inventor/nodes/SoLevelOfDetail.h>
# include <Inventor/nodes/SoSeparator.h>
# include <Inventor/nodes/SoShapeHints.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/fields/SoSFPlane.h>
# include <Inventor/fields/SoSFColor.h>
# include <Inventor/sensors/SoSensor.h>
# include <Inventor/sensors/SoFieldSensor.h>
# include "Iguana/Inventor/interface/IgSoGridPlaneMap.h"
# include "Iguana/Inventor/interface/IgSoPlaneManip.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgSoGridPlaneMap;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif // WIN32

class IG_OPEN_INVENTOR_API IgSoGridPlane : public SoGroup
{
    SO_NODE_HEADER (IgSoGridPlane);
public:
	SoSFBool   on;
	SoSFPlane  plane;
	SoSFFloat  xOrigin;
	SoSFFloat  zOrigin;
	SoSFFloat  xLen;
	SoSFFloat  zLen;
	SoSFFloat  spacing;
	SoSFColor  color;
	SoSFColor  labelColor;
	SoSFBool   manip;
	
	static void initClass (void);
	void        updateScale (void);
	
	IgSoGridPlane (void);

protected:
	~IgSoGridPlane (void);

private:
	static void onSensorCB        (void *me, SoSensor *data);
	static void manipSensorCB     (void *me, SoSensor *data);
	static void planeSensorCB     (void *me, SoSensor *data);
	static void mplaneSensorCB    (void *me, SoSensor *data);
	static void xOriginSensorCB   (void *me, SoSensor *data);
	static void zOriginSensorCB   (void *me, SoSensor *data);
	static void xLenSensorCB      (void *me, SoSensor *data);
	static void zLenSensorCB      (void *me, SoSensor *data);
	static void spacingSensorCB   (void *me, SoSensor *data);
	static void colorSensorCB     (void *me, SoSensor *data);
	static void labelColorSensorCB     (void *me, SoSensor *data);
	void        attachSensors     (void);
	void        addScale          (void);
	void        setLabelMove (SoSeparator* labelGroup, SbVec3f step);
	void        setLabelText (SoSeparator* labelGroup, float position, bool right = false);
	
	IgSoGridPlaneMap* m_plane;
	SoShapeHints*     m_shapeHints;
	SoFieldSensor*    m_planeSensor;
	SoFieldSensor*    m_xOriginSensor;
	SoFieldSensor*    m_zOriginSensor;
	SoFieldSensor*    m_xLenSensor;
	SoFieldSensor*    m_zLenSensor;
	SoFieldSensor*    m_spacingSensor;
	SoFieldSensor*    m_onSensor;
	SoFieldSensor*    m_colorSensor;
	SoFieldSensor*    m_labelColorSensor;
	IgSoPlaneManip*   m_manip;
	SoFieldSensor*    m_manipSensor;
	SoFieldSensor*    m_mplaneSensor;
	SoFieldSensor*    m_scaleSensor;
	SoLevelOfDetail*  m_scale;
	SoGroup*          m_gridPlane;
};
#ifdef WIN32
#include <SoWinEnterScope.h>
#endif // WIN32

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_GRIDPLANE_H
