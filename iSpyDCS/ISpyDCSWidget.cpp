#include "ISpyDCSWidget.h"
#include "SleepThread.h"
#include "QtGUI/Ig3DBaseModel.h"
#include "QtGUI/ISpy3DView.h"
#include "QtGUI/ISpyEventFilter.h"
#include "QtGUI/ISpyMainWindow.h"
#include "QtGUI/ISpySplashScreen.h"

#include "QtGUI/IgCollectionTableModel.h"
#include "Framework/IgCollection.h"

#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/SbColor.h>
#include <Inventor/engines/SoInterpolateVec3f.h>
#include <Inventor/engines/SoInterpolateRotation.h>

#include <QDebug>
#include <QPushButton>
#include <QUrl>
#include <QMouseEvent>
#include <QAction>
#include <QVariant>
#include <QMessageBox>

#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <cassert>
#include <exception>

ISpyDCSWidget::ISpyDCSWidget(QWidget *parent): QWidget(parent) {
	SoQt::init(this);
	m_viewer = NULL;
	m_splash = NULL;
	m_mainWindow = NULL;
	m_3DModel = NULL;

	m_showTableWidget = false;
	m_showTreeWidget = false;
	m_showMenu = false;
	m_showToolbar = false;

	layout = new QGridLayout;
	setLayout(layout);

	doRun();
	layout->addWidget(m_mainWindow);
	m_mainWindow->setParent(this);

	resize(640, 480);
}

ISpyDCSWidget::~ISpyDCSWidget() {
	if (m_viewer) {
		delete m_viewer;
		m_viewer = NULL;
	}
	
	if (m_splash) {
		delete m_splash;
		m_splash = NULL;
	}

	if (m_listModel) {
		delete m_listModel;
		m_listModel = NULL;
	}

	if (m_tableModel) {
		delete m_tableModel;
		m_tableModel = NULL;
	}

	if (m_3DModel) {
		m_3DModel->sceneGraph()->removeAllChildren();
		delete m_3DModel;
		m_3DModel = NULL;
	}

	m_events.clear();
	m_collections.clear();
	m_specs.clear();
	m_views.clear();
	m_cameras.clear();
	m_cameraSpecs.clear();
  
	for (int i = 0; i < 2; ++i) {
		if (m_storages[i]) {
			delete m_storages[i];
		}
	}

	if (m_archives[0] != m_archives[1] && m_archives[1]) {
		delete m_archives[1];
	}

	if (m_archives[0]) {
		delete m_archives[0];
	}
}

Ig3DBaseModel* ISpyDCSWidget::getModel() {
	return m_3DModel;
}

