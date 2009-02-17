#ifndef IG_OPEN_INVENTOR_IG_SO_QUAD_H
# define IG_OPEN_INVENTOR_IG_SO_QUAD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFVec2f.h>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/*! \class IgSoQuad
 *  \brief Quadrilateral in the (x,y) plane defined by the 2D vectors pointing
 * to the corners
 *
 *  \author Laurent Duflot   
 *  \date April 2002
 */
class  IG_OPEN_INVENTOR_API IgSoQuad : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoQuad);
    SO_KIT_CATALOG_ENTRY_HEADER (face);

public:
    IgSoQuad (void);
    static void initClass (void);

    SoSFVec2f llCornerOffset; //< lower left corner
    SoSFVec2f lrCornerOffset; //< lower right corner
    SoSFVec2f urCornerOffset; //< upper right corner
    SoSFVec2f ulCornerOffset; //< upper left corner

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_QUAD_H
