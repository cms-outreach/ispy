/*Lot's of the code is cut and paste of SoQt code. KUDOS to SoQt developers.*/

//<<<<<< INCLUDES                                                       >>>>>>
#include "Ig_Modules/IgOIVBrowser/interface/IgOIVBrowser.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseModel.h"
#include "Ig_Modules/Ig3DModel/interface/Ig3DModel.h"
#include "Ig_Modules/Ig2DModel/interface/Ig2DModel.h"
#include "Ig_Modules/IgLegoModel/interface/IgLegoModel.h"
#include "Ig_Modules/IgRPhiModel/interface/IgRPhiModel.h"
#include "Ig_Modules/IgRZModel/interface/IgRZModel.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowser.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepresentable.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRep.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgArgsElement.h"
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>
#include <Inventor/SoDB.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <qfile.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/UUID.h>
#include <unistd.h>
#include <Inventor/SbLinear.h>
#include <iostream>
#include <math.h>
#include <float.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

const char *
IgOIVBrowser::catalogLabel (void)
{ return "OIV"; }


SoGroup *
getParentOfNode (SoNode * root, SoNode * node)
{
    SbBool oldsearch = SoBaseKit::isSearchingChildren();
    SoBaseKit::setSearchingChildren(TRUE);

    ASSERT (node && root);
    SoSearchAction search;
    search.setSearchingAll (TRUE);
    search.setNode (node);
    search.apply (root);
    ASSERT (search.getPath ());
    SoNode * parent = ((SoFullPath *)search.getPath())->getNodeFromTail(1);
    ASSERT (parent);
    SoBaseKit::setSearchingChildren (oldsearch);
    return static_cast <SoGroup *> (parent);
} 



void initShapes (void);

// FIXME: initialisation of the 3D system should happen when the model
// is first created, not the browser!!!! this is true for any other
// browser that we have!
IgState *
IgOIVBrowser::initialise (IgState *state)
{
    static bool initialised = false;
    
    if (! initialised)
    {
	std::string display(":0.0");
	IgArgsElement::get (state)->find ("--ig-display", display);
	setenv ("DISPLAY", display.c_str(), 1);		
	SoDB::init (); 
	SoNodeKit::init ();
	SoInteraction::init ();
	initShapes (); 
    }
    initialised = true;    
    
    return state;    
}

IgOIVBrowser::IgOIVBrowser (IgState *state,
			    IgSite *site,
			    Ig3DBaseModel *model)
    : m_site (site),
      m_state (initialise (state)),
      m_model (model),
      m_topLevelGroup (0),
      m_camera (0),
      m_renderRegion (512, 512),
      m_viewRegion (512, 512),
      m_renderer (new SoOffscreenRenderer (m_renderRegion)),
      m_deleteCamera (false),
      m_storedCamera (0)
{
    // FIXME: ORRENDOUS HACK this needs to be fixed so that 3d system
    // initialisation is done by **models**, not by browsers.
    if (! m_model)
    {
	m_model = new Ig3DModel (state);	
    }
    else if (m_model == (Ig2DModel *) -1)
    {
	m_model = new Ig2DModel (state, new Ig3DModel (state));	
    }
    else if (m_model == (IgRPhiModel *) -2)
    {
	m_model = new IgRPhiModel (state);	
    }
    else if (m_model == (IgRZModel *) -3)
    {
	m_model = new IgRZModel (state);	
    }
    else if (m_model == (IgLegoModel *) -4)
    {
	m_model = new IgLegoModel (state);	
    }
    

    m_prevRedrawTime = SbTime::getTimeOfDay();
    m_spinAnimatingAllowed = TRUE;
    m_spinSampleCounter = 0;
    m_spinIncrement = SbRotation::identity();

    // FIXME: use a smaller sphere than the default one to have a larger
    // area close to the borders that gives us "z-axis rotation"?
    // 19990425 mortene.
    m_spinProjector = new SbSphereSheetProjector (SbSphere(SbVec3f(0, 0, 0), 0.8f));
    SbViewVolume volume;
    volume.ortho(-1, 1, -1, 1, -1, 1);
    m_spinProjector->setViewVolume (volume);

    m_log.size = MOUSEPOSLOGSIZE;
    m_log.position = new SbVec2f [ MOUSEPOSLOGSIZE ];
    m_log.time = new SbTime [ MOUSEPOSLOGSIZE ];
    m_log.historysize = 0;
    m_pointer.now = SbVec2s(0, 0);
    m_pointer.then = SbVec2s(0, 0);
}

IgOIVBrowser::~IgOIVBrowser (void)
{
    delete m_renderer;
}