int ISpyDCSWidget::doRun(void) {
  bool ok = parseViewsDefinitionFile(":/views/default-views.iml");
  assert(ok && "Default views are broken!!!");	
  
  ISpyEventFilter filter;
  QEventLoop evloop;

  setupMainWindow();

  restoreSettings();
  initHelpers();

  m_3DModel = new Ig3DBaseModel;
  m_viewer = new ISpy3DView(m_3DModel, m_mainWindow->workspace());
  m_viewer->setFeedbackVisibility(true);
  setupActions();
  m_mainWindow->addToolBar(Qt::TopToolBarArea, m_3DToolBar);

  QObject::connect(m_mainWindow, SIGNAL(save()), m_viewer, SLOT(save()));
  QObject::connect(m_mainWindow, SIGNAL(print()), m_viewer, SLOT(print()));
  QObject::connect(m_viewer, SIGNAL(cameraToggled()), 
                   (QWidget*)this, SLOT(cameraToggled()));
  QObject::connect(m_mainWindow->actionQuit, SIGNAL(triggered()), (QWidget*)this, SLOT(onExit()));
  QObject::connect((QWidget*)this, SIGNAL(showMessage(const QString &)), 
                   m_mainWindow->statusBar(), 
                   SLOT(showMessage(const QString &)));
  
  QObject::connect(&filter, SIGNAL(open(const QString &)),
                   (QWidget*)this, SLOT(openWithFallbackGeometry(const QString &)));
  
	showTreeWidget(false);
	showTableWidget(false);
	showMenu(false);
	showToolbar(false);
	m_mainWindow->toolBarEvent->hide();

  // Process pending events now to allow file open events through
  //(mac), calling "open()" on the files. Exclude user input and
  // don't wait for events to occur.
  evloop.processEvents(QEventLoop::ExcludeUserInputEvents);

  // If we don't have any files open, show the splash screen as a file
  // opening wizard. The file open dialog will eventually show the main
  // window. If we opened files above just make the "About iSpy" splash
  // screen visible for a few seconds, show the main window and proceed
  // immediatly to the main app event loop.
  m_splash = new ISpySplashScreen;
  m_splash->initActions(this);
  QObject::connect(m_mainWindow->actionOpenWizard, SIGNAL(triggered()),
                   m_splash, SLOT(showWizard()));

  // We look up for changes in the CSS file only at the end, so that 
  // we are sure that there is actually something to update.
  // Notice that I need to convert the relative path to an absolute one,
  // otherwise the filewatcher gets confused.
  
  if (!m_cssFilename.empty())
  {
    QFileInfo cssFilename(m_cssFilename.c_str());
    m_cssFilename = cssFilename.absoluteFilePath().toStdString();
    openCss(m_cssFilename.c_str());
    QFileInfo info(m_cssFilename.c_str());
    m_fileWatcher->addPath(m_cssFilename.c_str());
    m_fileWatcher->addPath(info.dir().absolutePath());
    QObject::connect(m_fileWatcher, SIGNAL(fileChanged(const QString &)),
                     (QWidget*)this, SLOT(openCss(const QString &)));
    QObject::connect(m_fileWatcher, SIGNAL(directoryChanged(const QString &)),
                     (QWidget*)this, SLOT(cssDirChanged(const QString &)));
  }

  return 0;
}

