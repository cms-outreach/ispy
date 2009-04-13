//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT

#include "Iguana/QtGUI/interface/ISpy3DView.h"
#include "Iguana/QtGUI/interface/Ig3DBaseModel.h"
#include "Iguana/QtGUI/interface/Ig3DBaseRep.h"
#include "Iguana/QtGUI/interface/IgSoGL2PSAction.h"
#include "Iguana/QtGUI/interface/gl2ps.h"
#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/Inventor/interface/IgSoGridPlane.h"
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoLineHighlightRenderAction.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/Qt/SoQtCursor.h>
#include <QtGui>
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Log.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>

lat::logflag LF3DView = { 0, "iSpy3D", true, -1 };

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpy3DView::ISpy3DView (IgState *state, Ig3DBaseModel *model, QWidget *parent)
    : SoQtExaminerViewer (parent, "iSpy 3D"),
      m_state (new IgState (state)),
      m_parent (parent),
      m_model (model),
      m_gl2psOptions (GL2PS_SIMPLE_LINE_OFFSET 
		      | GL2PS_BEST_ROOT
		      | GL2PS_OCCLUSION_CULL),
      m_gl2psFBBufferSize (1024*1024),
      m_whatsThisPicking (false),
      m_grid (false),
      m_oldView (true),
      m_oldSeek (false),
      m_farDistanceSensor (0),
      m_nearDistanceSensor (0)
{
    initWidget ();
}

void
ISpy3DView::initWidget (void) 
{
    setGLRenderAction (new SoLineHighlightRenderAction);
    setEventCallback (eventCallback, this);
    setSceneGraph (model ()->sceneGraph ());
    initCamera ();
    
    show ();
}

void
ISpy3DView::initCamera (void)
{
    //FIXME: remove when SoQt fixes the continious rendering problem
    //create sersors to sence the near and for clip plane distance and
    //force them to be 0.1 and SHRT_MAX
    SoCamera * const camera = SoQtExaminerViewer::getCamera ();
    
    ASSERT (camera);
    camera->farDistance  = SHRT_MAX;
    camera->nearDistance  = 0.1;
    m_farDistanceSensor = new SoFieldSensor (&farDistanceSensorCB, this);
    m_farDistanceSensor->attach (&camera->farDistance);
    m_nearDistanceSensor = new SoFieldSensor (&nearDistanceSensorCB, this);
    m_nearDistanceSensor->attach (&camera->nearDistance);
    
//     SoNode *node = model->attachPoint ()->findMagic (
//         Ig3DBaseModel::encode ("Default Grid Group"));
//     if (node && dynamic_cast<SoGroup*>(node)->getNumChildren ())
//         m_grid = true;
}

ISpy3DView::~ISpy3DView (void) 
{}

IgState *
ISpy3DView::state (void) const
{ return m_state; }

QWidget *
ISpy3DView::parent (void) const
{ return m_parent; }

Ig3DBaseModel *
ISpy3DView::model (void) const
{ return m_model; }

void
ISpy3DView::printBitmap (QString file, float ppi,
			 float dpi, QString format)
{
    // This is mostly like the *Inventor Mentor* example.
    // Initialise off-screen renderer.
    float               r, g, b;
    SbViewportRegion    outvr = this->getViewportRegion ();
    
    SbVec2s             pixels (outvr.getViewportSizePixels ());
    SbVec2s             size ((short) (pixels [0] * dpi / ppi + 0.5), (short) (pixels [1] * dpi / ppi + 0.5));
    SbVec2s             origin = outvr.getViewportOriginPixels();
    outvr.setViewportPixels (origin, size);

    // Set up a custom GL render action for the offscreen rendered.
    // Do *not* use the one returned by `getGLRenderAction()': doing
    // so leaves the display in a confused state and doesn't produce
    // an output file.  This way we also avoid issues with having to
    // mess and then later reset antialiasing and related parameters.
    SoGLRenderAction    *ra = new SoGLRenderAction (outvr);
    SoOffscreenRenderer *renderer = new SoOffscreenRenderer (outvr);

    // FIXME? renderer.setComponents (SoOffscreenRenderer::RGB_TRANSPARENCY);
    getSceneManager ()->getBackgroundColor ().getValue (r, g, b);
    renderer->setBackgroundColor (SbColor (r, g, b));
    renderer->setGLRenderAction (ra);
    ra->setTransparencyType (SoGLRenderAction::SORTED_OBJECT_BLEND);
    ra->setSmoothing (TRUE);
    ra->setNumPasses (8);
    
    // Want to render from above the SceneGraph so we get what the
    // camera sees; SoQtViewer uses the following code.  (FIXME:
    // do we actually want to just render root, or look for camera?)
    SoNode *root = getSceneManager ()->getSceneGraph ();
    
    SbBool ok = renderer->render (root);
    if (!ok) 
    {
        QMessageBox::information (parent (), "IGUANA Print Info",
				  tr ("Printing of the %1 format works only\n"
				      "if you run locally installed software\n"
				      "If iguana is running remotely, please,\n"
				      "print as vector Postscript.").arg (file.right (3)));
    }
    else if (!renderer->writeToFile (file.toStdString ().c_str (), format.toStdString ().c_str ()))
    {
        LOG(0, trace, LF3DView, 
	    QString(file + ": Failed to open file for writing.\n").toStdString ());
	QMessageBox::warning (getShellWidget (), "System Error",
			      "Failed to open file \""+file+"\" for writing.",
			      "Ok");
    }
    delete renderer;
    delete ra;
}

