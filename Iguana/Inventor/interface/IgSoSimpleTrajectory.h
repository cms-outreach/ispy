#ifndef IG_OPEN_INVENTOR_IG_SO_SIMPLE_TRAJECTORY_H
# define IG_OPEN_INVENTOR_IG_SO_SIMPLE_TRAJECTORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoMFVec3f.h>
# include <Inventor/fields/SoSFUShort.h>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/*! \class IgSoSimpleTrajectory
 *  The trajectory is described by a set of 3D points
 *  which are splined together. The spline curve is forced
 *  to pass through each point.
 *
 *  A set of markers may also be drawn.
 */ 
class IG_OPEN_INVENTOR_API IgSoSimpleTrajectory : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoTrajectory);
    SO_KIT_CATALOG_ENTRY_HEADER (style);
    SO_KIT_CATALOG_ENTRY_HEADER (line);
    SO_KIT_CATALOG_ENTRY_HEADER (markers);

public:
    IgSoSimpleTrajectory (void);
    static void initClass (void);

    SoMFVec3f		controlPoints; //< Control points for the curve
    SoMFVec3f		markerPoints;  //< Optional list of points to receive markers
    SoSFFloat		lineWidth;        
    SoSFFloat		pointSize;
    SoSFUShort          linePattern;

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_SIMPLE_TRAJECTORY_H
