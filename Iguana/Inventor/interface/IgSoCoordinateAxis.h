#ifndef IG_OPEN_INVENTOR_IG_SO_COORDINATE_AXIS_H
# define IG_OPEN_INVENTOR_IG_SO_COORDINATE_AXIS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFInt32.h>
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

/** @class IgSoCoordinateAxis Iguana/Inventor/interface/IgSoCoordinateAxis.h

    @brief Provides an x/y/z co-ordinate axis shape with labels and
    adjustable length markers. */

class IG_OPEN_INVENTOR_API IgSoCoordinateAxis : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoCoordinateAxis);

    SO_KIT_CATALOG_ENTRY_HEADER (x);
    SO_KIT_CATALOG_ENTRY_HEADER (xrot);
    SO_KIT_CATALOG_ENTRY_HEADER (xaxis);
    SO_KIT_CATALOG_ENTRY_HEADER (y);
    SO_KIT_CATALOG_ENTRY_HEADER (yrot);
    SO_KIT_CATALOG_ENTRY_HEADER (yaxis);
    SO_KIT_CATALOG_ENTRY_HEADER (z);
    SO_KIT_CATALOG_ENTRY_HEADER (zaxis);

public:
    IgSoCoordinateAxis (void);
    static void initClass (void);

    SoSFInt32 divisions;            //< number of divisions
    SoSFFloat divisionLength;       //< length of one division in the default units

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_COORDINATE_AXIS_H