void
ISpy3DView::printVector (QString file, QString format, int level)
{
    // Use gl2ps to print vector graphics output.  To be extended to
    // handle other vector graphics formats (SVG, WMF).
    static IgSoGL2PSAction  *gl2psAction = 0;
    if (FILE *output = fopen (file.toStdString ().c_str (), "wb+"))
    {
        int type = GL2PS_EPS;
	if (format == "pdf")
	    type = GL2PS_PDF;
	else if (format == "eps")
	    type = GL2PS_EPS;
	else
	    ASSERT (0);

	if (! gl2psAction )
	    gl2psAction = new IgSoGL2PSAction (this->getViewportRegion ());
	
	gl2psAction->setViewportRegion (this->getViewportRegion ());
	SoGLRenderAction* prevAction = this->getGLRenderAction ();
	setGLRenderAction (gl2psAction);
	
	int state = GL2PS_OVERFLOW;
	while (state == GL2PS_OVERFLOW)
	{ 
	    int gl2psOptions = GL2PS_SILENT | GL2PS_USE_CURRENT_VIEWPORT
			       | (level < 3 ? GL2PS_NO_PS3_SHADING : 0)
			       | getGL2PSOptions ();
	    gl2psBeginPage ("IGUANA Scene Graph", "IGUANA", NULL,
			    type, GL2PS_BSP_SORT,
			    gl2psOptions,
			    GL_RGBA, 0, NULL,0, 0, 0,
			    m_gl2psFBBufferSize, output, NULL);
	    actualRedraw ();
	    state = gl2psEndPage();
	    if (state == GL2PS_OVERFLOW)
		m_gl2psFBBufferSize += 1024*1024;
	}
	fclose (output);
	setGLRenderAction (prevAction);
    }
    else
    {
        LOG(0, trace, LF3DView, QString(file +": Failed to open file for writing.\n").toStdString ());
	QMessageBox::warning (parent (), "System Error",
			      "Failed to open file \""+file+"\" for writing.",
			      "Ok");	
    }
}

void
ISpy3DView::zoomIn (void) 
{    
    // There is no particular meaning in this 
    // number 0.5. It just looks like a reasonable step. 
    zoom (-0.5f);
}

void
ISpy3DView::zoomOut (void) 
{    
     // There is no particular meaning in this 
     // number 0.5. It just looks like a reasonable step. 
     zoom (0.5f);
}

void
ISpy3DView::zoom (const float diffvalue)
{
    SoCamera *cam = this->getCamera ();
    if (cam == NULL) return; // can happen for empty scenegraph
    SoType t = cam->getTypeId ();
    
    // This will be in the range of <0, ->>.
    float multiplicator = exp (diffvalue);
    
    if (t.isDerivedFrom (SoOrthographicCamera::getClassTypeId ())) 
    {
        // Since there's no perspective, "zooming" in the original sense
	// of the word won't have any visible effect. So we just increase
	// or decrease the field-of-view values of the camera instead, to
	// "shrink" the projection size of the model / scene.
	SoOrthographicCamera *oc = (SoOrthographicCamera *) cam;
	oc->height = oc->height.getValue () * multiplicator;
    }
    else if (t.isDerivedFrom (SoPerspectiveCamera::getClassTypeId ())) 
    {
        float oldfocaldist = cam->focalDistance.getValue ();
	cam->focalDistance = oldfocaldist * multiplicator;
	
	SbVec3f direction;
	cam->orientation.getValue ().multVec (SbVec3f (0, 0, -1), direction);
	cam->position = cam->position.getValue () +
			(cam->focalDistance.getValue () - oldfocaldist) * -direction;
	
    }
    else 
    {
        static SbBool first = TRUE;
	if (first) 
	{
	    SoDebugError::postWarning("Ig3DBaseBrowser::zoom",
				      "unknown camera type, "
				      "attempts to zoom will have no effect");
	    first = FALSE;
	}
    }
}

