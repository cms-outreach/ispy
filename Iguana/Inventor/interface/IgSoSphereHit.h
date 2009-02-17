#ifndef IG_OPEN_INVENTOR_IG_SO_SPHERE_HIT_H
# define IG_OPEN_INVENTOR_IG_SO_SPHERE_HIT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFVec3f.h>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Draws SoSphere at arbitrary location with arbitrary scale. */
class IG_OPEN_INVENTOR_API IgSoSphereHit : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoSphereHit);
    SO_KIT_CATALOG_ENTRY_HEADER (transform);
    SO_KIT_CATALOG_ENTRY_HEADER (sphere);

public:
    IgSoSphereHit (void);
    static void initClass (void);

    SoSFFloat	scale;	//< Energy scale factor for size of sphere
    SoSFFloat	energy;	//< Energy of hit
    SoSFVec3f	centre;	//< Centre of sphere

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_SPHERE_HIT_H
