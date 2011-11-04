//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT

#include "QtGUI/ISpy3DView.h"
#include "QtGUI/Ig3DBaseModel.h"
#include "QtGUI/IgSoGridPlane.h"
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/actions/SoLineHighlightRenderAction.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/Qt/SoQtCursor.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/Qt/devices/SoQtMouse.h>
#include <QtGui>
#include <iostream>
#include <typeinfo>

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SoPath.h>
#include <Inventor/details/SoDetail.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/details/SoLineDetail.h>

#include <QtGlobal>
#include <QDebug>

ISpy3DView::ISpy3DView(Ig3DBaseModel *model, QWidget *parent)
  : SoQtExaminerViewer(parent, "iSpy 3D"),
    m_parent(parent),
    m_model(model),
    m_whatsThisPicking(false),
    m_grid(false),
    m_oldView(true),
    m_oldSeek(false),
	m_interact(false),
    m_viewMode(ROTATION_MODE)
{
  initWidget();
  SoSelection *sel = dynamic_cast<SoSelection*>(getSceneGraph());
  if (sel)
  {
	  redrawOnSelectionChange(sel);
  }

}

void debugCameraClipPlanes(void * data, const SbVec2f & nearfar)
{
  SoCamera *camera = ((ISpy3DView *) data)->getCamera();
  SoNode *scenegraph = ((ISpy3DView *) data)->getSceneGraph();

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
    std::cerr << "Camera #" << i << " = " << (void *) cameras[i]->getTail() << "(" << typeid(*cameras[i]->getTail()).name() << ")\n";

  std::cerr << "Calculated clip-planes. Near: " << nearfar[0] << ". Far: " << nearfar[1] << "\n"
            << "Current camera clip-planes. Near: " << camera->nearDistance.getValue() << ", Far: " << camera->farDistance.getValue() << "\n"
            << "Camera ptr: " << (void *) camera << "\n"
            << "Camera name: " << camera->getName() << "\n"
            << "Camera type: " << typeid(*camera).name() << "\n"
            << "Camera position:(" << pos[0] << ", " << pos[1] << ", " << pos[2] << ")\n"
            << "Number of cameras in the scene: " << cameras.getLength() << "\n"
            << "Focal distance: " << camera->focalDistance.getValue() << std::endl;
}

SbVec2f fixedDistanceClipPlanesCB(void * data, const SbVec2f & nearfar)
{
  SoCamera *camera = ((ISpy3DView *) data)->getCamera();
  if (0) debugCameraClipPlanes(data, nearfar);

  if (camera->nearDistance.getValue() != nearfar[0])
    camera->nearDistance = nearfar[0];
  if (camera->farDistance.getValue() != nearfar[1])
    camera->farDistance = nearfar[1];

  return nearfar;
}

void
ISpy3DView::initWidget(void)
{
  setGLRenderAction(new SoLineHighlightRenderAction);
  setEventCallback(eventCallback, this);
  setSceneGraph(model()->sceneGraph());
  getSceneManager()->getGLRenderAction()->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);

  // LT FIXME: the following adds GL smoothing (anti-aliasing). It works but seems to make rotations "sticky". Can it be done better? 
  getSceneManager()->getGLRenderAction()->setSmoothing (TRUE);  // LT FIXME: "Computationally-cheap anti-aliasing" (smooths jagged line and points)
  setAutoClippingStrategy(CONSTANT_NEAR_PLANE, 0.9, fixedDistanceClipPlanesCB, this);
  setDecoration(false);
  parent()->setMinimumSize(300, 200);
}

void
ISpy3DView::setCamera(SoCamera *camera)
{
  model()->setCamera(camera);
  SoQtExaminerViewer::setCamera(camera);
}

ISpy3DView::~ISpy3DView(void)
{}

QWidget *
ISpy3DView::parent(void) const
{ return m_parent; }

Ig3DBaseModel *
ISpy3DView::model(void) const
{ return m_model; }

void
ISpy3DView::printBitmap(const QString &file, 
			const QString &format)
{
  // This is mostly like the *Inventor Mentor* example.
  // Initialise off-screen renderer.
  float               r, g, b;
  SbViewportRegion    outvr = this->getViewportRegion();

  SbVec2s             pixels(outvr.getViewportSizePixels());
  SbVec2s             size((short)(pixels[0] + 0.5), (short)(pixels[1] + 0.5));
  if (getenv("ISPY_HD_PRINT"))
  {
    size[0] = 1920;
    size[1] = 1080;
  }
  SbVec2s             origin = outvr.getViewportOriginPixels();
  outvr.setViewportPixels(origin, size);

  SoOffscreenRenderer *renderer = new SoOffscreenRenderer(outvr);

  getSceneManager()->getBackgroundColor().getValue(r, g, b);
  renderer->setBackgroundColor(SbColor(r, g, b));

  // Want to render from above the SceneGraph so we get what the
  // camera sees; SoQtViewer uses the following code.(FIXME:
  // do we actually want to just render root, or look for camera?)
  SoNode *root = getSceneManager()->getSceneGraph();

  if(renderer->render(root))
  {
    unsigned char *buffer = renderer->getBuffer();
    int width = size[0];
    int height = size[1];
    QImage image(width, height, QImage::Format_RGB32);
    QRgb value;

    for(int j = 0; j < height; j++)
      for(int k = 0; k < width; k++)
      {
	value = qRgb(buffer[0], buffer[1], buffer[2]);
	image.setPixel(k, j, value);
	buffer += 3;
      }
    QImage mimage = image.mirrored();
    if(!mimage.save(file, format.toAscii()))
      QMessageBox::warning(getShellWidget(), "iSpy Save Image Error",
			   "Failed to save an image.",
			   "Ok");
      
  }
  else
  {
    QMessageBox::warning(getShellWidget(), "iSpy Off-Screen Renderer Error",
			 "Failed to render a scene graph.",
			 "Ok");
  }
  delete renderer;
}

void
ISpy3DView::save(void)
{
  // Get the list of supported image formats
  // Qt should support a lot of formats by default,
  // but I do not get them...
  QList<QByteArray> extList = QImageWriter::supportedImageFormats();
  QStringList strList;
  for(int i = 0; i < extList.length(); ++i)
    strList << QString(extList.at(i));
  
  QStringList filters;
  if(strList.contains("PNG", Qt::CaseInsensitive))
     filters.append("The PNG file format(*.png)");

  // Pop up file dialog to as for the name.
  QFileDialog dialog(parent(), "Save To File");
  dialog.setFilters(filters);
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setLabelText(QFileDialog::Accept, "&Save");
  bool tryagain = true;
  QString     f;
  while (tryagain)
  {
    if (dialog.exec() != QDialog::Accepted)
      return;

    f = dialog.selectedFiles().front();
    if (f.isEmpty())
      return;
    else
    {
      QFile sealf(f);
      if (sealf.exists())
      {
        int button = QMessageBox::warning(parent(), "File Already Exists",
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
  QString sfilter = dialog.selectedFilter();
  QString format = sfilter.section('.', -1);
  format.remove(QChar(')'));

  // Add suffix.
  QString suffix("." + format);
  if (! f.endsWith(suffix))
    f += suffix;

  // FIXME: Use a state element to remeber all file operations(save, open)
  QDir::setCurrent(QFileInfo(f).absoluteFilePath());

  QApplication::setOverrideCursor(Qt::WaitCursor);
  printBitmap(f, format);
  QApplication::restoreOverrideCursor();

  //FIXME: Should be export menu:
  //saveNode(getSceneManager()->getSceneGraph(), "Save Scene As...", parent());
  //saveNode(model()->sceneGraph(), "Save Scene As...", getShellWidget());
}
void
ISpy3DView::exportIV(void)
{
	saveNode(m_model->sceneGraph(),"Export as Inventor");
}
bool
ISpy3DView::saveNode(SoNode *node, const QString& title,
                     QWidget* parent /* = 0 */, const char* file /*= 0*/)
{
  QString f;
  bool binaryfile = false;

  if (file == 0)
  {
    QFileDialog dialog(parent, title);
    QString binary("Binary OIV Files(*.iv)");
    QString ascii("ASCII OIV Files(*.iv)");
    QStringList filters(ascii);
    filters.append(binary);
    dialog.setFilters(filters);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setLabelText(QFileDialog::Accept, "&Save");

    bool tryagain = true;
    while (tryagain)
    {
      if (dialog.exec() != QDialog::Accepted)
        return false;

      f = dialog.selectedFiles().front();
      if (f.isEmpty())
        return false;
      else
      {
        QFile sealf(f);
        if (sealf.exists())
        {
          int button = QMessageBox::warning(parent, "File Already Exists",
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
    if (dialog.selectedFilter() == binary)
      binaryfile = true;
  }
  else
    f = file;

  QString fileName(f);

  if (! fileName.endsWith(".iv"))
    fileName.append(".iv");

  QDir::setCurrent(QFileInfo(fileName).dir().absolutePath());
  return writeNode(node, fileName, binaryfile);
}

bool
ISpy3DView::writeNode(SoNode *node, const QString& file, bool binary,
                      QWidget* parent /* = 0 */)
{
  SoOutput out;
  QApplication::setOverrideCursor(Qt::WaitCursor);
  bool ret = false;
  if (out.openFile(file.toStdString().c_str()))
  {
    out.setBinary(binary);
    SoWriteAction writer(&out);
    writer.apply(node);
    out.closeFile();

    ret = true;
  }
  else
  {
    QMessageBox::warning(parent, "System Error",
                         "Failed to open file \""+file+"\" for writing."
                         "Ok");
  }
  QApplication::restoreOverrideCursor();
  return ret;
}

void
ISpy3DView::print(void)
{
#ifndef QT_NO_PRINTER
  QPrintDialog pdialog(&m_printer, m_parent);
  if(pdialog.exec())
  {
    QPainter painter(&m_printer);
    QRect rect = painter.viewport();

    QList<QByteArray> extList = QImageWriter::supportedImageFormats();
    
    if(! extList.isEmpty())
    {     
      QTemporaryFile tmpFile;
      if(tmpFile.open())
      {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	printBitmap(tmpFile.fileName(), QString(extList.at(0)));
	QApplication::restoreOverrideCursor();
      }
      QPixmap pxm (tmpFile.fileName());
      if(!pxm.isNull())
      {	
	QSize size = pxm.size();
	size.scale(rect.size(), Qt::KeepAspectRatio);
	painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
	painter.setWindow(pxm.rect());
	painter.drawPixmap(0, 0, pxm);
      }
      else
	QMessageBox::warning(parent(), "iSpy Print Error",
			     "Failed to print the image.",
			     "Ok");
    }
    else
      QMessageBox::warning(parent(), "iSpy Print Error",
			   "No image formats are supported by Qt.",
			   "Ok");
  }
#endif
}

void
ISpy3DView::zoomIn(void)
{
  // There is no particular meaning in this
  // number 0.5. It just looks like a reasonable step.
  zoom(-0.5f);
}

void
ISpy3DView::zoomOut(void)
{
  // There is no particular meaning in this
  // number 0.5. It just looks like a reasonable step.
  zoom(0.5f);
}

void
ISpy3DView::zoom(const float diffvalue)
{
  SoCamera *cam = this->getCamera();
  if (cam == NULL) return; // can happen for empty scenegraph
  SoType t = cam->getTypeId();

  // This will be in the range of <0, ->>.
  float multiplicator = exp(diffvalue);

  if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
  {
    // Since there's no perspective, "zooming" in the original sense
    // of the word won't have any visible effect. So we just increase
    // or decrease the field-of-view values of the camera instead, to
    // "shrink" the projection size of the model / scene.
    SoOrthographicCamera *oc = (SoOrthographicCamera *) cam;
    oc->height = oc->height.getValue() * multiplicator;
  }
  else if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
  {
    float oldfocaldist = cam->focalDistance.getValue();
    cam->focalDistance = oldfocaldist * multiplicator;

    SbVec3f direction;
    cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
    cam->position = cam->position.getValue() +
                    (cam->focalDistance.getValue() - oldfocaldist) * -direction;

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
ISpy3DView::saveHomePosition(void)
{
  SoQtViewer::saveHomePosition();
}

void
ISpy3DView::viewAll(void)
{
  SoQtViewer::viewAll();
}

void
ISpy3DView::seek(void)
{
  if (SoQtViewer::isSeekMode())
  {
    SoQtViewer::setSeekMode(false);
    setComponentCursor(SoQtCursor::getRotateCursor());
  }
  else
  {
    SoQtViewer::setSeekMode(true);
    if (isAnimating()) { stopAnimating(); }
    setComponentCursor(SoQtCursor(SoQtCursor::CROSSHAIR));
  }
}

void
ISpy3DView::setFeedbackVisibility(bool enable)
{
  bool old = isFeedbackVisible();
  if (enable != old)
    SoQtExaminerViewer::setFeedbackVisibility(enable);
}

void
ISpy3DView::setGridVisibility(bool enable)
{
  ISpy3DView::drawGrid(enable);
}

void
ISpy3DView::view(void)
{
  if (isViewing() != true)
  {
    m_whatsThisPicking = false;
    SoQtViewer::setViewing(true);
    setComponentCursor(SoQtCursor::getRotateCursor());
  }
}

void
ISpy3DView::pick(void)
{
  if (isViewing() != false)
  {
    SoQtViewer::setViewing(false);
    SoQtViewer::setSeekMode(false);
    setComponentCursor(SoQtCursor(SoQtCursor::DEFAULT));
  }
}

void
ISpy3DView::drawGrid(const bool enable)
{
  // find the grid planes group
  SoNode* grid = findGroup(model()->contents(), Ig3DBaseModel::encode("Grid Planes").getString());
  SoGroup* all = 0;

  if (grid)
  {
    // and now find the group which contains all planes
    all = dynamic_cast<SoGroup *>(findGroup(grid, Ig3DBaseModel::encode("All").getString()));
  }

  if (all)
  {
    unsigned nbrChildren = all->getNumChildren();
    IgSoGridPlane* plane = 0;

    // iterate over all planes and enable or disable them
    for (unsigned i = 0; i < nbrChildren; i++)
    {
      plane = dynamic_cast<IgSoGridPlane* > (all->getChild(i));
      if (plane)
      {
        plane->on = enable;
      }
    }
  }
  m_grid = enable;
}

SoNode*
ISpy3DView::findGroup(SoNode *node, const char* name)
{
  if (node->isOfType(SoGroup::getClassTypeId()))
  {
    if (node->getName() == name)
      return node;
    else
    {
      SoGroup * group = dynamic_cast<SoGroup*>(node);
      int count = group->getNumChildren();
      for (int i = 0; i < count; i++)
      {
        SoNode *n = findGroup(group->getChild(i), name);
        if (n) return n;
      }
    }
  }
  return 0;
}

void
ISpy3DView::toggleCameraType(void)
{
  SoQtExaminerViewer::toggleCameraType();
  SoCamera * const camera = this->getCamera();
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
ISpy3DView::setCameraType(QAction * /*action*/)
{
  toggleCameraType();
}

void
ISpy3DView::invertCamera(void)
{
  SoCamera * const camera = this->getCamera();
  if (!camera) return; // probably a scene-less viewer

  camera->position = camera->position.getValue() * -1.0F;
  camera->orientation.setValue(
    SbRotation(SbVec3f(0.F,-1.F,0.F), M_PI)
    * camera->orientation.getValue());
}

void
ISpy3DView::setEventMessage(const QString &message)
{ 
  m_currentEvent = message;
}


void
ISpy3DView::autoPrint(QString text)
{
  QDateTime dt = QDateTime::currentDateTime();
  qDebug() << m_currentEvent << "--";
  QString fName = "iSpy-" + dt.toString("hh-mm-ss-zzz-dd-MM-yyyy") + ".png";
  QString dName = "iSpy-" + m_currentEvent + dt.toString("-hh:mm:ss.zzz-dd.MM.yyyy") + ".date";

  SbColor c = getBackgroundColor();
  SbViewportRegion    outvr = this->getViewportRegion();
  SoOffscreenRenderer renderer(outvr);
  SbVec2s             pixels(outvr.getViewportSizePixels());
  SbVec2s             size((short)(pixels[0] + 0.5), (short)(pixels[1] + 0.5));
  SbVec2s             origin = outvr.getViewportOriginPixels();
  outvr.setViewportPixels(origin, size);
  renderer.setBackgroundColor(c);
  SoNode *root = getSceneManager()->getSceneGraph();
  if(! renderer.render(root)) { return; }
  
  unsigned char *buffer = renderer.getBuffer();
  int width = size[0];
  int height = size[1];
  QImage image(width, height, QImage::Format_RGB32);
  QRgb value;

  for(int j = 0; j < height; j++)
    for(int k = 0; k < width; k++)
    {
      value = qRgb(buffer[0], buffer[1], buffer[2]);
      image.setPixel(k, j, value);
      buffer += 3;
    }

  QImage mimage = image.mirrored();
  if(!mimage.save(fName, "PNG")) { return; }
  
  dt = QDateTime::currentDateTime();
  QFile file(dName);
  if  (file.open(QIODevice::WriteOnly))
  {
    QTextStream stream(&file);
    stream << m_currentEvent << "\n";
    stream << dt.toString("ddd MMM d hh:mm:ss.zzz yyyy") << "\n";
    stream << text << "\n";
    file.close();
  }
}

void
ISpy3DView::viewPlaneX(void)
{
  if (isAnimating())
    stopAnimating();
  SoCamera * const camera = this->getCamera();
  if (!camera) return; // probably a scene-less viewer

  camera->position = SbVec3f(-1,0,0) * camera->position.getValue().length();
  camera->orientation = SbRotation(SbVec3f(0,1,0),-M_PI/2.f);
}

void
ISpy3DView::viewPlaneY(void)
{
  if (isAnimating())
    stopAnimating();
  SoCamera * const camera = this->getCamera();
  if (!camera) return; // probably a scene-less viewer

  SbVec3f norient = SbVec3f(0,-1,0);
  camera->position = -norient * camera->position.getValue().length();
  camera->orientation = SbRotation(SbVec3f(0,0,1),-M_PI/2.f) *
                        SbRotation(SbVec3f(0,0,-1),norient);
}

void
ISpy3DView::viewPlaneZ(void)
{
  if (isAnimating())
    stopAnimating();
  SoCamera * const camera = this->getCamera();
  if (!camera) return; // probably a scene-less viewer

  camera->position = SbVec3f(0,0,1) * camera->position.getValue().length();
  camera->orientation = SbRotation::identity();
}

void
ISpy3DView::setWhatsThisPicking(bool enable /* = true */)
{
  if (m_whatsThisPicking != enable)
  {
    m_whatsThisPicking = enable;
    if (enable)
    {
      m_oldView = isViewing();
      m_oldSeek = isSeekMode();
      pick();
      getGLWidget()->setCursor(QCursor(Qt::WhatsThisCursor));
    }
    else
    {
      if (m_oldView)
      {
        view();
        if (m_oldSeek)
          seek();
      }
      else
      {
        pick();
        setComponentCursor(SoQtCursor(SoQtCursor::DEFAULT));
      }
    }
  }
}

/** Sets the view mode for this view. Contrary to what we had before, there is
    only one view (was browser) which display models and its behavior is 
    controlled by the viewMode. While this is a quick solution to avoid having 
    R-Phi and R-Z view rotating it will also simplify, in the future, the 
    ability to construct views with multiple subviews or to have different
    behaviors for the same view according to its state.
    E.g. a 3D view could be put in "panning mode" by clicking on some button
    and until the "3D" mode is not clicked again, it will pan on mouse drag,
    rather than rotating.
    
    See also ISpy3DView::eventCallback documentation.
  */
void
ISpy3DView::setViewMode(enum Mode mode)
{
  m_viewMode = mode;
}

/** Overrides SoQtExaminerViewr event Callback which is used to handle user 
    interaction. 
    Handling of events actually depends on the view mode we are currently in
    and those depend, for the moment, on the kind of views we are displaying.
    For 3D views we rotate on mouse drags, for 2D views we pan and in case 
    the viewMode is actually LOCKED, we simply ignore user interaction with 
    the view. 
  */
SbBool
ISpy3DView::eventCallback(void *closure, QEvent *event)
{
  ISpy3DView *self = static_cast<ISpy3DView *>(closure);

  if (QMouseEvent *mEvent = dynamic_cast<QMouseEvent *>(event))
  {
	  // Block popping up a non-functional GUI on right mouse click
	  if (mEvent->button() == Qt::RightButton)
	  {
		  return true;
	  }
	  // Pick if in pick mode: will take everything and eliminate any SoAnnotations on top
	  else if (self->m_interact && mEvent->button() != Qt::NoButton) // take any button
	  {
		  SoSelection *sel = dynamic_cast<SoSelection*>(self->getSceneGraph());
		  sel->deselectAll(); // can later add shift select to add to selection instead
		  bool picked = false;
		  SoSearchAction sa;
		  sa.setType(SoAnnotation::getClassTypeId());
		  SoRayPickAction rp( self->getViewportRegion() );
		  rp.setPickAll(true); 
		  SoQtMouse m;
		  m.setWindowSize(self->getSize());
		  rp.setPoint(m.translateEvent(mEvent)->getPosition());
		  rp.apply(self->getSceneGraph());
		  self->m_interact = false;
		  self->setComponentCursor(SoQtCursor(SoQtCursor::DEFAULT));
		  qDebug() << "Pick!...";
		  SoPickedPointList ppl(rp.getPickedPointList());
		  int list=0;
		  while (list < ppl.getLength() && !picked )
		  {
			  SoPath *path = ppl[list]->getPath();
			  sa.apply(path);
//			  if (sa.getPath() == NULL) // not an annotation
//			  {
				  picked = true;
				  for (int n=0; n < path->getLength(); ++n)
				  {
					  // expecting as names(from node 0 down): ISPY_SCENE_GRAPH_V.. (an SoSelection), CollectionName (an SoSwitch)
					  // bottom node should be MarkerSet, IndexedLineSet, NurbsCurve, LineSet, etc, with a Separator above
					  SoNode *node = path->getNode(n);
					  SbName name(node->getName());
					  uint32_t nid = node->getNodeId();
					  std::string s(name.getString());
					  SoType type = node->getTypeId();
					  SbName typen(type.getName());
					  std::string sn(typen.getString());
					  qDebug() << n << "  node id: " << nid << ", node type: " << sn.c_str() << ", node name: " << s.c_str();
				  }
				  // Now pull out the pieces
				  SoNode *collectionNode = path->getNode(2); // Collection Name
				  SoNode *collectionItem = path->getNode(4); // Item
				  // check for a set
				  // only concerned with SoMarkerSet and SoLineSet. In both cases, want the index
				  int detailIndex = -1;
				  const SoDetail *detail = ppl[list]->getDetail();
				  if (detail)
				  {
					  const SoPointDetail *pd = dynamic_cast<const SoPointDetail*>(detail);
					  if (pd)
					  {
						  detailIndex = pd->getCoordinateIndex();
					  }
					  const SoLineDetail *ld = dynamic_cast<const SoLineDetail*>(detail);
					  if (ld)
					  {
						  detailIndex = ld->getLineIndex();
					  }
				  }
				  if (detailIndex > -1) qDebug() << "  Picked part: " << detailIndex;
				  sel->select(ppl[list]->getPath());
			  }
			  ++list;
//		  }
		  if (picked) self->getSceneManager()->scheduleRedraw();
		  return false;
	  }  
  }
  // Toggle the interaction mode on keyboard P (pick). Seek (S) is built into SoQtViewer.
  if (QKeyEvent *mEvent = dynamic_cast<QKeyEvent *>(event))
  {
	  if (mEvent->key() == Qt::Key_P && mEvent->type() == QEvent::KeyRelease)
	  {
		  self->m_interact = ! self->m_interact;
		  if (self->m_interact)
		  {
			  self->setComponentCursor(SoQtCursor(SoQtCursor::CROSSHAIR));
		  }
		  else
		  {  
			  self->setComponentCursor(SoQtCursor(SoQtCursor::DEFAULT));
		  }
	  }
  }
  switch (self->m_viewMode)
  {
    case PAN_MODE:
      return eventCallbackPanMode(closure, event);
    case ROTATION_MODE:
      return eventCallbackRotationMode(closure, event);
    //TODO: add a "ZOOM_MODE" which basically mimics mouse wheel and allows 
    //      you to zoom in and out, without the usage of the mouse wheel.
    case LOCKED:
      return false;
  }
  return false;
}

/** Callback that rotates the camera position following a mouse drag */
SbBool
ISpy3DView::eventCallbackRotationMode(void *closure, QEvent *event)
{
  ISpy3DView *self = static_cast<ISpy3DView *>(closure);
  if (!self->getParentWidget()->testAttribute(Qt::WA_UnderMouse) ||
      !self->isViewing())
    return false;

  if (QWheelEvent *wheelEvent = dynamic_cast<QWheelEvent *>(event))
  {
    self->zoom(0.1 * (wheelEvent->delta() > 0 ? 1:-1));
    return true;
  }
  else if (QHoverEvent *hEvent = dynamic_cast<QHoverEvent *>(event))
  {
    if (hEvent->type() == QEvent::HoverEnter)
    {
      self->setComponentCursor(SoQtCursor::getRotateCursor());      
      return true;
    }
    return false;
  }
  else if (QMouseEvent *mEvent = dynamic_cast<QMouseEvent *>(event))
  {
    if (mEvent->button() == Qt::LeftButton
        &&  mEvent->type() == QEvent::MouseButtonRelease
        && self->isSeekMode())
    {
      self->setComponentCursor(SoQtCursor::getRotateCursor());
      return true;
    }
    return false;
  }
  return false;  
}

/** Callback that pans the camera position following a mouse drag.
    Notice that we always return true on mouse drags, because we don't want 
    the SoQtExaminerViewer to handle them, as it would interpret those
    as request to rotate.
 */
SbBool
ISpy3DView::eventCallbackPanMode(void *closure, QEvent *event)
{
  static bool firstEvent = true;    
  static float oldX = 0.;
  static float oldY = 0.;
  ISpy3DView *self = static_cast<ISpy3DView *>(closure);
  if (!self->getParentWidget()->testAttribute(Qt::WA_UnderMouse) ||
      !self->isViewing())
    return false;
  
  if (QWheelEvent *wheelEvent = dynamic_cast<QWheelEvent *>(event))
  {
    self->zoom(0.1 * (wheelEvent->delta() > 0 ? 1:-1));
    return true;
  }
  else if (QHoverEvent *hEvent = dynamic_cast<QHoverEvent *>(event))
  {
    if (hEvent->type() == QEvent::HoverEnter)
    {
      self->setComponentCursor(SoQtCursor::getPanCursor());      
      return true;
    }
    return false;
  }
  else if (QMouseEvent *mEvent = dynamic_cast<QMouseEvent *>(event))
  {
        
    if (!(mEvent->buttons() & Qt::LeftButton))
      return false;

    if (mEvent->type() == QEvent::MouseButtonPress)
    {
      self->setComponentCursor(SoQtCursor::getPanCursor());
      firstEvent = false;
      oldX = mEvent->x();
      oldY = mEvent->y();
      return true;
    }
    else if (mEvent->type() == QEvent::MouseButtonRelease)
    {
      firstEvent = true;
      return true;
    }
    else if (mEvent->type() == QEvent::MouseMove)
    {
      float deltaX = mEvent->x() - oldX;
      float deltaY = oldY - mEvent->y();
      SoCamera *camera = self->getCamera();

      // We need to adjust the amount we translate of to the actual
      // zoom factor.
      // FIXME: do the same thing for perspective views.
      float scaleFactor = 100;
      
      if (camera->isOfType(SoOrthographicCamera::getClassTypeId()))
      {
        SoOrthographicCamera *oc = (SoOrthographicCamera *) camera;
        scaleFactor *= 1 / oc->height.getValue();
      }

      // We need to pan according to the camera orientation, not for absolute 
      // X and Y axis!
      SbVec3f oldPosition = camera->position.getValue();
      SbRotation cameraOrientation = camera->orientation.getValue();
      SbVec3f newPosition;
      cameraOrientation.multVec(SbVec3f(deltaX/scaleFactor, 
                                        deltaY/scaleFactor, 
                                        0),
                                 newPosition);
      camera->position = oldPosition - newPosition;
      oldX = mEvent->x();
      oldY = mEvent->y();
      return true;
    }
  }
  return false;
}
