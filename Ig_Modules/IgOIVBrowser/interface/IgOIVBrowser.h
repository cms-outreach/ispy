#ifndef IG_SOCKET_DRIVER_IG_OIV_BROWSER_H
# define IG_SOCKET_DRIVER_IG_OIV_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgOIVBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgBrowser.h"
# include <qcstring.h>
# include <Inventor/SbViewportRegion.h>
# include <Inventor/SbLinear.h>
# include <Inventor/SbTime.h>
# include <Inventor/SoType.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgSite;
class IgState;
class Ig3DBaseModel;
class SoCamera;
class SoGroup;
class SoOffscreenRenderer;
class SbSphereSheetProjector;
class SoPerspectiveCamera;
class SoOrthographicCamera;


//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OIV_BROWSER_API IgOIVBrowser : public IgBrowser
{
public:
    static const char *		catalogLabel (void);    
    static const int MOUSEPOSLOGSIZE = 16;

    IgOIVBrowser (IgState *state,
		  IgSite *site,
		  Ig3DBaseModel *model = 0);
    ~IgOIVBrowser (void);
    
    virtual void		viewAll (void);
    virtual SoCamera *		getCamera (void);
    virtual void		setCamera (void);
    QByteArray			getBuffer (void);
    QByteArray			getJPGBuffer (void);
    QByteArray			getIV (void);
    virtual void		browse (IgRepresentable *rep);

    virtual void 		moveCameraScreen (const SbVec2f &screenpos);    
    virtual void 		spin (const SbVec2f & pointerpos);
    virtual void		zoom (const float diffValue);    
    void 			clearLog (void);
    void 			addToLog (const SbVec2f pos, const SbTime time);
    void 			reorientCamera (const SbRotation &r);
    const SbViewportRegion &	viewRegion (void);
    const SbViewportRegion &	renderRegion (void);
    void			setCameraType (SoType t);    
    void			toggleCameraType (void);
    void 			saveHomePosition (void);
    void 			replaceCamera (SoCamera *cam);
    

    static void 		convertOrtho2Perspective (const SoOrthographicCamera * in,
							  SoPerspectiveCamera *out);
    static void 		convertPerspective2Ortho (const SoPerspectiveCamera * in,
							  SoOrthographicCamera * out);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    
    SbVec2f 				m_lastMousePosition;
    SbPlane 				m_panningPlane;
    SbBool 				m_spinAnimatingAllowed;
    SbVec2f 				m_lastSpinPosition;
    int 				m_spinSampleCounter;
    SbRotation 				m_spinIncrement;
    SbSphereSheetProjector * 		m_spinProjector;

    SbRotation				m_spinRotation;

    struct { // tracking mouse movement in a log
	short size;
	short historysize;
	SbVec2f * position;
	SbTime * time;
    } 					m_log;

    // The Microsoft Visual C++ v6.0 compiler needs a name on this class
    // to be able to generate a constructor (which it wants to have for
    // running the the SbVec2s constructors). So don't try to be clever
    // and make it anonymous.
    struct Pointer {
	SbVec2s now, then;
    } 					m_pointer;

    SbBool 				m_button1down;
    SbBool 				m_button3down;
    SbBool 				m_ctrldown, m_shiftdown;
    SbTime			 	m_prevRedrawTime;
protected:
    static IgState *	       	initialise (IgState *state);    
private:
    IgSite *			m_site;
    IgState *			m_state;
    Ig3DBaseModel *		m_model;
    SoGroup *			m_topLevelGroup;
    SoCamera *			m_camera;
    SoType 			m_cameraType;    
    SbViewportRegion		m_renderRegion;
    SbViewportRegion		m_viewRegion;
    // FIXME: offscreen renderer should be a service so that different
    // browsers do not create multiple opengl contextes.
    SoOffscreenRenderer * 	m_renderer;  
    bool			m_deleteCamera;    
    SoCamera *			m_storedCamera;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_SOCKET_DRIVER_IG_OIV_BROWSER_H
