#ifndef IG_OPEN_INVENTOR_IG_SO_SLICER_H
# define IG_OPEN_INVENTOR_IG_SO_SLICER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include <Inventor/nodes/SoGroup.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFPlane.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgSoClipPlane;
class SoFieldSensor;
class SoSensor;
class IgSoSlicerEngine;
class IgSoPlaneManip;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

class IG_OPEN_INVENTOR_API IgSoSlicer : public SoGroup
{
    SO_NODE_HEADER (IgSoSlicer);
public:
    SoSFBool		on;
    SoSFBool		manip;
    SoSFFloat		thickness;
    SoSFPlane		plane;

    static void		initClass (void);
    virtual void	write (SoWriteAction *action);

    IgSoSlicer (void);

protected:
    ~IgSoSlicer (void);

private:
    static void		planeSensorCB (void *me, SoSensor *data);
    static void		mplaneSensorCB (void *me, SoSensor *data);

    IgSoClipPlane	*m_front;
    IgSoClipPlane	*m_back;
    IgSoPlaneManip	*m_manipPlane;
    IgSoSlicerEngine	*m_engine;
    SoFieldSensor	*m_planeSensor;
    SoFieldSensor	*m_mplaneSensor;
};
#ifdef WIN32
#include <SoWinEnterScope.h>
#endif // WIN32

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_SLICER_H