void ISpyDCSWidget::updateCollections(void) {
	// Create a few dynamic collections which contain stats about the rest of
  // the IgDataStorage. This is useful to create summary tables etc.
  createStats();

  // Change the background to match the current style.
  // This will allow to have different backgrounds for different
  // events, if required.
  SbColor bgColor = m_styles[findStyle("Background")].material->diffuseColor[0];
  m_viewer->setBackgroundColor(bgColor);

  // Remember currently selected collection.
  QSettings settings;
  std::string selectedCollection;
  QTreeWidgetItem *current = m_treeWidget->currentItem();
  int currentIndex = getCollectionIndex(current);

  if (currentIndex != -1) 
    selectedCollection = m_collections[currentIndex].collectionName;
  
  // Clear tree and table views. We clear 3D view at the end.
  if (m_tableModel)
    m_tableModel->clear();
  m_treeWidget->clear();

  // Rebuild the collections. Loop over collections in the order they
  // appear in the input storages, event first, geometry after.
  // Record these in our collections list for later reference.
  Collections oldcollections(m_storages[0]->collectionNames().size() +
                             m_storages[1]->collectionNames().size());
  Groups oldgroups;

  m_groupIndex.clear();
  m_groups.swap(oldgroups);
  m_collections.swap(oldcollections);
  View &view = m_views[m_currentViewIndex];

  // Add a name of the view to the meta-data of
  // an automatically printed image
  m_metaData.clear();

  // crashes calling QString::fromStdString with some Qt versions
  //m_metaData.append("View: ").append(QString::fromStdString(view.spec->name));
  if (m_views.size() == 0) return;
  m_metaData.append("View: ").append(QString(view.spec->name.c_str()));

  for (size_t sti = 0, ste = 2, i = 0; sti < ste; ++sti)
  {
    // Try to locate a matching collection spec.  Accept the spec only
    // if it qualifies all stated requirements.  If it matches, use
    // the "friendly" collection name from the spec and its drawing
    // utility.
    std::vector<std::string> &names = m_storages[sti]->collectionNames();
    for (size_t ci = 0, ce = names.size(); ci != ce; ++ci, ++i)
    {
      std::string       collectionName = names[ci];
      std::string       name = collectionName;
      IgCollection      *coll = m_storages[sti]->getCollectionPtr(ci);
      IgCollection      *other = 0;
      IgAssociations  *assoc = 0;
      CollectionSpec    *spec = 0;

      // Get the current view and iterate on all the specs found there.
      // FIXME: pick up something different from the first one.
      assert(m_currentViewIndex < m_views.size());

      for (size_t spi = view.spec->startCollIndex, spe = view.spec->endCollIndex; 
           spi != spe && !spec;
           ++spi)
      {
        CollectionSpec &cand = m_specs[spi];
        if (cand.collection.empty() || cand.collection == name)
        {
          bool hasFields = true;
          bool hasOther = true;
          bool hasOtherFields = true;
          bool hasAssoc = true;

          for (size_t fi = 0, fe = cand.requiredFields.size(); fi != fe && hasFields; ++fi)
            hasFields = coll->hasProperty(cand.requiredFields[fi]);

          if (! cand.otherCollection.empty())
          {
            if (!(other = m_storages[sti]->getCollectionPtr(cand.otherCollection)))
              hasOther = hasOtherFields = false;

            for (size_t fi = 0, fe = cand.otherRequiredFields.size(); fi != fe && hasOtherFields; ++fi)
              hasOtherFields = other->hasProperty(cand.otherRequiredFields[fi]);
          }

          if (! cand.otherAssociation.empty())
          {
            if (!(assoc = m_storages[sti]->getAssociationsPtr(cand.otherAssociation)))
              hasAssoc = false;
          }

          if (hasFields && hasOther && hasOtherFields && hasAssoc)
          {
            if (! cand.friendlyName.empty())
              name = cand.friendlyName;
            spec = &cand;
          }
        }
      }
      
      // Handles the collections groups:
      // * Determine the group name from the spec name.
      // * Lookup the groups directory by name to see if the group is already 
      //   there, create it otherwise.
      // * Save the group index to store it in the Collection.
      // Actual addition of the children to a group is done later on, so that
      // we can make sure that the group ordering is the same as the 
      // one in which the collections where declared.
      //
      // Remember that divPos + 1 is always 0 because std::string::npos := 1.
      size_t divPos = name.find_first_of('/');
      std::string groupName;
      if (!divPos || divPos == std::string::npos)
        groupName = "Other";
      else
        groupName = std::string(name, 0, divPos);

      name = std::string(name, divPos + 1);
      
      int groupIdx = -1;
      for (size_t gi = 0, ge = m_groups.size(); gi != ge; gi++)
        if(m_groups[gi].name == groupName)
        {
          groupIdx = gi;
          break;
        }
        
      if (groupIdx < 0)
      {
        groupIdx = m_groups.size();
        m_groups.resize(m_groups.size() + 1);
        Group &group = m_groups.back();
        group.name = groupName;
        group.item = new QTreeWidgetItem;
        group.item->setFont(0, *m_groupFont);
        group.item->setText(0, groupName.c_str());
        group.item->setTextColor(0, QColor(93, 110, 129));
        group.item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        // Loop over the old groups and bring over the "expanded" property.
        bool expanded = true;
        for (size_t ogi = 0, oge = oldgroups.size(); ogi != oge; ogi++)
          if (oldgroups[ogi].name == groupName)
          {
            expanded = oldgroups[ogi].expanded;
            break;
          }
        group.expanded = expanded;
      }
      
      
      // Create new items in the tree view and a placeholder content
      // node in the 3D model. The latter will be filled in on first
      // display, and directly here if the visibility is on.
      // 
      // Notice taht tree items will be added to the widget only 
      // later on, once we have sorted the collections by the
      // associated specs.
      QTreeWidgetItem *item = new QTreeWidgetItem;
      item->setFont(0, *m_itemFont);
      item->setFont(1, *m_itemFont);
      item->setText(0, name.c_str());
      item->setTextAlignment(1, Qt::AlignRight|Qt::AlignVCenter);
      item->setTextAlignment(2, Qt::AlignCenter);
      item->setText(1, QString("%1").arg(coll->size()));
      
      SoSwitch *sw = new SoSwitch;
      SoSeparator *sep = new SoSeparator;
			sep->setName(SbName((std::string("Sep") + collectionName).c_str()));
      sw->setName(SbName(collectionName.c_str()));
      sw->addChild(sep);
      
      // If this is geometry, and we had the same geometry, transfer
      // it over to the new collection to avoid recomputing it, but
      // just in the case in the new view the make3D method is the same.
      // Note that reading files in deliberately clears the .sep fields so
      // we won't be fooled here to think a new collection is the same
      // as the old one just because it has the same pointer.
      
      for (size_t oi = 0, oe = oldcollections.size(); oi != oe; ++oi)
      {
        if (sti == 1)
          if ((oldcollections[oi].data[0] == coll && oldcollections[oi].sep)
              && (spec 
                  && oldcollections[oi].spec 
                  && oldcollections[oi].spec->make3D == spec->make3D))
            for (int zi = 0, ze = oldcollections[oi].sep->getNumChildren(); zi != ze; ++zi)
              sep->addChild(oldcollections[oi].sep->getChild(zi));
      }
      
      // Fill in the collection item. This needs to be in place before
      // we modify the tree as our slots on tree notifications use it.
      // We also determine at this point which style needs to be used by 
      // to render the collection.
      m_collections[i].spec = spec;
      m_collections[i].data[0] = coll;
      m_collections[i].data[1] = other;
      m_collections[i].assoc = assoc;
      m_collections[i].item = item;
      m_collections[i].node = sw;
      m_collections[i].sep = sep;
      m_collections[i].collectionName = collectionName;
      m_collections[i].groupIndex = groupIdx;
      m_collections[i].style = findStyle(collectionName.c_str());
    }
  }  
  
  // Sort the collections by spec so that they are displayed in the 
  // tree according to the order of the various "collection" calls
  // rather than in the one they happen to be in the file.
  std::sort(m_collections.begin(), m_collections.end(), SortBySpecAndName());

  std::set<size_t> createdGroups;
  m_groupIndex.clear();
  for (size_t i = 0, e = m_collections.size(); i != e; ++i)
  {
    Collection &coll = m_collections[i];
    if (!coll.spec && view.spec->specialized)
      continue;
    // Get the associated group, check if it is already in the tree widget 
    // (create and add it eventually, with correct expand settings).
    // Add the children to the group.
    Group &group = m_groups[coll.groupIndex];
    
    if (createdGroups.find(coll.groupIndex) == createdGroups.end())
    {
      createdGroups.insert(coll.groupIndex);
      m_groupIndex.push_back(coll.groupIndex);
      m_treeWidget->addTopLevelItem(group.item);
      m_treeWidget->setItemExpanded(group.item, group.expanded);
    }
    
    group.children.push_back(i);
    group.item->addChild(coll.item);
    
    // Finish setting up the tree items and append them to the tree widget.
    bool visibility = m_visibility[coll.spec->visibilityIndex];
    
    if (coll.spec && coll.spec->make3D)
      coll.item->setCheckState(2, visibility ? Qt::Checked : Qt::Unchecked);
    else
      coll.item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    
    // Set current item. Updates table and 3D views too.
    // If this is not current and visible, show in 3D.
    if (! selectedCollection.empty() && selectedCollection == coll.collectionName)
    {
      // FIXME: this is required because otherwise setCurretItem() forces the
      //        group to be expanded on next event, even if it is not.
      //        I could not find any better solution.
      if (m_groups[coll.groupIndex].expanded)
        m_treeWidget->setCurrentItem(coll.item);
    }
    else
      displayCollection(coll);
  }

  // Looks like because we called "clear" we need to reset all the 
  // tree properties to have the QTreeWidget behave correctly.
  m_treeWidget->header()->setMinimumSectionSize(33);
  m_treeWidget->header()->setDefaultSectionSize(33);
  m_treeWidget->header()->setStretchLastSection(false);
  m_treeWidget->header()->setCascadingSectionResizes(true);
  m_treeWidget->header()->setResizeMode(0, QHeaderView::Stretch);
  m_treeWidget->header()->setResizeMode(1, QHeaderView::ResizeToContents);
  m_treeWidget->header()->setResizeMode(2, QHeaderView::Fixed);
  
  // Clear and re-fill the 3D now that we don't need old data.
  // Update the camera.
  m_3DModel->contents()->removeAllChildren();
  if (view.camera->node)
  {
    m_viewer->setCamera(view.camera->node);
    if (view.camera->spec && view.camera->spec->rotating)
    {
      m_viewer->setViewMode(ISpy3DView::ROTATION_MODE);
      m_viewPlaneGroup->setEnabled(true);
      m_viewModeGroup->setEnabled(true);
    }
    else
    {
      m_viewer->setViewMode(ISpy3DView::PAN_MODE);
      m_viewPlaneGroup->setEnabled(false);
      m_viewModeGroup->setEnabled(false);
    }
    
    if (view.camera->node->getTypeId() == SoPerspectiveCamera::getClassTypeId())
      m_actionCameraPerspective->setChecked(true);
    else
      m_actionCameraOrthographic->setChecked(true);
  }
  for (size_t i = 0, e = m_collections.size(); i != e; ++i)
    m_3DModel->contents()->addChild(m_collections[i].node);

  // Finally adjust buttons to what can be done here.
  m_mainWindow->actionNext->setEnabled(! m_events.empty() && m_eventIndex < m_events.size()-1);
  m_mainWindow->actionPrevious->setEnabled(m_eventIndex > 0);
  
  if (m_eventIndex >= (m_events.empty() ? 0 : m_events.size()-1))
  {
    m_mainWindow->showNormal();
		m_mainWindow->dockTreeWidget->setShown(m_showTreeWidget);
    m_mainWindow->dockTableWidget->setShown(m_showTableWidget);

		if (m_showMenu)
			m_mainWindow->menubar->show();
		else
			m_mainWindow->menubar->hide();
    
		if (m_showToolbar)
			m_3DToolBar->show();
		else
			m_3DToolBar->hide();
  }

	m_mainWindow->show();
}
	
