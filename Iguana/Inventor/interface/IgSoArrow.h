#ifndef IG_OPEN_INVENTOR_IG_SO_ARROW_H
# define IG_OPEN_INVENTOR_IG_SO_ARROW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
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

/** An arrow has a cylindar and also a conical tip.  It is suitable for
    representing vectors and the like.  */
class IG_OPEN_INVENTOR_API IgSoArrow : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoArrow);
    SO_KIT_CATALOG_ENTRY_HEADER (finalTranslation);
    SO_KIT_CATALOG_ENTRY_HEADER (scale);
    SO_KIT_CATALOG_ENTRY_HEADER (myRotation);
    SO_KIT_CATALOG_ENTRY_HEADER (rot);
    SO_KIT_CATALOG_ENTRY_HEADER (zTranslation);
    SO_KIT_CATALOG_ENTRY_HEADER (axisCyl);
    SO_KIT_CATALOG_ENTRY_HEADER (cTranslation);
    SO_KIT_CATALOG_ENTRY_HEADER (axisCone);
 
public:
    IgSoArrow (void);
    static void initClass (void);

    SoSFVec3f tip;		//< tail world coordinates
    SoSFVec3f tail;		//< tip world coordinates
 
protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_ARROW_H
