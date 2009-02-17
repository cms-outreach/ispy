#ifndef IG_OPEN_INVENTOR_IG_SO_CIRCLE_ARC_H
# define IG_OPEN_INVENTOR_IG_SO_CIRCLE_ARC_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
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

/*
 * Circle arc in the (x,y) plane centered at center with radius radius, 
 * that spans an angle sweepAngle in degrees starting along the x axis
 *
 * Laurent Duflot   April 2002
 */
class IG_OPEN_INVENTOR_API IgSoCircleArc : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoCircleArc);
    SO_KIT_CATALOG_ENTRY_HEADER (line);

public:
    IgSoCircleArc (void);
    static void initClass (void);

    SoSFVec2f center;     //< disk center
    SoSFFloat radius;     //< circle radius
    SoSFFloat sweepAngle; //< angle that the disk spans, starting along the x axis

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_CIRCLE_ARC_H
