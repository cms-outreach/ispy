#ifndef IGUANA_PROJECTORS_H
# define IGUANA_PROJECTORS_H

//<<<<<< INCLUDES                                                       >>>>>>
# include <Inventor/SbLinear.h>
# include <cmath>
# include "Framework/IgLinearAlgebra.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

typedef SbVec3f(*Projector)(IgV3d &);
typedef SbVec3f(*ProjectorThetaPhi)(IgV3d &);
typedef SbVec3f(*ProjectorAsPoint)(IgV3d &, IgV3d &);

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

struct Projectors
{
  /** The projector function to be used for point-like quantities. */
  Projector project;
  /** Transforms coordinates when they are given in RHO, THETA, PHI terms */
  ProjectorThetaPhi projectThetaPhi;
  /** The projector to be used for transforming track associated quantities.

      The reason why this is separated from the rest is because we need to
      work around discontinuities of the map which does the projection.
      
      The first argument is the point to be projected, the second argument
      is a point which  defines which map should be used for the projection.
  */
  ProjectorAsPoint projectAs;
  ProjectorAsPoint projectAsWithOffset;
};

void getProjectors(const std::string &projectionType, Projectors &projectors);

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif
