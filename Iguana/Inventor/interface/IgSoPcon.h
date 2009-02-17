#ifndef IG_OPEN_INVENTOR_IG_SO_PCON_H
# define IG_OPEN_INVENTOR_IG_SO_PCON_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFVec3f.h>
# include <Inventor/fields/SoMFVec3f.h>

#include <vector>

# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/*!   \class IgSoPcon 
 *    \brief IgSoPcon produces an OIV version of the GEANT3 PCON shape
 *
 *     This particular version has five fields which control the shape
 *	   which can be supplied or a set of arguments (following the GEANT3
 *	   version) can be given via the setPconPts method.
 *
 *     The representation is based on two NURB sufaces, one each for the inner
 *	   and outer surfaces of revolution, plus two NURB surfaces for the end faces and
 *	   two face sets for the side surfaces in case the full 2 PI of revolution is not covered.
 *
 *     Since TGS-based NURB surfaces average surface normals, the junctures between regions of different r/z
 *	   slope seem blurry. This was deemed less troublesome than either including a separate surface for 
 *	   each region or replacing a NURB surface by some less adaptive surface function.
 *
 *     The MF field arguments are cached: to change the shape the field must be replaced.
 *
 *     Textures are not currently supported.
 *
 *     \author George Alverson, CMS/Northeastern University Software Group
 *	   \date   April 2002
 *     Original HEPVis version, June 1998
 */
class IG_OPEN_INVENTOR_API IgSoPcon : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoPcon);
    SO_KIT_CATALOG_ENTRY_HEADER (translation);
    SO_KIT_CATALOG_ENTRY_HEADER (outerCoords);
    SO_KIT_CATALOG_ENTRY_HEADER (outerSurface);
    SO_KIT_CATALOG_ENTRY_HEADER (innerCoords);
    SO_KIT_CATALOG_ENTRY_HEADER (innerSurface);
    SO_KIT_CATALOG_ENTRY_HEADER (topCoords);
    SO_KIT_CATALOG_ENTRY_HEADER (topSurface);
    SO_KIT_CATALOG_ENTRY_HEADER (bottomCoords);
    SO_KIT_CATALOG_ENTRY_HEADER (bottomSurface);
    SO_KIT_CATALOG_ENTRY_HEADER (side1Rot);
    SO_KIT_CATALOG_ENTRY_HEADER (side1Hints);
    SO_KIT_CATALOG_ENTRY_HEADER (side1Face);
    SO_KIT_CATALOG_ENTRY_HEADER (side2Rot);
    SO_KIT_CATALOG_ENTRY_HEADER (side2Hints);
    SO_KIT_CATALOG_ENTRY_HEADER (side2Face);

public:
    IgSoPcon (void);
    static void initClass (void);

    SoMFVec3f vertices;		//< set of points at the breaks
    SoSFVec3f center;		//< translation of the vtxPts relative to the origin
    SoSFFloat phiStart;		//< starting angle of the region of revolution (in radians)
    SoSFFloat phiDelta;		//< extent of revolution

    //! this initialisation method matches the old definition
    void initialise (const int nzee, const float phi, const float delPhi, 
		     const float *zvals, const float *rmin, const float *rmax);

    //! this initialisation method matches the equivalent one in IgSoRotSolid
    void makePcon (const std::vector<float> &zvals, 
			  const std::vector<float> &rmin, 
			  const std::vector<float> &rmax,
			  float phiStart = 0, 
			  float phiDelta = 2*M_PI,
			  int divisions = -1);

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
inline void IgSoPcon::makePcon (const std::vector<float> &zvals, const std::vector<float> &rmin,
			   const std::vector<float> &rmax, float phiStart, float phiDelta,
			   int divisions)
{ initialise (static_cast<int>( zvals.size() ), phiStart, phiDelta, &zvals[0], &rmin[0], &rmax[0]); }
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_PCON_H
