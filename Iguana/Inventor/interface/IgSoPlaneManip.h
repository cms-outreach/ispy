#ifndef IG_OPEN_INVENTOR_IG_SO_PLANE_MANIP_H
# define IG_OPEN_INVENTOR_IG_SO_PLANE_MANIP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include <Inventor/fields/SoSFPlane.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/nodes/SoGroup.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoJackDragger;
class SoDragger;
class SoSensor;
class SoFieldSensor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

class IG_OPEN_INVENTOR_API IgSoPlaneManip : public SoGroup
{
    SO_NODE_HEADER (IgSoPlaneManip);

public:
    SoSFPlane plane;
    SoSFBool  manip;

    IgSoPlaneManip (void);

    static void 	initClass (void);
    virtual void	write (SoWriteAction *action);

    SoDragger *		getDragger(void);
    void    		getMotionMatrix (SbMatrix &matrix, 
                                         bool scaling =true);
    
protected:
    ~IgSoPlaneManip (void);

private:
    bool		hasDragger      (void);
    static void 	jackChanged     (void *me, SoDragger *dragger);
    static void		manipChanged    (void *me, SoSensor  *sensor);
    static void		planeChanged    (void *me, SoSensor  *sensor);
    
    SoJackDragger	*m_manip;
    SoFieldSensor	*m_manipSensor;
    SoFieldSensor	*m_planeSensor;
};
#ifdef WIN32
#include <SoWinEnterScope.h>
#endif // WIN32

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_PLANE_MANIP_H
