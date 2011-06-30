#ifndef IG_OPEN_INVENTOR_IG_SO_SPLINE_TRACK_H
# define IG_OPEN_INVENTOR_IG_SO_SPLINE_TRACK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "QtGUI/IgSoShapeKit.h"
# include <Inventor/fields/SoMFVec3f.h>
# include <Inventor/nodes/SoGroup.h>
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
//namespace {
class RootFunction;
//}
/** \class IgDrawSplinesHelper IgDrawSplinesHelper.h Iguana/QtGUI/src/IgDrawSplinesHelper.h
  
    \brief Provides an helper class to create spline-based tracks which requires the
     knowledge of the trajectory and its slope at two separate points.
     
     Based on code by G. Alverson.
  
    \author G. Eulisse
    \author G. Alverson
    \date 20 July 1999
  
 */
class IgDrawSplinesHelper
{
public:
  virtual void create(SoGroup *sep, SoMFVec3f &points, SoMFVec3f &tangents);
protected:
  virtual float findRoot(const RootFunction func, float x1, float x2, 
                         float xacc, bool &err);
  
  // meat of the algorithm: given a pair of point/tangents, adds control 
  // points for the appropriate locations between the pair.
  virtual void midpoint(const SbVec3f pa, const SbVec3f pb, 
                        const SbVec3f ta, const SbVec3f tb,
                        int &ik, int &ik_count, int &ipo, 
                        std::vector<SbVec4f> &controlPoints, 
                        std::vector<float> &knotVals);
private:
  static const int  NORDER; // order of NurbsCurve
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_SPLINE_TRACK_H