void ISpyDCSWidget::loadModel(const QString& file) {
	if (!file.isEmpty()) {
		simpleOpen(file);
	}
}

void ISpyDCSWidget::loadLayout(const QString& file) {
	if (!file.isEmpty()) {
		m_viewSpecs.clear();
		m_specs.clear();
		m_cameraSpecs.clear();

		qDebug() << "Reading " << file;
		parseViewsDefinitionFile(file.toAscii().data());
	}
}

void ISpyDCSWidget::loadStyles(const QString& file) {
	if (!file.isEmpty()) {
		QFileInfo cssFilename(file);
		std::string resCssFilename = cssFilename.absoluteFilePath().toStdString();
		openCss(resCssFilename.c_str());
	}
}

bool ISpyDCSWidget::isValidColor(uint r, uint g, uint b) {
	if (r > 255 || g > 255 || b > 255) {		
		QMessageBox::warning(m_mainWindow, "Warning", 
			QString().sprintf(
				"Invalid color: (%d, %d, %d). (R, G, B) values must be in range [0, 255].", 
				r, g, b));

		return false;
	}

	return true;
}

void ISpyDCSWidget::setColor(const QString& name, uint r, uint g, uint b) {
	if (!m_mainWindow || !m_viewer || !isValidColor(r, g, b) ) { 
			return;
	}

	SoNode* node = m_viewer->findGroup(name.toUtf8().data());

	if (!node) {
		QMessageBox::warning(m_mainWindow, "Warning", 
			QString().sprintf("Element %s does not exist.",	name.toUtf8().data()));

		return;
	}

	if (node->getChildren()->getLength() > 0) {
		SoChildList* childs = node->getChildren();
		SoNode* child = (SoNode*)childs->get(0);
			
		if (child->getTypeId() == SoMaterial::getClassTypeId()) {
			SoMaterial* material = (SoMaterial*)child;
			material->diffuseColor = SbColor(r/255.0, g/255.0, b/255.0);			
		}

		for (int i=0; i<childs->getLength(); i++) {
			child = (SoNode*)childs->get(i);

			if (child->getTypeId() == SoSeparator::getClassTypeId()) {
				SoSeparator* sep = (SoSeparator*)child;
				setColor(QString(sep->getName().getString()), r, g, b);
			}
		}
	}
}

