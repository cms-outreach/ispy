#ifndef IG_OPEN_INVENTOR_IG_SO3DERROR_BAR_H
# define IG_OPEN_INVENTOR_IG_SO3DERROR_BAR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFVec3f.h>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//  Draws a 3D point with error bars in 3 orthogonal directions.  
//  Bars are aligned arbitrarily in space (i.e. not necessarily
//  parallel to x,y,z axes.
//  Used for e.g. a space point from a tracker.
class IG_OPEN_INVENTOR_API IgSo3DErrorBar : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSo3DErrorBar);
    SO_KIT_CATALOG_ENTRY_HEADER (drawStyle);
    SO_KIT_CATALOG_ENTRY_HEADER (lineCoords);
    SO_KIT_CATALOG_ENTRY_HEADER (lineSet);
 
public:
    IgSo3DErrorBar (void);
    static void initClass (void);

    SoSFFloat lineWidth;	//< line thickness in pixels
    SoSFVec3f u1;		//< first  end of error bar in u dimension
    SoSFVec3f u2;		//< second end of error bar in u dimension
    SoSFVec3f v1;		//< first  end of error bar in v dimension
    SoSFVec3f v2;		//< second end of error bar in v dimension
    SoSFVec3f w1;		//< first  end of error bar in w dimension
    SoSFVec3f w2;		//< second end of error bar in w dimension
 
protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO3DERROR_BAR_H
