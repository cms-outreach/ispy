#ifndef IG_OPEN_INVENTOR_IG_SO_CLIPPLANE_H
# define IG_OPEN_INVENTOR_IG_SO_CLIPPLANE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include <Inventor/nodes/SoGroup.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/fields/SoSFPlane.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoClipPlane;
class SoShapeHints;
class SoFieldSensor;
class SoSensor;
class SoWriteAction;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif // WIN32

class IG_OPEN_INVENTOR_API IgSoClipPlane : public SoGroup
{
    SO_NODE_HEADER (IgSoClipPlane);
public:
    SoSFBool		on;
    SoSFBool		manip;
    SoSFPlane		plane;

    static void		initClass (void);
    virtual void	write (SoWriteAction *action);

    IgSoClipPlane (void);

protected:
    ~IgSoClipPlane (void);

private:
    void		reconnect      (void);
    void		syncPlane      (void);
    static void		onSensorCB     (void *me, SoSensor *data);
    static void		manipSensorCB  (void *me, SoSensor *data);
    static void		planeSensorCB  (void *me, SoSensor *data);
    static void		mplaneSensorCB (void *me, SoSensor *data);

    SoClipPlane		*m_plane;
    SoShapeHints	*m_shapeHints;
    SoFieldSensor	*m_onSensor;
    SoFieldSensor	*m_manipSensor;
    SoFieldSensor	*m_planeSensor;
    SoFieldSensor	*m_mplaneSensor;
};
#ifdef WIN32
#include <SoWinEnterScope.h>
#endif // WIN32

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_CLIPPLANE_H