void
ISpy3DView::resetToHomePosition (void)
{
    SoQtViewer::resetToHomePosition ();    
}

void
ISpy3DView::saveHomePosition (void)
{
    SoQtViewer::saveHomePosition ();    
}

void
ISpy3DView::viewAll (void)
{
    SoQtViewer::viewAll ();
}

void
ISpy3DView::seek (void)
{
    if (SoQtViewer::isSeekMode())
    {
	SoQtViewer::setSeekMode (false);
	setComponentCursor (SoQtCursor::getRotateCursor ());
    }
    else
    {
	SoQtViewer::setSeekMode (true);
	if (isAnimating ()) { stopAnimating (); }
	setComponentCursor(SoQtCursor(SoQtCursor::CROSSHAIR));
    }
}

void
ISpy3DView::setFeedbackVisibility (bool enable)
{
    bool old = isFeedbackVisible ();
    if (enable != old)
        SoQtExaminerViewer::setFeedbackVisibility (enable);
}

void
ISpy3DView::setGridVisibility (bool enable)
{     
    ISpy3DView::drawGrid (enable);
}

void
ISpy3DView::view (void)
{
    if (isViewing () != true)
    {
	m_whatsThisPicking = false;
	SoQtViewer::setViewing (true);    
	setComponentCursor (SoQtCursor::getRotateCursor ());    
    }
}

void
ISpy3DView::pick (void)
{
    if (isViewing () != false)
    {
	SoQtViewer::setViewing (false);    
	SoQtViewer::setSeekMode (false);
	setComponentCursor(SoQtCursor(SoQtCursor::DEFAULT));
    }    
}

void
ISpy3DView::drawGrid (const bool enable) 
{
    // find the grid planes group
    SoNode* grid = findGroup (model ()->attachPoint (), Ig3DBaseModel::encode ("Grid Planes").getString ());
    SoGroup* all = 0;
    
    if (grid)
    {
        // and now find the group which contains all planes
	all = dynamic_cast<SoGroup *> (findGroup (grid, Ig3DBaseModel::encode ("All").getString ()));
    }
    
    if (all)
    {
        unsigned nbrChildren = all->getNumChildren ();
	IgSoGridPlane* plane = 0;
	
	// iterate over all planes and enable or disable them
	for (unsigned i = 0; i < nbrChildren; i++)
	{
	    plane = dynamic_cast<IgSoGridPlane* > (all->getChild (i));
	    if (plane)
	    {
		plane->on = enable;
	    }
	}
    }
    m_grid = enable;
}

int
ISpy3DView::getGL2PSOptions (void)
{ return m_gl2psOptions; }

SoNode*
ISpy3DView::findGroup (SoNode *node, const char* name)
{
    if (node->isOfType(SoGroup::getClassTypeId()))
    {
	if (node->getName () == name)
	    return node;
	else
	{
	    SoGroup * group = dynamic_cast<SoGroup*>(node);
	    int count = group->getNumChildren ();
	    for (int i = 0; i < count; i++)
	    {
		SoNode *n = findGroup (group->getChild(i), name);
		if (n) return n;
	    }
	}
    }
    return 0;
}

void
ISpy3DView::toggleCameraType (void)
{
    SoQtExaminerViewer::toggleCameraType ();
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer

    camera->farDistance  = SHRT_MAX;
    camera->nearDistance  = 0.1;
    m_farDistanceSensor->attach (&camera->farDistance);
    m_nearDistanceSensor->attach (&camera->nearDistance);
    cameraToggled();
}

void
ISpy3DView::invertCamera (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer
    
    camera->position = camera->position.getValue () * -1.0F;
    camera->orientation.setValue (
        SbRotation (SbVec3f (0.F,-1.F,0.F), M_PI)
	* camera->orientation.getValue ());
}

