#ifndef INVENTOR_IG_SO_G4_TORUS_H
# define INVENTOR_IG_SO_G4_TORUS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/SbBox.h>
# include <Inventor/SbLinear.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_API IgSoG4Torus : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoG4Torus);
    SO_KIT_CATALOG_ENTRY_HEADER (torus);

public:
    IgSoG4Torus (void);
    static void initClass (void);

    SoSFFloat 	minRadius;	//< min radius of torus tube
    SoSFFloat 	maxRadius;   	//< max radius of torus tube
    SoSFFloat 	radius;   	//< max radius of torus tube
    SoSFFloat 	phiStart;    	//< Start angle for the rotation.
    SoSFFloat 	phiDelta;    	//< Angle of rotation.

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected:
    virtual void refresh (void);

private:
    void 	cleanParts (void);    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // INVENTOR_IG_SO_G4_TORUS_H