SoCamera *
IgOIVBrowser::getCamera (void)
{
    if (! m_camera)
    {
	setCamera ();
    }
    
    return m_camera;    
}

void
IgOIVBrowser::convertOrtho2Perspective(const SoOrthographicCamera * in,
				       SoPerspectiveCamera * out)
{
    out->aspectRatio.setValue(in->aspectRatio.getValue());
    out->focalDistance.setValue(in->focalDistance.getValue());
    out->orientation.setValue(in->orientation.getValue());
    out->position.setValue(in->position.getValue());
    out->viewportMapping.setValue(in->viewportMapping.getValue());
  
    float focaldist = in->focalDistance.getValue();

    // focalDistance==0.0f happens for empty scenes.
    if (focaldist != 0.0f) {
	out->heightAngle = 2.0f * (float)atan(in->height.getValue() / 2.0 / focaldist);
    }
    else {
	// 45… is the default value of this field in SoPerspectiveCamera.
	out->heightAngle = (float)(M_PI / 4.0);
    }
}


void
IgOIVBrowser::convertPerspective2Ortho(const SoPerspectiveCamera * in,
				       SoOrthographicCamera * out)
{
    out->aspectRatio.setValue (in->aspectRatio.getValue());
    out->focalDistance.setValue (in->focalDistance.getValue());
    out->orientation.setValue (in->orientation.getValue());
    out->position.setValue (in->position.getValue());
    out->viewportMapping.setValue (in->viewportMapping.getValue());

    float focaldist = in->focalDistance.getValue ();

    out->height = 2.0f * focaldist * (float) tan(in->heightAngle.getValue() / 2.0);
}

void
IgOIVBrowser::replaceCamera (SoCamera *cam)
{
    if (this->m_camera) 
    {
	// If we made the camera, detach it. Otherwise just leave it in
	// the graph.
	if (this->m_deleteCamera) 
	{
	    SoGroup * cameraParent =
		getParentOfNode (m_topLevelGroup, this->m_camera);
	    cameraParent->removeChild (this->m_camera);
	    this->m_deleteCamera = false;
	}

	this->m_camera->unref();
    }

    this->m_camera = cam;

    if (this->m_camera) 
    {
	this->m_camera->ref();
	this->saveHomePosition();
	this->m_cameraType = this->m_camera->getTypeId ();
    }
}

void
IgOIVBrowser::saveHomePosition (void)
{
    if (! this->m_camera) 
	return; // probably a scene-less viewer

  // We use SoType::createInstance() to store a copy of the camera,
  // not just assuming it's either a perspective or an orthographic
  // camera.

  SoType t = this->m_camera->getTypeId();
  ASSERT (t.isDerivedFrom(SoNode::getClassTypeId()));
  ASSERT (t.canCreateInstance());

  if (this->m_storedCamera) 
  { 
      this->m_storedCamera->unref (); 
  }

  this->m_storedCamera 
      = dynamic_cast <SoCamera *> (static_cast<SoNode *>(t.createInstance ()));
  this->m_storedCamera->ref ();
  this->m_storedCamera->copyContents(this->m_camera, false);
}

void
IgOIVBrowser::setCamera (void)
{
    ASSERT (m_model->sceneGraph ());	

    SoSearchAction findCameraAction;
    findCameraAction.setType (SoCamera::getClassTypeId ());
    findCameraAction.apply (m_model->sceneGraph ());
    
    m_topLevelGroup = new SoGroup ();
    m_topLevelGroup->ref ();
    
    if (findCameraAction.isFound ())
    {
	m_camera 
	    = dynamic_cast <SoCamera *> (findCameraAction.getPath ()->getTail ());
	ASSERT (m_camera);	
    } 
    else
    {
	m_camera = new SoPerspectiveCamera ();
    }
    m_topLevelGroup->addChild (new SoPointLight ());    
    m_topLevelGroup->addChild (m_camera);    
    m_topLevelGroup->addChild (m_model->sceneGraph ());    
}


void
IgOIVBrowser::viewAll (void)
{
    m_camera->viewAll (m_topLevelGroup, m_renderRegion);
}

void
IgOIVBrowser::browse (IgRepresentable *object)
{
    // FIXME: can we just depend on model change event?
    IgRepSet::lookup (object, m_model, true);
}

QByteArray
IgOIVBrowser::getBuffer (void)
{
    //FIXME: for the time being just redirect it to JPG.
    return IgOIVBrowser::getJPGBuffer ();    
}

