#include "QtGUI/Projectors.h"

/** Helper method to do RZ projections.
    
    @a v the vector to be transformed.

  */
static SbVec3f
projectRZ(const SbVec3f &v)
{
  double sign = v[1] / fabs(v[1]);
  double size = sqrt(v[0]*v[0]+ v[1]*v[1]);
  return SbVec3f(0, sign * size, v[2]);
}
/** Helper method to do RZ projections. It transforms the the vector @a v
    with the same kind of projection that would be used for the vector @a s.
    
    @a v the vector to be transformed.
    
    @a s the vector which defines which map to use for the transformation.
  */
static SbVec3f
projectRZAs(const SbVec3f &v, const SbVec3f &s)
{
  double size = sqrt(v[0]*v[0] + v[1]*v[1]);
  if (s[1] < 0.)
    return SbVec3f(0, -size, v[2]);
  return SbVec3f(0, size, v[2]);
}

/** Helper method to do RZ projections. It transforms the the vector @a v
    with the same kind of projection that would be used for the vector @a s.
    The difference with the above is that the Z coordinate is set according
    to the angle, to make sure that we don't end up with degenerated volumes.
    This is handy to project rechits and similar things.
    
    @a v the vector to be transformed.
    
    @a s the vector which defines which map to use for the transformation.
  */
static SbVec3f
projectRZAsWithOffset(const SbVec3f &v, const SbVec3f &s)
{
  double size = sqrt(v[0]*v[0] + v[1]*v[1]);
  double angle = atan2(v[0], v[1]);

  if (s[1] < 0.)
    return SbVec3f(-angle, -size, v[2]);

  return SbVec3f(angle, size, v[2]);
}


/** Helper method to do the RZ projection in the case of coordinates passed 
    in terms of RHO, THETA, PHI */
static SbVec3f
prejectRZThetaPhi(const SbVec3f &v)
{
  double theta = v[1];
  double phi = v[2];
  
  double x = cos(theta) * sin(phi);
  double y = sin(theta) * sin(phi);
  double z = cos(theta);

  double sign = y / fabs(y);
  double size = sqrt(x*x + y*y);
  
  return SbVec3f(v[0], acos(z), atan2(sign*size, 0));
}

/** Helper method for the identity projection.
    
    @a v the vector to be transformed.
    
    @return the transformed vector
  */
static SbVec3f
identity(const SbVec3f &v)
{
  return v;
}

static SbVec3f
identityAs(const SbVec3f &v, const SbVec3f &)
{
  return v;
}

static SbVec3f
identityAsWithOffset(const SbVec3f &v, const SbVec3f &)
{
  return v;
}


static SbVec3f
identityThetaPhi(const SbVec3f &v)
{
  return v;
}

/** Custom projection for sliced views
  */

/** Helper structure to keep track of slices.
*/
struct ZSlice
{
  ZSlice(double dZMin, double dZMax, double dX, double dY)
  :zMin(dZMin), zMax(dZMax), x(dX), y(dY)
  {
  }
  
  double zMin, zMax, x, y;
};
ZSlice slices[15] = { ZSlice(-20.96, -10.83, 20 * -7, 0),
                     ZSlice(-10.83, -9.750, 20 * -6, 0),
                     ZSlice(-9.750, -8.495, 20 * -5, 0),
                     ZSlice(-8.495, -7.240, 20 * -4, 0),
                     ZSlice(-7.240, -6.610, 20 * -3, 0),
                     ZSlice(-6.610, -3.954, 20 * -2, 0),
                     ZSlice(-3.954, -1.268, 20 * -1, 0),
                     ZSlice(-1.268,  1.268, 20 *  0, 0),
                     ZSlice( 1.268,  3.954, 20 *  1, 0),
                     ZSlice( 3.954,  6.610, 20 *  2, 0), 
                     ZSlice( 6.610,  7.240, 20 *  3, 0),
                     ZSlice( 7.240,  8.495, 20 *  4, 0),
                     ZSlice( 8.495,  9.750, 20 *  5, 0),
                     ZSlice( 9.750,  10.83, 20 *  6, 0),
                     ZSlice( 10.83,  20.96, 20 *  7, 0)};

static SbVec3f
projectMuonSliced(const SbVec3f &v)
{
  for (size_t i = 0; i < 15; i++)
  {
    ZSlice &slice = slices[i];
    if (v[2] > slice.zMin && v[2] < slice.zMax)
      return SbVec3f(slice.x + v[0], slice.y + v[1], v[2]);
  }
  // Clip whatever is outside the slices.
  return SbVec3f(v[0], v[1], -1000);
}

static SbVec3f
projectMuonSlicedAs(const SbVec3f &v, const SbVec3f &s)
{
  for (size_t i = 0; i < 15; i++)
  {
    ZSlice &slice = slices[i];
    if (s[2] > slice.zMin && s[2] < slice.zMax)
      return SbVec3f(slice.x + v[0], slice.y + v[1], v[2]);
  }
  // Clip whatever is outside the slices.
  return SbVec3f(v[0], v[1], 10000);
}

/** Return the projectors associated to @a projectionType 
    
    @a projectionType the symbolic name of the projection. For the time being
     "identity",  "RZ" and "muonslices".
    
    @a projectors structure to be filled in with the requested projectors.

    For the time being adding a new projection should be simply a matter of 
    specifing new projectors here and the specifing it in the IML.
  */
void
getProjectors(const std::string &projectionType, Projectors &projectors)
{
  // Select which projectors to add. Adding a new projection should be simply
  // a matter of specifing new projectors here.
  if (projectionType == "identity")
  {
    projectors.project = identity;
    projectors.projectThetaPhi = identityThetaPhi;
    projectors.projectAs = identityAs;
    projectors.projectAsWithOffset = identityAsWithOffset;
  }
  else if (projectionType == "RZ")
  {
    projectors.project = projectRZ;
    projectors.projectThetaPhi = prejectRZThetaPhi;
    projectors.projectAs = projectRZAs;
    projectors.projectAsWithOffset = projectRZAsWithOffset;
  }
  else if (projectionType == "muonslices")
  {
    projectors.project = projectMuonSliced;
    projectors.projectThetaPhi = projectMuonSliced;
    projectors.projectAs = projectMuonSlicedAs;
    projectors.projectAsWithOffset = projectMuonSlicedAs;    
  }
}