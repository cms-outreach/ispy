#ifndef IG_OPEN_INVENTOR_IG_SO_GRID_PLANE_MAP_H
# define IG_OPEN_INVENTOR_IG_SO_GRID_PLANE_MAP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include "Iguana/Inventor/interface/IgSoGridPlane.h"
# include <Inventor/nodes/SoShape.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/fields/SoSFPlane.h>
# include <Inventor/fields/SoSFUInt32.h>
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFColor.h>
# include <Inventor/SbLinear.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgSoGridPlane;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A planar slice (e.g. of volumetric data) drawn as a colour map.  */
class IG_OPEN_INVENTOR_API IgSoGridPlaneMap : public SoShape
{
	SO_NODE_HEADER (IgSoGridPlaneMap);
public:
	SoSFBool   on;
	SoSFPlane  plane;
	SoSFFloat  xOrigin;
	SoSFFloat  zOrigin;
	SoSFFloat  xLen;
	SoSFFloat  zLen;
	SoSFFloat  spacing;
	SoSFColor  color;
	SbVec3f    labelSpot[3];
	SoSFBool   refreshScale;

	IgSoGridPlaneMap (void);
	// implicit copy constructor
	// implicit assignment operator
	
	static void initClass (void);
	void        setCaller (IgSoGridPlane* caller);

protected:
	virtual ~IgSoGridPlaneMap (void);
	
	virtual void computeBBox (SoAction *, SbBox3f &box, SbVec3f &center);
	virtual void GLRender (SoGLRenderAction *renderAction);
	virtual void generatePrimitives (SoAction *action);

private:
	SbVec3f        m_curCorner[3];
	IgSoGridPlane* m_gridPlane;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_GRID_PLANE_MAP_H