std::string
randomFile (void)
{
    lat::UUID uuid = lat::UUID::generateRandom ();
    char *uuidBuffer = new char [37];
    uuid.format (uuidBuffer);
    std::string filename (uuidBuffer);
    delete[] uuidBuffer;
    return filename;    
}


QByteArray
IgOIVBrowser::getJPGBuffer (void)
{
    if (! m_topLevelGroup)
    {
	setCamera ();	
    }
    m_renderer->render (m_topLevelGroup);
    
    std::string filename = randomFile ();
    
    m_renderer->writeToFile ((filename + ".jpg").c_str (), "jpg");
    QFile f ((filename + ".jpg").c_str ());
    ASSERT (f.exists ());
		
    bool opened = f.open (IO_ReadOnly);
    ASSERT (opened);
		
    QByteArray tmpArray = f.readAll ();
    ASSERT (! tmpArray.isEmpty ());
    f.remove ();    
    return tmpArray;    
}

QByteArray
IgOIVBrowser::getIV (void)
{
    SoOutput output;
    std::string filename = randomFile ();

    output.openFile((filename+".iv").c_str ());

    SoWriteAction *writeAction = new SoWriteAction(&output);

    writeAction->apply (m_topLevelGroup);

    output.closeFile();
    //FIXME: we should be writing to some buffer directly!!!

    QFile f ((filename + ".iv").c_str ());
    ASSERT (f.exists ());
		
    bool opened = f.open (IO_ReadOnly);
    ASSERT (opened);
		
    QByteArray tmpArray = f.readAll ();
    ASSERT (! tmpArray.isEmpty ());
    f.remove ();    
    return tmpArray;    
}


void
IgOIVBrowser::moveCameraScreen (const SbVec2f & screenpos)
{
    SoCamera * cam = getCamera ();

    ASSERT (cam);


    SbViewVolume vv = cam->getViewVolume (cam->aspectRatio.getValue ());
    SbPlane panplane = vv.getPlane (cam->focalDistance.getValue());

    SbLine line;
    vv.projectPointToLine(screenpos + SbVec2f (0.5, 0.5f), line);
    SbVec3f current_planept;
    panplane.intersect (line, current_planept);
    vv.projectPointToLine (SbVec2f(0.5f, 0.5f), line);
    SbVec3f old_planept;
    panplane.intersect (line, old_planept);
    
    // Reposition camera according to the vector difference between the
    // projected points.
    cam->position = cam->position.getValue () - (current_planept - old_planept);
}

void
IgOIVBrowser::spin (const SbVec2f & pointerpos)
{
    if (m_log.historysize < 2) 
	return;
    
    ASSERT (m_spinProjector != NULL);

    SbVec2f lastpos;
    lastpos[0] = float (m_log.position[1][0]);
    lastpos[1] = float (m_log.position[1][1]);

    m_spinProjector->project (lastpos);
    SbRotation r;
    m_spinProjector->projectAndGetRotation (pointerpos, r);
    r.invert();
    reorientCamera (r);

    // Calculate an average angle magnitude value to make the transition
    // to a possible spin animation mode appear smooth.

    SbVec3f dummy_axis, newaxis;
    float acc_angle, newangle;
    m_spinIncrement.getValue (dummy_axis, acc_angle);
    acc_angle *= m_spinSampleCounter; // weight
    r.getValue (newaxis, newangle);
    acc_angle += newangle;

    m_spinSampleCounter++;
    acc_angle /= m_spinSampleCounter;
    // FIXME: accumulate and average axis vectors aswell? 19990501 mortene.
    m_spinIncrement.setValue (newaxis, acc_angle);

    // Don't carry too much baggage, as that'll give unwanted results
    // when the user quickly trigger (as in "click-drag-release") a spin
    // animation.
    if (m_spinSampleCounter > 3) 
	m_spinSampleCounter = 3;
}

void
IgOIVBrowser::reorientCamera (const SbRotation &rot)
{
    SoCamera * cam = getCamera ();
    if (cam == NULL) 
	return;

    // Find global coordinates of focal point.
    SbVec3f direction;
    cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
    SbVec3f focalpoint = cam->position.getValue() +
			 cam->focalDistance.getValue() * direction;

    // Set new orientation value by accumulating the new rotation.
    cam->orientation = rot * cam->orientation.getValue();

    // Reposition camera so we are still pointing at the same old focal point.
    cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
    cam->position = focalpoint - cam->focalDistance.getValue() * direction;
}

