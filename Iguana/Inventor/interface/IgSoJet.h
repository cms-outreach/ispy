#ifndef IG_OPEN_INVENTOR_IG_SO_JET_H
# define IG_OPEN_INVENTOR_IG_SO_JET_H

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

/** Draw a Jet object using energy, theta, phi, thrust.  */
class IG_OPEN_INVENTOR_API IgSoJet : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoJet);
    SO_KIT_CATALOG_ENTRY_HEADER (body);
    SO_KIT_CATALOG_ENTRY_HEADER (bodyTrans);
    SO_KIT_CATALOG_ENTRY_HEADER (bodyCone);
    SO_KIT_CATALOG_ENTRY_HEADER (hat);
    SO_KIT_CATALOG_ENTRY_HEADER (hatTrans);
    SO_KIT_CATALOG_ENTRY_HEADER (hatCone);

public:
    IgSoJet (void);
    static void initClass (void);

    SoSFFloat energy;       //< Energy of the jet
    SoSFFloat theta;        //< Theta of the jet axis
    SoSFFloat phi;          //< Phi of the jet axis
    SoSFFloat thrust;       //< Thrust of the jet
    SoSFFloat maxZ;         //< Half length of cylinder outside of which jet does not extend
    SoSFFloat maxR;         //< Radius      of cylinder outside of which jet does not extend
    SoSFFloat maxEnergy;    //< Maximum energy used to scale jet size

protected:
    virtual void refresh (void);

    // dimensions and angles for various things in the IgSoJet object
    void setParameters ();
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_JET_H
