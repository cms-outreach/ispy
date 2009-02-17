#ifndef IG_OPEN_INVENTOR_IG_SO_SHAPE_KIT_H
# define IG_OPEN_INVENTOR_IG_SO_SHAPE_KIT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"
# include <Inventor/nodekits/SoBaseKit.h>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoSensor;
class SoNodeSensor;
class SoAction;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_API IgSoShapeKit : public SoBaseKit
{
    SO_KIT_HEADER (IgSoShapeKit);
    SO_KIT_CATALOG_ENTRY_HEADER (separator);
 
public:
    IgSoShapeKit (void);
    static void initClass (void);

protected:
    virtual ~IgSoShapeKit (void);

    virtual void	setDefaultOnNonWritingFields (void);
    virtual SbBool	setUpConnections (SbBool onoff, SbBool doitalways = false);
    virtual void	enableSensor (void);
    virtual void	disableSensor (void);
    virtual void	refresh (void);

    // FIXME: GLRender*Path
    // FIXME: audioRender?
    virtual void	preAction (void);
    virtual void	doAction (SoAction *action);
    virtual void	callback (SoCallbackAction *action);
    virtual void	GLRender (SoGLRenderAction *action);
    virtual void	getBoundingBox (SoGetBoundingBoxAction *action);
    virtual void	getMatrix (SoGetMatrixAction *action);
    virtual void	handleEvent (SoHandleEventAction *action);
    virtual void	rayPick (SoRayPickAction *action);
    virtual void	pick (SoPickAction *action);
    virtual void	search (SoSearchAction *action);
    virtual void	write (SoWriteAction *action);
    virtual void	getPrimitiveCount (SoGetPrimitiveCountAction *action);
    virtual SbBool 	readInstance (SoInput *in, unsigned short flags);
    
private: 
    static void		selfMonitorCB (void *data, SoSensor *sensor);
    SoNodeSensor	*m_sensor;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_SHAPE_KIT_H
