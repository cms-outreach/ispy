#ifndef IG_OPEN_INVENTOR_IG_SO_CAL_HIT_H
# define IG_OPEN_INVENTOR_IG_SO_CAL_HIT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include "Inventor/fields/SoSFBool.h"
# include "Inventor/fields/SoSFFloat.h"
# include "Inventor/fields/SoMFVec3f.h"
# include "Inventor/fields/SoMFUInt32.h"
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/*! \class IgSoCalHit 
 *  \brief Represents an energy deposition by an extruded shape proportional to the energy.
 *  This is a cheap copy of IgSoCrystalHits. All parts are currently 
 *  drawn despite the settings of the flags.
 *
 *  At present, all the flags must be initialised first,and then
 *  the vertices must be supplied.
 *
 * The different colors for EM/Had are accessible as follows:
 *
    std::vector<SbVec3f> pts(8);
    pts	[0] = SbVec3f (0, 0, 0); pts [4] = SbVec3f (0, 0, 1);
    pts	[1] = SbVec3f (1, 0, 0); pts [5] = SbVec3f (1, 0, 1);
    pts	[2] = SbVec3f (1, 1, 0); pts [6] = SbVec3f (1, 1, 1);
    pts	[3] = SbVec3f (0, 1, 0); pts [7] = SbVec3f (0, 1, 1);

    IgSoCalHit *obj = new IgSoCalHit;
    obj->energy = 10;
    obj->vertices.setValues (0, 8, &pts[0]);
    obj->orderedRGBA.set1Value (0, SbColor (0.5,0.5,0.5).getPackedValue ());
    obj->orderedRGBA.set1Value (1, SbColor (0.5,1.0,0.5).getPackedValue ());
    obj->orderedRGBA.set1Value (2, SbColor (0.5,0.5,1.0).getPackedValue ());
    obj->minimumEnergy = 0.2;
    obj->drawShape = false;
 *
 *  \author G. Alverson, Northeastern University/CMS Software
 *  \date 20 June 2002
 *
 */
class IG_OPEN_INVENTOR_API IgSoCalHit : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoCalHit);
    SO_KIT_CATALOG_ENTRY_HEADER (faces);

public:
    IgSoCalHit (void);
    static void initClass (void);

    SoMFVec3f	vertices;	//< vertices for the base shape, in order (must be even no.)
    SoSFFloat	energy;		//< total energy deposited
    SoSFFloat	emFraction;	//< EM/Total
    SoSFFloat	scaleFactor;	//< GeV to unit distance
    SoSFFloat	minimumEnergy;	//< energy threshold to draw hit
    SoSFFloat	relativeWidth;	//< relative size of hit wrt shape
    SoSFBool	drawShape;
    SoSFBool	drawHit;
    SoSFBool	twoColor;	//< display EM/had in different materials
    SoMFUInt32	orderedRGBA;	//< materials

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_CAL_HIT_H
