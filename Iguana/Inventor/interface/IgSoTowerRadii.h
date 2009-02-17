#ifndef IG_OPEN_INVENTOR_IG_SO_TOWER_RADII_H
# define IG_OPEN_INVENTOR_IG_SO_TOWER_RADII_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFVec2f.h>
# include <Inventor/fields/SoSFBool.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_API IgSoTowerRadii : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoTowerRadii);
    SO_KIT_CATALOG_ENTRY_HEADER (line);
    SO_KIT_CATALOG_ENTRY_HEADER (marker);

public:
    IgSoTowerRadii (void);
    static void initClass (void);

    SoSFVec2f center;     //< disk center
    SoSFFloat radius;     //< circle radius
    SoSFBool  displayCenter; //< put a marker in the center
    
protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_TOWER_RADII_H