void 
IgOIVBrowser::addToLog (const SbVec2f pos, const SbTime time)
{
    // In case someone changes the const size setting at the top of this
    // file too small.
    ASSERT (m_log.size > 2 && "mouse log too small!");

    if (m_log.historysize > 0 && pos == m_log.position[0]) 
    {
	return;
    }

    int lastidx = m_log.historysize;
    // If we've filled up the log, we should throw away the last item:
    if (lastidx == m_log.size) { lastidx--; }

    ASSERT (lastidx < m_log.size);
    for (int i = lastidx; i > 0; i--) {
	m_log.position[i] = m_log.position[i-1];
	m_log.time[i] = m_log.time[i-1];
    }

    m_log.position[0] = pos;
    m_log.time[0] = time;
    if (m_log.historysize < m_log.size)
	m_log.historysize += 1;
}

void 
IgOIVBrowser::clearLog (void)
{
    m_log.historysize = 0;
}

const SbViewportRegion &
IgOIVBrowser::renderRegion (void)
{
    return m_renderRegion;    
}

const SbViewportRegion &
IgOIVBrowser::viewRegion (void)
{
    return m_viewRegion;    
}


void
IgOIVBrowser::zoom (const float diffvalue)
{
    SoCamera * cam = getCamera ();
    if (cam == NULL) return; // can happen for empty scenegraph
    SoType t = cam->getTypeId();
    SbName tname = t.getName();

    // This will be in the range of <0, ->>.
    float multiplicator = float(exp(diffvalue));

    if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId())) {

	// Since there's no perspective, "zooming" in the original sense
	// of the word won't have any visible effect. So we just increase
	// or decrease the field-of-view values of the camera instead, to
	// "shrink" the projection size of the model / scene.
	SoOrthographicCamera * oc = (SoOrthographicCamera *)cam;
	oc->height = oc->height.getValue() * multiplicator;

    }
    else {
	// FrustumCamera can be found in the SmallChange CVS module (it's
	// a camera that lets you specify (for instance) an off-center
	// frustum (similar to glFrustum())
	if (!t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()) &&
	    tname != "FrustumCamera") {
	    static SbBool first = TRUE;
	    if (first) {
		SoDebugError::postWarning("SoGuiFullViewerP::zoom",
					  "Unknown camera type, "
					  "will zoom by moving position, but this might not be correct.");
		first = FALSE;
	    }
	}
    
	const float oldfocaldist = cam->focalDistance.getValue();
	const float newfocaldist = oldfocaldist * multiplicator;

	SbVec3f direction;
	cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);

	const SbVec3f oldpos = cam->position.getValue();
	const SbVec3f newpos = oldpos + (newfocaldist - oldfocaldist) * -direction;

	// This catches a rather common user interface "buglet": if the
	// user zooms the camera out to a distance from origo larger than
	// what we still can safely do floating point calculations on
	// (i.e. without getting NaN or Inf values), the faulty floating
	// point values will propagate until we start to get debug error
	// messages and eventually an assert failure from core Coin code.
	//
	// With the below bounds check, this problem is avoided.
	//
	// (But note that we depend on the input argument ''diffvalue'' to
	// be small enough that zooming happens gradually. Ideally, we
	// should also check distorigo with isinf() and isnan() (or
	// inversely; isinfite()), but those only became standardized with
	// C99.)
	const float distorigo = newpos.length();
	// sqrt(FLT_MAX) == ~ 1e+19, which should be both safe for further
	// calculations and ok for the end-user and app-programmer.
	if (distorigo > float(sqrt(FLT_MAX))) 
	{
	}
	else {
	    cam->position = newpos;
	    cam->focalDistance = newfocaldist;
	}
    }
}

//// Workaround for initialising IGUANA OIV shapes.

#include "Ig_Extensions/IgOpenInventor/interface/IgParticleChar.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoShapeKit.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSo2DArrow.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSo3DErrorBar.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoArrow.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoAxis.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCalHit.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCircleArc.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCoordinateAxis.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCrystalHit.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCube.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoEllipsoid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoFieldPlane.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoFieldPlaneMap.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoG4Box.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoG4Trap.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoG4Trd.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoGrid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoHits.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoIdealTrack.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoJet.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoLegoPlot.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoLegoTowers.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoPcon.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoPolyVol.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoQuad.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoRotSolid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoRZHist.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSiStrips.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSimpleTrajectory.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSphereHit.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSplineTrack.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoTower.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoTowerRadii.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoXYZGrid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoAnimator.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoClipPlane.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSlicer.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSlicerEngine.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoViewpoint.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoPlaneManip.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCircularHist.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoGL2PSAction.h"

