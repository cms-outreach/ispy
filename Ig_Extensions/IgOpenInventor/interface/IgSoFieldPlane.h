#ifndef IG_OPEN_INVENTOR_IG_SO_FIELD_PLANE_H
# define IG_OPEN_INVENTOR_IG_SO_FIELD_PLANE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Extensions/IgOpenInventor/interface/config.h"
# include <Inventor/SbBox.h>
# include <Inventor/nodes/SoSwitch.h>
# include <Inventor/fields/SoSFPlane.h>
# include <Inventor/fields/SoSFUInt32.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/fields/SoSFEnum.h>
# include <Inventor/fields/SoSFFloat.h>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgSoPlaneManip;
class IgSoFieldPlaneMap;
class SoIndexedLineSet;
class SoSensor;
class SoFieldSensor;
class IgSbField;
class IgSbColorMap;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif // WIN32

/** An Inventor representing a an arbitrary planar slice of a field.  */
class IG_OPEN_INVENTOR_API IgSoFieldPlane : public SoSwitch
{
    SO_NODE_HEADER (IgSoFieldPlane);
public:
    enum Component {
	XYZ_ALL,	//< Total magnitude
	XYZ_X,		//< Magnitude in x-axis
	XYZ_Y,		//< Magnitude in y-axis
	XYZ_Z,		//< Magnitude in z-axis
	XYZ_PHI,	//< Magnitude in angle (cylindrical)
	XYZ_R		//< Magnitude in radius (cylindrical)
    };

    SoSFPlane		plane;
    SoSFUInt32		mapDensityX;
    SoSFUInt32		mapDensityZ;
    SoSFUInt32		segRatioX;
    SoSFUInt32		segRatioZ;
    SoSFEnum		component;
    SoSFFloat		maxvalue;
    SoSFUInt32		invisible;
    SoSFUInt32		alpha;
    SoSFBool		show;
    SoSFBool		manip;
    SoSFBool		showMap;
    SoSFBool		showSegments;
    SoSFBool		gradientScale;
    // SoSFBool		showContours;
    // SoSFBool		showSurfaces;

    IgSoFieldPlane (void);
    // implicit copy constructor
    // implicit assignment operator

    static void		initClass (void);
    
    void		colorMap (const IgSbColorMap *map);
    void		field (const IgSbField *field, SbBox3f world);

protected:
    virtual ~IgSoFieldPlane (void);

    virtual void	attachSensors (void);
    virtual void	detachSensors (void);
    
private:

    void		buildStructure (void);
    void		update (void);
    void		updateCorners (void);
    void		refreshColors (void);
    void		resampleSegments (void);

    static void		showChanged  (void *me, SoSensor *sensor);
    static void		showMapChanged  (void *me, SoSensor *sensor);
    static void		showSegmentsChanged  (void *me, SoSensor *sensor);
    static void		gradientScaleChanged (void *me, SoSensor *sensor);
    static void		planeChanged  (void *me, SoSensor *sensor);
    static void		mplaneChanged (void *me, SoSensor *sensor);
    static void		resampleSegmentsCB (void *me, SoSensor *sensor);

    const IgSbColorMap	*m_cmap;
    const IgSbField	*m_field;
    SbBox3f		m_world;
    IgSoPlaneManip	*m_manip;
    IgSoFieldPlaneMap	*m_fieldPlane;
    SoIndexedLineSet	*m_segments;
    SoGroup		*m_gradientScale;
    SoFieldSensor	*m_showSensor;
    SoFieldSensor	*m_planeSensor;
    SoFieldSensor	*m_mapDensityXSensor;
    SoFieldSensor	*m_mapDensityZSensor;
    SoFieldSensor	*m_segRatioXSensor;
    SoFieldSensor	*m_segRatioZSensor;
    SoFieldSensor	*m_componentSensor;
    SoFieldSensor	*m_maxvalueSensor;
    SoFieldSensor	*m_invisibleSensor;
    SoFieldSensor	*m_alphaSensor;
    SoFieldSensor	*m_mplaneSensor;
    SoFieldSensor	*m_showMapSensor;
    SoFieldSensor	*m_showSegmentsSensor;
    SoFieldSensor	*m_gradientScaleSensor;
    
    std::vector<double> m_colorsMag;
};
#ifdef WIN32
#include <SoWinEnterScope.h>
#endif // WIN32

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_FIELD_PLANE_H
