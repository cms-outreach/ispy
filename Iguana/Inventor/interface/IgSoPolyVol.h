#ifndef IG_OPEN_INVENTOR_IG_SO_POLY_VOL_H
# define IG_OPEN_INVENTOR_IG_SO_POLY_VOL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFVec3f.h>
# include <Inventor/fields/SoMFVec3f.h>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/*! \class IgSoPolyVol IgSoPolyVol.h Iguana/Inventor/interface/IgSoPolyVol.h
 *  \brief Provides an OpenInventor shape which matches the
 *   description of a D0-type extrusion of a polygonal face.
 *
 *  \author G. Alverson, Northeastern Software Group
 *  \date 16 December 1998
 */
class IG_OPEN_INVENTOR_API IgSoPolyVol : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoPolyVol);
    SO_KIT_CATALOG_ENTRY_HEADER (hints);
    SO_KIT_CATALOG_ENTRY_HEADER (trans);
    SO_KIT_CATALOG_ENTRY_HEADER (face);

public:
    IgSoPolyVol (void);
    static void initClass (void);

    SoMFVec3f vtxPts;  //< vector of vertex locations
    SoSFVec3f center;  //< the center of the object
    SoSFFloat dZ;      //< the z half-length

    virtual void initialise (const int sides, const float dZHalf, 
			     const float *angles, const float *sideHalf);

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_POLY_VOL_H