void initShapes (void)
{
    IgParticleChar::initParticles ();
    IgSoShapeKit::initClass ();
    IgSo2DArrow::initClass ();
    IgSo3DErrorBar::initClass ();
    IgSoArrow::initClass ();
    IgSoAxis::initClass ();
    IgSoCalHit::initClass ();
    IgSoCircleArc::initClass ();
    IgSoCoordinateAxis::initClass ();
    IgSoCrystalHit::initClass ();
    IgSoCube::initClass ();
    IgSoEllipsoid::initClass ();
    IgSoFieldPlane::initClass ();
    IgSoFieldPlaneMap::initClass ();
    IgSoG4Box::initClass ();
    IgSoG4Trap::initClass ();
    IgSoG4Trd::initClass ();
    IgSoGrid::initClass ();
    IgSoHits::initClass ();
    IgSoIdealTrack::initClass ();
    IgSoJet::initClass ();
    IgSoLegoPlot::initClass ();
    IgSoLegoTowers::initClass ();
    IgSoPcon::initClass ();
    IgSoPolyVol::initClass ();
    IgSoQuad::initClass ();
    IgSoRotSolid::initClass ();
    IgSoRZHist::initClass ();
    IgSoSiStrips::initClass ();
    IgSoSimpleTrajectory::initClass ();
    IgSoSphereHit::initClass ();
    IgSoSplineTrack::initClass ();
    IgSoTower::initClass ();
    IgSoTowerRadii::initClass ();
    IgSoXYZGrid::initClass ();
    IgSoAnimator::initClass ();
    IgSoClipPlane::initClass ();
    IgSoSlicer::initClass ();
    IgSoSlicerEngine::initClass ();
    IgSoViewpoint::initClass ();
    IgSoPlaneManip::initClass();
    IgSoCircularHist::initClass ();
    IgSoGL2PSAction::initClass ();
}

void
IgOIVBrowser::toggleCameraType (void)
{
    SoType perspectiveType = SoPerspectiveCamera::getClassTypeId ();
    SoType orthoType = SoOrthographicCamera::getClassTypeId ();
    this->setCameraType (this->m_cameraType.isDerivedFrom (perspectiveType)
			 ? orthoType : perspectiveType);
}

void
IgOIVBrowser::setCameraType(SoType t)
{
    ASSERT (this->m_camera);
    ASSERT (this->m_camera->isOfType(SoPerspectiveCamera::getClassTypeId()) 
	   || this->m_camera->isOfType(SoOrthographicCamera::getClassTypeId()));
    
    SoType perspectiveType = SoPerspectiveCamera::getClassTypeId();
    SoType orthoType = SoOrthographicCamera::getClassTypeId();
    SbBool oldIsPerspective = this->m_camera->getTypeId ().isDerivedFrom (perspectiveType);
    SbBool newIsPerspective = t.isDerivedFrom(perspectiveType);

    if ((oldIsPerspective && newIsPerspective) 
	|| (!oldIsPerspective && !newIsPerspective)) // Same old, same old..
    {
	std::cerr << "Same kind of camera!" << std::endl;	
	return;
    }
    
    if (this->m_camera != 0) 
    {
	std::cerr << "Camera found, replacing!" << std::endl;	

	SoCamera * newCamera = (SoCamera *) t.createInstance();

	// Transfer and convert values from one camera type to the other.
	if (newIsPerspective) 
	{
	    IgOIVBrowser::convertOrtho2Perspective((SoOrthographicCamera *) this->m_camera,
						   (SoPerspectiveCamera *) newCamera);
	}
	else 
	{
	    IgOIVBrowser::convertPerspective2Ortho((SoPerspectiveCamera *) this->m_camera,
						   (SoOrthographicCamera *) newCamera);
	}

	SoGroup *cameraParent =
	    getParentOfNode (m_topLevelGroup, this->m_camera);

	cameraParent->insertChild (newCamera,
				   cameraParent->findChild(this->m_camera));
	SoCamera * oldCamera = !this->m_deleteCamera ? this->m_camera : NULL;

	// The setCamera() invokation below will set the saved "home"
	// position of the camera to the current camera position. We make
	// no attempt to avoid this, as it would involve nasty hacks, and
	// it shouldn't really matter.

	this->replaceCamera (newCamera); // This will set PRIVATE(this)->cameratype.

	this->m_deleteCamera = TRUE;
	if (oldCamera) 
	{ 
	    std::cerr << "Removing old camera" << std::endl;	    
	    cameraParent->removeChild (oldCamera); 
	}
    }
    else 
    { // A camera has not been instantiated for the scene.
	this->m_cameraType = t; // No call to setCamera(), so set type explicitly.
    }
}

