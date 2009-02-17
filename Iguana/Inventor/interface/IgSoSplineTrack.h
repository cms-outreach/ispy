#ifndef IG_OPEN_INVENTOR_IG_SO_SPLINE_TRACK_H
# define IG_OPEN_INVENTOR_IG_SO_SPLINE_TRACK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoMFVec3f.h>
# include <Inventor/SbLinear.h>
# include <vector>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>
namespace {
class RootFunction;
}
/*! \class IgSoSplineTrack IgSoSplineTrack.h Iguana/Inventor/interface/IgSoSplineTrack.h
 *
 *  \brief Provides a spline-based track class which requires the
 *   knowledge of the trajectory and its slope at two separate points.
 *
 *  \author G. Alverson
 *  \date 20 July 1999
 * 
 */
class IG_OPEN_INVENTOR_API IgSoSplineTrack : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoSplineTrack);
    SO_KIT_CATALOG_ENTRY_HEADER (coordinates);
    SO_KIT_CATALOG_ENTRY_HEADER (curve);
public:
    IgSoSplineTrack (void);
    static void initClass (void);

    SoMFVec3f points;		//< the points used for the track
    SoMFVec3f tangents;		//< the tangents at those points

protected:
    // re-implement refresh from base class
    virtual void refresh (void);
    
    // meat of the algorithm: given a pair of point/tangents, adds control points for the appropriate locations between the pair
    virtual void midpoint (const SbVec3f pa, const SbVec3f pb, const SbVec3f ta, const SbVec3f tb,
	int &ik, int &ik_count, int &ipo, std::vector<SbVec4f> &controlPoints, std::vector<float> &knotVals);
    // utility function used in determining midpoint: based on {/it Numerical Recipies} routine
    float findRoot (const RootFunction func, float x1, float x2, float xacc, bool &err);

private:
    static const int	NORDER; // order of NurbsCurve
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_SPLINE_TRACK_H