void ISpyDCSWidget::setTransparency(const QString& name, float val) {
	if (!m_mainWindow || !m_viewer) { 
			return;
	}

	if (val < 0 || val > 1) {
		QMessageBox::warning(m_mainWindow, "Warning", 
			QString().sprintf("Transparency value %f is out of range [0.0, 1.0].", val));

		return;
	}

	SoNode* node = m_viewer->findGroup(name.toUtf8().data());

	if (!node) {
		QMessageBox::warning(m_mainWindow, "Warning", 
			QString().sprintf("Element %s does not exist.",	name.toUtf8().data()));

		return;
	}

	if (node->getChildren()->getLength() > 0) {
		SoChildList* childs = node->getChildren();
		SoNode* child = (SoNode*)childs->get(0);
			
		if (child->getTypeId() == SoMaterial::getClassTypeId()) {
			SoMaterial* material = (SoMaterial*)child;
			material->transparency = val;
		}

		for (int i=0; i<childs->getLength(); i++) {
			child = (SoNode*)childs->get(i);

			if (child->getTypeId() == SoSeparator::getClassTypeId()) {
				SoSeparator* sep = (SoSeparator*)child;
				setTransparency(QString(sep->getName().getString()), val);
			}
		}
	}
}

void ISpyDCSWidget::setSolid(const QString& name, bool b) {
	if (!m_mainWindow || !m_viewer) return;
	SoNode* node = m_viewer->findGroup(name.toUtf8().data());

	if (!node) {
		QMessageBox::warning(m_mainWindow, "Warning", 
			QString().sprintf("Element %s does not exist.",	name.toUtf8().data()));

		return;
	}

	if (node && node->getChildren()->getLength() > 0) {
		SoChildList* childs = node->getChildren();
		
		if (childs->getLength() > 1) {
			SoNode* child = (SoNode*)childs->get(1);			
			if (child->getTypeId() == SoDrawStyle::getClassTypeId()) {
				SoDrawStyle* drawStyle = (SoDrawStyle*)child;

				drawStyle->style = b ? SoDrawStyle::FILLED : SoDrawStyle::LINES;
			}
		}

		for (int i=0; i<childs->getLength(); i++) {
			SoNode* child = (SoNode*)childs->get(i);

			if (child->getTypeId() == SoSeparator::getClassTypeId()) {
				SoSeparator* sep = (SoSeparator*)child;
				setSolid(QString(sep->getName().getString()), b);
			}
		}
	}
}

