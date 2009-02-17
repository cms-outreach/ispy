#ifndef IG_OPEN_INVENTOR_IG_SO_XYZ_GRID_H
# define IG_OPEN_INVENTOR_IG_SO_XYZ_GRID_H

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

/*! \class IgSoXYZGrid
 *  \brief Draws a grid/grids in x, y, z planes
 *  \author L. Taylor
 */
class IG_OPEN_INVENTOR_API IgSoXYZGrid : public IgSoShapeKit
{
    // FIXME: Currently identical to IgSoCoordinateAxis.  Is this
    // class supposed to add some more functionality, like a grid at
    // the walls of the coordinate area?
    SO_KIT_HEADER (IgSoXYZGrid);
    SO_KIT_CATALOG_ENTRY_HEADER (axes);

public:
    IgSoXYZGrid (void);
    static void initClass (void);

    SoSFInt32 divisions;            //< number of divisions
    SoSFFloat divisionLength;       //< length of a division in meters

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_XYZ_GRID_H
