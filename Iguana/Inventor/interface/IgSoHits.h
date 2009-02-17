#ifndef IG_OPEN_INVENTOR_IG_SO_HITS_H
# define IG_OPEN_INVENTOR_IG_SO_HITS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoMFVec3f.h>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/*! \class IgSoHits IgSoHits.h IgOpenInventor/IgSoHits.h
 *  \brief Provides a minimal object to represent a series of points
 *   in space. 
 * 
 *  \author G. Alverson
 */
class IG_OPEN_INVENTOR_API IgSoHits : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoHits);
    SO_KIT_CATALOG_ENTRY_HEADER (pointset);

public:
    IgSoHits (void);
    static void initClass (void);

    SoMFVec3f	points;

    void	initXYZ (int n, const float (*pts) [3]);
    void	initXYZ (int n, const SbVec3f *pts);
    void	initRZPhi (int n, const float (*pts) [3]);
    void	initRZPhi (int n, const SbVec3f *pts);
    static void	convertRZPhiToXYZ (const float *rzphi, float *xyz);

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_HITS_H