void
ISpy3DView::farDistanceSensorCB (void *me, SoSensor *)
{
    ISpy3DView *self = static_cast<ISpy3DView *> (me);
    if (self->isAutoClipping ())
    {
        SoCamera * const camera = self->getCamera ();
	self->m_farDistanceSensor->detach ();
	camera->farDistance  = SHRT_MAX;
	self->m_farDistanceSensor->attach (&camera->farDistance);
    }
}

void
ISpy3DView::nearDistanceSensorCB (void *me, SoSensor *)
{
    ISpy3DView *self = static_cast<ISpy3DView *> (me);
    if (self->isAutoClipping ())
    {
        SoCamera * const camera = self->getCamera ();
	self->m_nearDistanceSensor->detach ();
	camera->nearDistance  = 0.1;
	self->m_nearDistanceSensor->attach (&camera->nearDistance);
    }
}

void
ISpy3DView::autoPrint (void)
{    
    autoPrint (this->getTitle ());
}

void
ISpy3DView::autoPrint (const std::string text)
{
    QDateTime dt = QDateTime::currentDateTime ();
    QString fName = "screenShot-" + dt.toString ("hh:mm:ss.zzz-dd.MM.yyyy") + ".png";
    QString dName = "screenShot-" + dt.toString ("hh:mm:ss.zzz-dd.MM.yyyy") + ".date";
    
    SbColor c = getBackgroundColor ();
    SoOffscreenRenderer osr (this->getViewportRegion ());
    osr.setBackgroundColor (c);
    SoNode *root = getSceneManager ()->getSceneGraph ();
    SbBool ok = osr.render (root);
    
    if (!ok) { return; }
    // ok = osr.writeToRGB (fName);
    ok = osr.writeToFile (fName.toStdString ().c_str (), "png");
    if (!ok) { return; }
    
    dt = QDateTime::currentDateTime ();
    QFile file (dName);
    if  (file.open (QIODevice::WriteOnly)) 
    {
	QTextStream stream (&file);
	stream << dt.toString ("ddd MMM d hh:mm:ss.zzz yyyy") << "\n";
	stream << QString::fromStdString (text) << "\n";
	file.close ();
    }
}

void
ISpy3DView::viewPlaneX (void) 
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer
    
    camera->position = SbVec3f(-1,0,0) * camera->position.getValue().length();
    camera->orientation = SbRotation(SbVec3f(0,1,0),-M_PI/2.f);
}

void
ISpy3DView::viewPlaneY (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer
    
    SbVec3f norient = SbVec3f(0,-1,0);
    camera->position = -norient * camera->position.getValue().length();
    camera->orientation = SbRotation(SbVec3f(0,0,1),-M_PI/2.f) *
			  SbRotation(SbVec3f(0,0,-1),norient);
}

void
ISpy3DView::viewPlaneZ (void) 
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer
    
    camera->position = SbVec3f(0,0,1) * camera->position.getValue().length();
    camera->orientation = SbRotation::identity();
}

void
ISpy3DView::setGL2PSOptions (int options)
{ m_gl2psOptions = options; }

void
ISpy3DView::setWhatsThisPicking (bool enable /* = true */)
{
    if (m_whatsThisPicking != enable)
    {
	m_whatsThisPicking = enable;
	if (enable)
	{
	    m_oldView = isViewing ();
	    m_oldSeek = isSeekMode();
	    pick ();
	    getGLWidget ()->setCursor (QCursor (Qt::WhatsThisCursor));
	}
	else
	{
	    if (m_oldView)
	    {
		view ();
		if (m_oldSeek)
		    seek ();
	    }
	    else
	    {
		pick ();
		setComponentCursor(SoQtCursor(SoQtCursor::DEFAULT));
	    }
	}
    }
}

SbBool
ISpy3DView::eventCallback (void *closure, QEvent *event)
{
    ISpy3DView *self = static_cast<ISpy3DView *> (closure);
    if (!self->getParentWidget()->testAttribute(Qt::WA_UnderMouse) ||
        !self->isViewing())
        return false;
    
    if (QWheelEvent *wheelEvent = dynamic_cast<QWheelEvent *> (event))
    {
        self->zoom (0.1 * (wheelEvent->delta () > 0 ? 1:-1));
	return true;
    }
    else if (QMouseEvent *mEvent = dynamic_cast<QMouseEvent *> (event))
    {
        if (mEvent->button () == Qt::LeftButton
	    &&  mEvent->type () == QEvent::MouseButtonRelease
	    && self->isSeekMode())
	{
	    self->setComponentCursor (SoQtCursor::getRotateCursor ());
	    return true;
	}
	return false;
    }
    return false;
}