void ISpyDCSWidget::showElement(const QString& name, bool b) {
	if (!m_mainWindow || !m_viewer) return;

	SoNode* node = m_viewer->findGroup(name.toUtf8().data());

	if (!node) {
		QMessageBox::warning(m_mainWindow, "Warning", 
			QString().sprintf("Element %s does not exist.",	name.toUtf8().data()));

		return;
	}

	if (node && node->getChildren()->getLength() > 0) {
		// If it's a switch, then it's a fastest way to show hide group of elements
		if (node->getTypeId() == SoSwitch::getClassTypeId()) {
			SoSwitch* switchNode = (SoSwitch*)node;
			switchNode->whichChild = b ? SO_SWITCH_ALL : SO_SWITCH_NONE;
			return;
		}

		SoChildList* childs = node->getChildren();
		SoNode* child = (SoNode*)childs->get(1);
			
		if (child->getTypeId() == SoDrawStyle::getClassTypeId()) {
			SoDrawStyle* drawStyle = (SoDrawStyle*)child;

			drawStyle->style = b ? SoDrawStyle::FILLED : SoDrawStyle::INVISIBLE;
		}

		for (int i=2; i<childs->getLength(); i++) {
			child = (SoNode*)childs->get(i);

			if (child->getTypeId() == SoSeparator::getClassTypeId()) {
				SoSeparator* sep = (SoSeparator*)child;
				showElement(QString(sep->getName().getString()), b);
			}
		}
	}
}

