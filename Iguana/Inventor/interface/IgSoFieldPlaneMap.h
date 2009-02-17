#ifndef IG_OPEN_INVENTOR_IG_SO_FIELD_PLANE_MAP_H
# define IG_OPEN_INVENTOR_IG_SO_FIELD_PLANE_MAP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include <Inventor/nodes/SoShape.h>
# include <Inventor/fields/SoMFVec3f.h>
# include <Inventor/fields/SoSFUInt32.h>
# include <Inventor/fields/SoMFUInt32.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A planar slice (e.g. of volumetric data) drawn as a colour map.  */
class IG_OPEN_INVENTOR_API IgSoFieldPlaneMap : public SoShape
{
	SO_NODE_HEADER (IgSoFieldPlaneMap);
public:
	SoMFVec3f  corners;
	
	SoMFUInt32 xdivs;
	SoMFUInt32 zdivs;
	SoMFVec3f  lowerLeftCorners;
	SoMFVec3f  upperRightCorners;
	SoMFVec3f  lowerRightCorners;
	SoMFUInt32 orderedRGBA;

	IgSoFieldPlaneMap (void);
	// implicit copy constructor
	// implicit assignment operator
	
	static void initClass (void);

protected:
	virtual ~IgSoFieldPlaneMap (void);
	
	virtual void computeBBox (SoAction *, SbBox3f &box, SbVec3f &center);
	virtual void GLRender (SoGLRenderAction *renderAction);
	virtual void generatePrimitives (SoAction *action);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_FIELD_PLANE_MAP_H
