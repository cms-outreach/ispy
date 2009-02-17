#ifndef IG_OPEN_INVENTOR_IG_SO_ANIMATOR_H
# define IG_OPEN_INVENTOR_IG_SO_ANIMATOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include <Inventor/nodes/SoRotation.h>
# include <Inventor/nodes/SoSubNode.h>
# include <Inventor/fields/SoSFShort.h>
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFVec3f.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/fields/SoSFInt32.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoComposeRotation;
class SoTimeCounter;
class SoSensor;
class SoFieldSensor;
class SoCalculator;
class SoCounter;
class SoWriteAction;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif // WIN32

class IG_OPEN_INVENTOR_API IgSoAnimator : public SoRotation
{
    SO_NODE_HEADER (IgSoAnimator);
public:
    SoSFShort		timeMax;
    SoSFShort		timeStep;
    SoSFFloat		timeFreq;
    SoSFVec3f		axis;
    SoSFBool		on;
    SoSFBool		reset;
    SoSFInt32		state;
    SoSFInt32		newAxis;	// output signal
    SoSFBool		complete;	// output signal

    static void		initClass (void);
    virtual void	write (SoWriteAction *action);

    IgSoAnimator (void);

protected:

    ~IgSoAnimator ();
    
private:
    void		updateAxis (void);
    void 		initAnim   (void);
    static void		invokeUpdateAxis (void *data, SoSensor *sensor);
    static void		resetSensorCB    (void *me, SoSensor *data);
    static void		onSensorCB       (void *me, SoSensor *data);

    SoComposeRotation	*m_crot;
    SoCalculator        *m_calc;
    SoTimeCounter	*m_counter;
    SoCounter		*m_stateCounter;
    SoFieldSensor	*m_onSensor;
    SoFieldSensor	*m_resetSensor;
    SoFieldSensor       *m_stateSensor;
};
#ifdef WIN32
#include <SoWinEnterScope.h>
#endif // WIN32

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_ANIMATOR_H
