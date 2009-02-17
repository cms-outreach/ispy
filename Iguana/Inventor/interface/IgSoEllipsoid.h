#ifndef IG_OPEN_INVENTOR_IG_SO_ELLIPSOID_H
# define IG_OPEN_INVENTOR_IG_SO_ELLIPSOID_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFRotation.h>
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

/*!
 * /class           IgSoEllipsoid
 * 
 * /brief           Inventor version of an ellipsoid
 * 
 * /author	    Joe Boudreau 
 * /date            Nov 11 1996
 * 
 */
class IG_OPEN_INVENTOR_API IgSoEllipsoid: public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoEllipsoid);
    SO_KIT_CATALOG_ENTRY_HEADER (trans);
    SO_KIT_CATALOG_ENTRY_HEADER (rot);
    SO_KIT_CATALOG_ENTRY_HEADER (scale);
    SO_KIT_CATALOG_ENTRY_HEADER (sphere);

public:
    IgSoEllipsoid (void);
    static void initClass (void);

    SoSFVec3f    eigenvalues;	//< Shape parameters
    SoSFRotation rotation;	//< The rotation
    SoSFVec3f    center;	//< Center of the ellipsoid

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif
