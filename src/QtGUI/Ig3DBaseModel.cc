//<<<<<< INCLUDES                                                       >>>>>>

#include "QtGUI/Ig3DBaseModel.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/SoPath.h>
#include <cassert>
#include <climits>
#include <QDebug>

Ig3DBaseModel::Ig3DBaseModel(void)
  : m_sceneGraph(new SoSelection),
    m_contents(0),
    m_selection(0)
{
  // assert("attachPoint is the scene graph or within it");
  m_sceneGraph->ref();
  m_sceneGraph->setName("ISPY_SCENE_GRAPH_V1");
  initScene(m_sceneGraph);
}

void debugPath(SoPath* path) {
	qDebug("Nodes count: %d", path->getLength());
	for (int i = 0; i < path->getLength(); i++) {
		qDebug() << i << " = " << path->getNode(i)->getName()
				 << " type: "  << path->getNode(i)->getTypeId().getName();
	}
}

void selectionCallback(void * obj, SoPath * path) {
	Ig3DBaseModel* model = (Ig3DBaseModel*) obj;
	SoNode* triggeredNode = path->getNodeFromTail(1);

	if (triggeredNode) {
		model->updateSelection(triggeredNode);
		triggeredNode->touch(); // redraw
	}

	debugPath(path);
}

void deselectionCallback(void * obj, SoPath * path) {
	SoNode* triggeredNode = path->getNodeFromTail(1);

	if (triggeredNode) {
		triggeredNode->touch(); // redraw
	}
}

void Ig3DBaseModel::updateSelection(SoNode* node) {
	if (node) {
		emit elementSelected(QString(node->getName().getString()));

		qDebug() << "Selected node: " << node->getName()
						 << " type: "  << node->getTypeId().getName();
	}
}

void
Ig3DBaseModel::initScene(SoGroup *top)
{
  SoSelection *root = dynamic_cast <SoSelection *>(top);
  // FIXME: root->addChild(new SoPerspectiveCamera);

  SoSeparator *sel = new SoSeparator;
  sel->setName(SbName("CollectionSelection"));
  SoDrawStyle *sty = new SoDrawStyle;
  sty->style = SoDrawStyle::LINES;
  sty->lineWidth = 3;
  sel->addChild(sty);

  SoMaterial *mat = new SoMaterial;
  mat->diffuseColor = SbColor(0xC0/255., 0x00/255., 0x00/255.);
  sel->addChild(mat);

  m_selection = new SoSeparator;
  sel->addChild(m_selection);
  root->addChild(sel);

  m_contents = new SoSeparator;
  root->addChild(m_contents);

	root->addSelectionCallback(selectionCallback, this);
	root->addDeselectionCallback(deselectionCallback, this);
}

void Ig3DBaseModel::update() {
	m_selection->touch(); // to redraw
}

Ig3DBaseModel::~Ig3DBaseModel(void)
{ m_sceneGraph->unref(); }

SoGroup *
Ig3DBaseModel::sceneGraph(void) const
{ return m_sceneGraph; }

SoGroup *
Ig3DBaseModel::contents(void) const
{ return m_contents; }

SoGroup *
Ig3DBaseModel::selection(void) const
{ return m_selection; }

SbString
Ig3DBaseModel::encode(const std::string &name)
{
  // Encode names so that they use valid OpenInventor characters and
  // thre rest is escaped as `_X<hex code>', and decode them when
  // reading in the files.
  static const char   hexdigits []     = "0123456789abcdef";
  std::string         result;

  if (! SbName::isBaseNameStartChar(name [0]))
  {
    result += '_';
  }

  for (std::string::size_type i = 0; i < name.size(); ++i)
    if (SbName::isBaseNameChar(name[i]))
      result.append(1, name[i]);
    else if (i > 0 && SbName::isBaseNameStartChar(name[i]))
      result.append(1, name[i]);
    else
    {
#           if UCHAR_MAX != 255 || CHAR_BIT != 8
#             error expected 8-bit characters
#           endif
      result += '_';
      result += 'X';
      result += hexdigits [((unsigned char) name[i]>>4)&0xf];
      result += hexdigits [(unsigned char) name[i]&0xf];
    }

  return result.c_str();
}

/** Sets the camera to be used for the main scene. In case no previos camera is
    found, the camera gets added as first node of the scenegraph, otherwise 
    it replaces the one that is already there.
    
    @the camera
    
    the camera to be used.
 */
void
Ig3DBaseModel::setCamera(SoCamera *camera)
{
  SoSearchAction *cameraAction = new SoSearchAction;
  cameraAction->setType(SoCamera::getClassTypeId(), true);
  cameraAction->setInterest(SoSearchAction::FIRST);
  cameraAction->apply(m_sceneGraph);
  if (!cameraAction->isFound())
  {
    m_sceneGraph->insertChild(camera, 0);
    return;
  }
  SoPath *path = cameraAction->getPath();
  SoCamera *currentCamera = dynamic_cast<SoCamera *>(path->getTail());
  assert(currentCamera);
  if (camera != currentCamera)
  {
    SoGroup *group = dynamic_cast<SoGroup*>(path->getNodeFromTail(1));
    assert(group);
    group->replaceChild(currentCamera, camera);
  }
  delete cameraAction;
}

std::string
Ig3DBaseModel::decode(const std::string &name)
{
  // Encode names so that they use valid OpenInventor characters and
  // thre rest is escaped as `_X<hex code>', and decode them when
  // reading in the files.
  static const char   hexdigits []     = "0123456789abcdef";
  std::string         result;
  const char          *first;
  const char          *second;

  for (std::string::size_type i = 0; i < name.size(); ++i)
    if (name[i] != '_'
        || i > name.size() - 4
        || name[i+1] != 'X'
        || !(first = strchr(hexdigits, name[i+2]))
        || !(second = strchr(hexdigits, name[i+3])))
      result.append(1, name[i]);
    else
    {
#           if UCHAR_MAX != 255 || CHAR_BIT != 8
#             error expected 8-bit characters
#           endif
      unsigned int code = ((unsigned)(first - hexdigits) << 4)
                         + ((unsigned)(second - hexdigits));
      assert(code <= UCHAR_MAX);
      result += static_cast<char>(code);
      i += 3;
    }

  return result;
}

std::string
Ig3DBaseModel::decode(const SbName &name)
{ return decode(std::string(name.getString())); }
