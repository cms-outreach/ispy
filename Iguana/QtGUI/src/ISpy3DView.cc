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
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/actions/SoLineHighlightRenderAction.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/Qt/SoQtCursor.h>
#include <QtGui>
#include "classlib/utils/DebugAids.h"
#include <iostream>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodekits/SoBaseKit.h>

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
      m_oldSeek (false)
{
    initWidget ();
}

void debugCameraClipPlanes (void * data, const SbVec2f & nearfar)
{
  SoCamera *camera =  ((ISpy3DView *) data)->getCamera ();
  SoNode *scenegraph =  ((ISpy3DView *) data)->getSceneGraph ();

  SbVec3f pos = camera->position.getValue();

  SoSearchAction action;
  SbBool oldsearch = SoBaseKit::isSearchingChildren();
  SoBaseKit::setSearchingChildren(TRUE);

  action.setSearchingAll(true);
  action.setType(SoCamera::getClassTypeId());
  action.setInterest(SoSearchAction::ALL);
  action.apply(scenegraph);

  SoBaseKit::setSearchingChildren(oldsearch);

  SoPathList &cameras = action.getPaths();

  for (int i = 0, e = cameras.getLength(); i != e; ++i)
    std::cerr << "Camera #" << i << " = " << (void *) cameras[i]->getTail() << " (" << typeid(*cameras[i]->getTail()).name() << ")\n";

  std::cerr << "Calculated clip-planes. Near: " << nearfar[0] << ". Far: " << nearfar[1] << "\n"
            << "Current camera clip-planes. Near: " << camera->nearDistance.getValue() << ", Far: " << camera->farDistance.getValue() << "\n"
            << "Camera ptr: " << (void *) camera << "\n"
            << "Camera name: " << camera->getName() << "\n"
            << "Camera type: " << typeid(*camera).name() << "\n"
            << "Camera position: (" << pos[0] << ", " << pos[1] << ", " << pos[2] << ")\n"
            << "Number of cameras in the scene: " << cameras.getLength() << "\n"
            << "Focal distance: " << camera->focalDistance.getValue () << std::endl;
}

SbVec2f fixedDistanceClipPlanesCB (void * data, const SbVec2f & nearfar)
{
  SoCamera *camera =  ((ISpy3DView *) data)->getCamera ();
  if (0) debugCameraClipPlanes (data, nearfar);

  if (camera->nearDistance.getValue() != nearfar[0])
    camera->nearDistance = nearfar[0];
  if (camera->farDistance.getValue() != nearfar[1])
    camera->farDistance = nearfar[1];

  return nearfar;
}

void
ISpy3DView::initWidget (void) 
{
    setGLRenderAction (new SoLineHighlightRenderAction);
    setEventCallback (eventCallback, this);
    initCamera ();
    setSceneGraph (model ()->sceneGraph ());
    setAutoClippingStrategy (CONSTANT_NEAR_PLANE, 0.9, fixedDistanceClipPlanesCB, this);
    setDecoration (false);
    setupActions ();
    parent ()->setMinimumSize (300, 200);
}

