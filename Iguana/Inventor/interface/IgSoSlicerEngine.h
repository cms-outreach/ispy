#ifndef IG_OPEN_INVENTOR_IG_SO_SLICER_ENGINE_H
# define IG_OPEN_INVENTOR_IG_SO_SLICER_ENGINE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include <Inventor/engines/SoEngine.h>
# include <Inventor/engines/SoSubEngine.h>
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFPlane.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

class IG_OPEN_INVENTOR_API IgSoSlicerEngine : public SoEngine
{
    SO_ENGINE_HEADER (IgSoSlicerEngine);
public:
    SoSFFloat		thickness;
    SoSFPlane		plane;
    SoEngineOutput	frontPlane;
    SoEngineOutput	backPlane;

    static void		initClass (void);

    IgSoSlicerEngine (void);
    ~IgSoSlicerEngine (void);

private:
    virtual void	evaluate (void);
};
#ifdef WIN32
#include <SoWinEnterScope.h>
#endif // WIN32

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_SLICER_ENGINE_H
