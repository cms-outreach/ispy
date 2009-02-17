#ifndef IG_OPEN_INVENTOR_IG_SO_CUBE_H
# define IG_OPEN_INVENTOR_IG_SO_CUBE_H

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

/*! 
 *  \class IgSoCube IgSoCube.h
 *
 *  \brief Creates an OpenInventor representation for an outlined cube
 *
 *  \author Ianna Osborne, CMS/Northeastern Software Group
 *  \date  19 Mar 2002
 *
 */
class IG_OPEN_INVENTOR_API IgSoCube : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoCube);
    SO_KIT_CATALOG_ENTRY_HEADER (hints);
    SO_KIT_CATALOG_ENTRY_HEADER (faces);
    SO_KIT_CATALOG_ENTRY_HEADER (lines);

public:
    IgSoCube (void);
    static void initClass (void);

    SoSFFloat width;			//< full width
    SoSFFloat length;			//< full length
    SoSFFloat thickness;		//< full thickness

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_CUBE_H