void
ISpy3DView::setupActions (void)
{
//     QAction *actionViewAll = new QAction (parent ());
//     actionViewAll->setObjectName (QString::fromUtf8 ("actionViewAll"));
//     QIcon icon1;
//     icon1.addPixmap (QPixmap (QString::fromUtf8 (":/images/view_all.xpm")), QIcon::Normal, QIcon::Off);
//     actionViewAll->setIcon (icon1);
//     actionViewAll->setText (QApplication::translate ("ISpy3DView", "ViewAll", 0, QApplication::UnicodeUTF8));
// #ifndef QT_NO_TOOLTIP
//     actionViewAll->setToolTip (QApplication::translate ("ISpy3DView", "View All", 0, QApplication::UnicodeUTF8));
// #endif // QT_NO_TOOLTIP

    QAction *actionZoomIn = new QAction (parent ());
    actionZoomIn->setObjectName (QString::fromUtf8 ("actionZoomIn"));
    QIcon icon3;
    icon3.addPixmap (QPixmap (QString::fromUtf8 (":/images/zoom_in.png")), QIcon::Normal, QIcon::Off);
    actionZoomIn->setIcon (icon3);
    actionZoomIn->setText (QApplication::translate ("ISpy3DView", "Zoom In", 0, QApplication::UnicodeUTF8));
    actionZoomIn->setShortcut (QApplication::translate ("ISpy3DView", "Ctrl++", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    actionZoomIn->setToolTip (QApplication::translate ("ISpy3DView", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    
    QAction *actionZoomOut = new QAction (parent ());
    actionZoomOut->setObjectName (QString::fromUtf8("actionZoomOut"));
    QIcon icon4;
    icon4.addPixmap (QPixmap (QString::fromUtf8 (":/images/zoom_out.png")), QIcon::Normal, QIcon::Off);
    actionZoomOut->setIcon (icon4);
    actionZoomOut->setText (QApplication::translate ("ISpy3DView", "Zoom Out", 0, QApplication::UnicodeUTF8));
    actionZoomOut->setShortcut (QApplication::translate ("ISpy3DView", "Ctrl+-", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    actionZoomOut->setToolTip (QApplication::translate ("ISpy3DView", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    QAction *actionHome = new QAction (parent ());
    actionHome->setObjectName (QString::fromUtf8 ("actionHome"));
    QIcon icon2;
    icon2.addPixmap (QPixmap (QString::fromUtf8 (":/images/home.xpm")), QIcon::Normal, QIcon::Off);
    actionHome->setIcon (icon2);
    actionHome->setText (QApplication::translate ("ISpy3DView", "Home", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    actionHome->setToolTip (QApplication::translate ("ISpy3DView", "Home", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
   
    QAction *actionViewPlaneX = new QAction (parent ());
    actionViewPlaneX->setObjectName (QString::fromUtf8("actionViewPlaneX"));
    QIcon icon5;
    icon5.addPixmap (QPixmap (QString::fromUtf8 (":/images/yz_small.xpm")), QIcon::Normal, QIcon::Off);
    actionViewPlaneX->setIcon (icon5);
    actionViewPlaneX->setText (QApplication::translate ("ISpy3DView", "Plane X", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    actionViewPlaneX->setToolTip (QApplication::translate ("ISpy3DView", "Plane X", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    QAction *actionViewPlaneY = new QAction (parent ());
    actionViewPlaneY->setObjectName(QString::fromUtf8("actionViewPlaneY"));
    QIcon icon6;
    icon6.addPixmap (QPixmap (QString::fromUtf8 (":/images/xz_small.xpm")), QIcon::Normal, QIcon::Off);
    actionViewPlaneY->setIcon (icon6);
    actionViewPlaneY->setText (QApplication::translate ("ISpy3DView", "Plane Y", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    actionViewPlaneY->setToolTip (QApplication::translate ("ISpy3DView", "Plane Y", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    QAction *actionViewPlaneZ = new QAction (parent ());
    actionViewPlaneZ->setObjectName (QString::fromUtf8("actionViewPlaneZ"));
    QIcon icon7;
    icon7.addPixmap (QPixmap (QString::fromUtf8 (":/images/yx_small.xpm")), QIcon::Normal, QIcon::Off);
    actionViewPlaneZ->setIcon (icon7);
    actionViewPlaneZ->setText (QApplication::translate ("ISpy3DView", "Plane Z", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    actionViewPlaneZ->setToolTip (QApplication::translate ("ISpy3DView", "Plane Z", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    QAction *actionCameraToggle = new QAction (parent ());
    actionCameraToggle->setObjectName (QString::fromUtf8 ("actionCameraToggle"));
    QIcon icon8;
    icon8.addPixmap (QPixmap (QString::fromUtf8 (":/images/ortho.xpm")), QIcon::Normal, QIcon::On);
    icon8.addPixmap (QPixmap (QString::fromUtf8 (":/images/perspective.xpm")), QIcon::Normal, QIcon::Off);
    actionCameraToggle->setIcon (icon8);
    actionCameraToggle->setCheckable (true);
    actionCameraToggle->setText (QApplication::translate ("ISpy3DView", "CameraToggle", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    actionCameraToggle->setToolTip (QApplication::translate ("ISpy3DView", "CameraToggle", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


//     QObject::connect (actionViewAll, SIGNAL(triggered()), this, SLOT(viewAll()));
    QObject::connect (actionHome, SIGNAL(triggered()), this, SLOT(resetToHomePosition()));
    QObject::connect (actionZoomIn, SIGNAL(triggered ()), this, SLOT(zoomIn ()));
    QObject::connect (actionZoomOut, SIGNAL(triggered ()), this, SLOT(zoomOut ()));
    QObject::connect (actionViewPlaneX, SIGNAL(triggered ()), this, SLOT(viewPlaneX ()));
    QObject::connect (actionViewPlaneY, SIGNAL(triggered ()), this, SLOT(viewPlaneY ()));
    QObject::connect (actionViewPlaneZ, SIGNAL(triggered ()), this, SLOT(viewPlaneZ ()));
    QObject::connect (actionCameraToggle, SIGNAL(triggered()), this, SLOT(toggleCameraType()));

    m_toolBar = new QToolBar (parent ());
    m_toolBar->setObjectName (QString::fromUtf8 ("ISpy3DView::toolBar"));
    m_toolBar->addAction (actionHome);
//     m_toolBar->addAction (actionViewAll);
    m_toolBar->addAction (actionZoomIn);
    m_toolBar->addAction (actionZoomOut);
    m_toolBar->addAction (actionViewPlaneZ);
    m_toolBar->addAction (actionViewPlaneY);
    m_toolBar->addAction (actionViewPlaneX);
    m_toolBar->addAction (actionCameraToggle);

    m_toolBar->setWindowTitle (QApplication::translate ("ISpy3DView", "toolBar", 0, QApplication::UnicodeUTF8));
}

void
ISpy3DView::initCamera (void)
{
    SoOrthographicCamera *camera = new SoOrthographicCamera;
    camera->nearDistance = 1;
    camera->farDistance = 10;
    camera->pointAt (SbVec3f(0.0, 0.0, 0.0));
    camera->scaleHeight (5.5f);
    ((SoGroup *) model()->sceneGraph())->insertChild(camera, 0);
    
//     camera->position.setValue (-18.1, 8.6, 14.0);
//     camera->orientation.setValue (-0.3, -0.93, -0.2, 1.1);
//     camera->nearDistance  = 0.1;
//     camera->farDistance  = 32767;

//     camera->focalDistance = 19.6;
//     camera->pointAt (org);
    
//     SoLight *headLight = getHeadlight ();
//     SoSeparator *root = dynamic_cast<SoSeparator *>(getSceneManager ()->getSceneGraph ());

//     SoPointLight *light_2 = new SoPointLight;
//     light_2->on = true;
//     light_2->intensity = 0.6;
//     light_2->color.setValue (1, 1, 1);
//     light_2->location.setValue (6.7, 41.67, 9.64);
//     root->addChild (light_2);

//     SoPointLight *light_3 = new SoPointLight;
//     light_3->on = true;
//     light_3->intensity = 0.5;
//     light_3->color.setValue (1, 1, 1);
//     light_3->location.setValue (-21.83, -8.81, 13.0);
//     root->addChild (light_3);

//     SoDirectionalLight *light_1 = new SoDirectionalLight;
//     light_1->on = true;
//     light_1->intensity = 0.85;
//     light_1->color.setValue (1, 1, 1);
//     light_1->direction.setValue (0.49, -0.41, -0.77);
//     root->addChild (light_1);
    
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

QToolBar *
ISpy3DView::toolBar (void) const
{ return m_toolBar; }

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
        qDebug () << file << ": Failed to open file for writing.\n";
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
	qDebug() << file << ": Failed to open file for writing.\n";
	QMessageBox::warning (parent (), "System Error",
			      "Failed to open file \""+file+"\" for writing.",
			      "Ok");	
    }
}

void
ISpy3DView::save (void)
{ 
    saveNode (getSceneManager ()->getSceneGraph(), "Save Scene As...", parent ()); 
    // saveNode (model ()->sceneGraph (), "Save Scene As...", getShellWidget ()); 
}

bool
ISpy3DView::saveNode (SoNode *node, const QString& title,
		      QWidget* parent /* = 0 */, const char* file /*= 0*/)
{
    QString f;
    bool binaryfile = false;
    
    if (file == 0)
    {
	QFileDialog dialog (parent, title);
	QString binary ("Binary OIV Files (*.iv)");
	QString ascii ("ASCII OIV Files (*.iv)");
	QStringList filters (ascii);
	filters.append (binary);
	dialog.setFilters (filters);
	dialog.setFileMode (QFileDialog::AnyFile);
	dialog.setAcceptMode (QFileDialog::AcceptSave);	
	dialog.setLabelText (QFileDialog::Accept, "&Save");
	
        bool tryagain = true;
        while (tryagain)
	{
	    if (dialog.exec () != QDialog::Accepted)
		return false;
	    
	    f = dialog.selectedFiles ().front ();
            if (f.isEmpty ())
		return false;
	    else
	    {
		QFile sealf (f);
		if (sealf.exists ())
		{
		    int button = QMessageBox::warning (parent, "File Already Exists",
						       "File \""+f+"\" already exists.\n"
						       "Do you want to overwrite it?",
						       "Yes", "No");
		    if (button == 0)
		    {
			tryagain = false;
		    }
		}	
		else
		    tryagain = false;
	    }

	}
	if (dialog.selectedFilter () == binary)
	    binaryfile = true;
    }
    else
	f = file;
    
    QString fileName (f);
	
    if (! fileName.endsWith (".iv"))
	fileName.append (".iv");

    QDir::setCurrent (QFileInfo (fileName).dir ().absolutePath ());
    return writeNode (node, fileName, binaryfile);
}

bool
ISpy3DView::writeNode (SoNode *node, const QString& file, bool binary,
		       QWidget* parent /* = 0 */)
{
    SoOutput out;
    QApplication::setOverrideCursor (Qt::WaitCursor);
    bool ret = false;
    if (out.openFile (file.toStdString ().c_str ()))
    {
	out.setBinary (binary);
	SoWriteAction writer (&out);
	writer.apply (node);
	out.closeFile ();
	
	ret = true;
    }
    else
    {
	QMessageBox::warning (parent, "System Error",
			      "Failed to open file \""+file+"\" for writing."
			      "Ok");
    }
    QApplication::restoreOverrideCursor ();
    return ret;
}

void
ISpy3DView::print (void)
{
    QString     vector2EPS ("Vector EPS [Level 2] (*.eps)");
    QString     vector3EPS ("Vector EPS [Level 3] (*.eps)");
    QString     vectorPDF  ("Portable Document Format (*.pdf)");
    QStringList filters (vector2EPS);
    filters.append (vector3EPS);
    filters.append (vectorPDF);
    
    SoOffscreenRenderer *renderer =
        new SoOffscreenRenderer (this->getViewportRegion ());
    
    int num = renderer->getNumWriteFiletypes();

    if (num == 0)
    {
        filters.append ("Encapsulated postscript (*.eps)");
	filters.append ("Encapsulated postscript (*.ps)");
	filters.append ("The SGI RGB file format (*.rgb)");
	filters.append ("The SGI RGB file format (*.rgba)");
	filters.append ("The SGI RGB file format (*.bw)");
	filters.append ("The SGI RGB file format (*.inta)");
	filters.append ("The SGI RGB file format (*.int)");
    }
    else
    {
        for (int i = 0; i < num; i++)
	{
	    SbPList extlist;
	    SbString fullname;
	    SbString description;
	    renderer->getWriteFiletypeInfo (i, extlist, fullname, description);
	    QString filter (fullname.getString ());
            filter += " (*.";
	    for (int j = 0; j < extlist.getLength (); j++)
		filters.append (filter + (const char *)extlist [j] + ")");
	}
    }
    delete renderer;
    
    // Pop up file dialog to as for the name.
    QFileDialog dialog (parent (), "Print To File");
    dialog.setFilters (filters);
    dialog.setFileMode (QFileDialog::AnyFile);
    dialog.setAcceptMode (QFileDialog::AcceptSave);	
    dialog.setLabelText (QFileDialog::Accept, "&Save");
    bool tryagain = true;
    QString     f;
    while (tryagain)
    {
        if (dialog.exec () != QDialog::Accepted)
	    return;
	
	f = dialog.selectedFiles ().front ();
	if (f.isEmpty ())
	    return;
	else
	{
	    QFile sealf (f);
	    if (sealf.exists ())
	    {
		int button = QMessageBox::warning (parent (), "File Already Exists",
						   "File \""+f+"\" already exists.\n"
						   "Do you want to overwrite it?",
						   "Yes", "No");
		if (button == 0)
		{
		    tryagain = false;
		}
	    }
	    else
		tryagain = false;
	}
    }
    // Pick format settings.
    float       ppi = SoOffscreenRenderer::getScreenPixelsPerInch ();
    //float     dpi = 300.;
    float       dpi = ppi;
    QString     format ("jpg");
    
    QString sfilter = dialog.selectedFilter ();
    format = sfilter.section ('.', -1);
    format.remove (QChar (')'));    
    
    // Add suffix.
    QString suffix ("." + format);
    if (! f.endsWith (suffix))
        f += suffix;
    
    // FIXME: Use a state element to remeber all file operations(save, open)
    QDir::setCurrent (QFileInfo (f).absoluteFilePath ());
    
    QApplication::setOverrideCursor (Qt::WaitCursor);
    if (sfilter == vector2EPS)
        printVector (f, format, 2);
    else if (sfilter == vector3EPS)
        printVector (f, format, 3);
    else if (sfilter == vectorPDF)
        printVector (f, format, 0);
    else
        printBitmap (f, ppi, dpi, format);
    QApplication::restoreOverrideCursor ();
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

    // FIXME: This is needed because the heighAngle values could get huge 
    // when coming from orthographic camera.
    // It resets them to 45 degrees although some better logic would
    // be nice so that the detector does not change dimensions when
    // going from orthographic to perspective view.
    if (camera->isOfType(SoPerspectiveCamera::getClassTypeId()))
    {
      ((SoPerspectiveCamera *)camera)->heightAngle = 0.785398163;
    }

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
