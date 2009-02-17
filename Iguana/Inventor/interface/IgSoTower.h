#ifndef IG_OPEN_INVENTOR_IG_SO_TOWER_H
# define IG_OPEN_INVENTOR_IG_SO_TOWER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include "Inventor/fields/SoSFFloat.h"
# include "Inventor/fields/SoSFVec2f.h"
# include "Inventor/fields/SoMFUInt32.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_API IgSoTower : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoTower);
    SO_KIT_CATALOG_ENTRY_HEADER (shapeHints);
    SO_KIT_CATALOG_ENTRY_HEADER (faces);

public:
    IgSoTower (void);
    static void initClass (void);

    SoSFVec2f 	position;     	//< tower center in eta, phi
    SoSFFloat	energy;		//< total energy deposited
    SoSFFloat	emFraction;	//< EM/Total
    SoSFFloat	scaleFactor;	//< GeV to unit distance
    SoSFFloat	minimumEnergy;	//< energy threshold to draw hit
    SoSFFloat	etaWidth;	//< size of hit in eta
    SoSFFloat	phiWidth;	//< size of hit in phi
    SoMFUInt32	orderedRGBA;	//< materials

protected:
    virtual void refresh (void);
private:
    void cleanParts (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_TOWER_H