void ISpyDCSWidget::showTableWidget(bool b) {
	if (m_mainWindow) {
		m_showTableWidget = b;
		m_mainWindow->dockTableWidget->setShown(m_showTableWidget);
	}
}

void ISpyDCSWidget::showTreeWidget(bool b) {
	if (m_mainWindow) {
		m_showTreeWidget = b;
		m_mainWindow->dockTreeWidget->setShown(m_showTreeWidget);
	}
}

void ISpyDCSWidget::showMenu(bool b) {
	if (m_mainWindow) {
		m_showMenu = b;

		if (m_showMenu && m_mainWindow)
			m_mainWindow->menubar->show();
		else
			m_mainWindow->menubar->hide();
	}
}

void ISpyDCSWidget::showToolbar(bool b) {
	if (m_mainWindow) {
		m_showToolbar = b;

		if (m_showToolbar) {
			m_3DToolBar->show();
			actionEnableDisablePicking->setEnabled(true);
		}
		else
			m_3DToolBar->hide();	
	}
}

void ISpyDCSWidget::setupActions(void) {
	ISpyApplication::setupActions();

	actionEnableDisablePicking = new QAction(QObject::parent());
  actionEnableDisablePicking->setObjectName(QString::fromUtf8("actionEnableDisablePicking"));
	actionEnableDisablePicking->setIcon(QApplication::style()->standardIcon(QStyle::SP_CommandLink));
  actionEnableDisablePicking->setText(QApplication::translate("ISpy3DView", "Pick", 0, QApplication::UnicodeUTF8));
	actionEnableDisablePicking->setCheckable(true);
	#ifndef QT_NO_TOOLTIP
	  actionEnableDisablePicking->setToolTip(QApplication::translate("ISpy3DView", "Pick", 0, QApplication::UnicodeUTF8));
	#endif // QT_NO_TOOLTIP

  m_viewPlaneGroup->addAction(actionEnableDisablePicking);

	QObject::connect(actionEnableDisablePicking, SIGNAL(triggered()), 
                   m_viewer, SLOT(enableDisablePicking()));

	m_3DToolBar->addAction(actionEnableDisablePicking);
}

void ISpyDCSWidget::enableDisablePicking() {
	m_viewer->setWhatsThisPicking(true);
}

void ISpyDCSWidget::enableRendering(bool b) {
	if (m_viewer) {
		if (b) m_viewer->getSceneManager()->activate();
		else	 m_viewer->getSceneManager()->deactivate();
	}
}

void ISpyDCSWidget::enableNavigation(bool b) {
	m_viewer->setViewing(b);
	showToolbar(b);
}

void ISpyDCSWidget::showView(int index) {
	if (!m_mainWindow || 
			!m_mainWindow->viewSelector || 
			m_mainWindow->viewSelector->count() == 0 ) 
	{
		return;
	}

	int viewCount = m_mainWindow->viewSelector->count();

	if (m_currentViewIndex != index) {
		if (index < 0 || index >= viewCount) {
			QMessageBox::warning(m_mainWindow, "Warning", 
				QString().sprintf("Failed to switch view. Index [%d] is out of range [%d, %d].",	
				index, 0, viewCount-1));
			
			return;
		}

		m_mainWindow->viewSelector->setCurrentIndex(index);
		m_currentViewIndex = index;

		//m_viewer->viewAll();
		m_viewer->zoom(0.8);
	}
}

