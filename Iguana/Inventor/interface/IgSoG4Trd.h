#ifndef IG_OPEN_INVENTOR_IG_SO_G4_TRD_H
# define IG_OPEN_INVENTOR_IG_SO_G4_TRD_H

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

/*!	\class IgSoG4Trd IgSoG4Trd.h
 *
 * \brief The documentation from Geant says:
 * A Trd is a trapezoid with the x and y dimensions varying along z
 * functions:
 *
 * \author Joe Boudreau
 * \date 11 Nov 1996
 */
class IG_OPEN_INVENTOR_API IgSoG4Trd : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoG4Trd);
    SO_KIT_CATALOG_ENTRY_HEADER (faces);

public:
    IgSoG4Trd (void);
    static void initClass (void);

    SoSFFloat dx1;      
    SoSFFloat dx2;
    SoSFFloat dy1;
    SoSFFloat dy2;
    SoSFFloat dz;

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_G4_TRD_H
