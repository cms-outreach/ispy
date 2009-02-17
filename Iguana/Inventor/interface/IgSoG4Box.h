#ifndef IG_OPEN_INVENTOR_IG_SO_G4_BOX_H
# define IG_OPEN_INVENTOR_IG_SO_G4_BOX_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Represents the G4Box Geant Geometry entity.

    A Box is a cuboid of given half lengths dx,dy,dz. The box is
    centred on the origin with sides parallel to the x/y/z axes.  */
class IG_OPEN_INVENTOR_API IgSoG4Box : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoG4Box);
    SO_KIT_CATALOG_ENTRY_HEADER (scale);
    SO_KIT_CATALOG_ENTRY_HEADER (cube);

public:
    IgSoG4Box (void);
    static void initClass (void);

    SoSFFloat dx;   
    SoSFFloat dy;
    SoSFFloat dz;

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_G4_BOX_H