QStringList ISpyDCSWidget::getCameraInfo() {
	SoCamera* camera = m_viewer->getCamera();
	QStringList list;

	list.append(QString::number(camera->position.getValue()[0]));
	list.append(QString::number(camera->position.getValue()[1]));
	list.append(QString::number(camera->position.getValue()[2]));

	float radians;
	SbVec3f vector; 
	camera->orientation.getValue().getValue(vector, radians);

	list.append(QString::number(vector[0]));
	list.append(QString::number(vector[1]));
	list.append(QString::number(vector[2]));
	list.append(QString::number(radians));
	
	return list;
}

const int DEFAULT_TRANS_TIME = 1000;
const int DEFAULT_FPS = 30;

void ISpyDCSWidget::checkSmoothMoveParams(int& time, int& fps) {
	if (time <= 0 || time > 60000) {
		QMessageBox::warning(m_mainWindow, "Warning", 
			QString().sprintf("Time param: %d ms out of range. Setting to default: %d ms", 
			time, DEFAULT_TRANS_TIME));

		time = DEFAULT_TRANS_TIME;
	}

	if (fps <= 0 || fps > 30) {
		QMessageBox::warning(m_mainWindow, "Warning", 
			QString().sprintf("FPS param: %d out of range. Setting to default: %d", 
			fps, DEFAULT_FPS));

		fps = DEFAULT_FPS;
	}
}

void ISpyDCSWidget::seekToTargetSmoothly(const SbVec3f& pos, const SbRotation& rot, int time, int fps) {
	SoPerspectiveCamera* cam = (SoPerspectiveCamera*)m_viewer->getCamera();
	SoInterpolateVec3f * interpolatePos = NULL;
	SoInterpolateRotation * interpolateRot = NULL;

	checkSmoothMoveParams(time, fps);

	try {
		interpolatePos = new SoInterpolateVec3f;
		interpolatePos->ref();
		interpolatePos->input0 = cam->position.getValue();
		interpolatePos->input1 = pos;
		cam->position.connectFrom(&interpolatePos->output);

		interpolateRot = new SoInterpolateRotation;
		interpolateRot->ref();
		interpolateRot->input0 = cam->orientation.getValue();
		interpolateRot->input1 = rot;
		cam->orientation.connectFrom(&interpolateRot->output);

		int frames = time / 1000.0 * fps;
		frames = (frames > 0) ? frames : 1;
		int timeStep = time / frames;

		for (int i = 0; i < frames; i++) {
			SleepThread::msleep(timeStep);
			interpolatePos->alpha = float(i+1) / frames;
			interpolateRot->alpha = float(i+1) / frames;
			m_viewer->render();
		}

	}catch(std::exception& e) {
		qDebug(e.what());
	}

	if (interpolatePos) interpolatePos->unref();
	if (interpolateRot) interpolateRot->unref();
}

void ISpyDCSWidget::seekToTargetSmoothly(QList<QVariant>& cameraTargetSettings, int time, int fps) {
	if (cameraTargetSettings.size() != 7) {
		QMessageBox::warning(m_mainWindow, "Warning", 
			"Camera's settings should consists of 7 parameters!");
		
		return;
	}

	try {
		SbVec3f pos(
			cameraTargetSettings[0].toDouble(),
			cameraTargetSettings[1].toDouble(),
			cameraTargetSettings[2].toDouble());

		SbRotation rot(
			SbVec3f(
				cameraTargetSettings[3].toDouble(),
				cameraTargetSettings[4].toDouble(),
				cameraTargetSettings[5].toDouble()), 
			cameraTargetSettings[6].toDouble()
		);

		seekToTargetSmoothly(pos, rot, time, fps);
	} catch(std::exception& e) {
		qDebug(e.what());
	}
}

void ISpyDCSWidget::zoom(float level) {
	m_viewer->zoom(level);
}