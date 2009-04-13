//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT
#include "Iguana/QtGUI/interface/ISpyApplication.h"
#include "Iguana/QtGUI/interface/IgDrawFunctions.h"
#include "Iguana/QtGUI/interface/IgDrawFactoryService.h"
#include "Iguana/QtGUI/interface/ISpyMainWindow.h"
#include "Iguana/QtGUI/interface/ISpy3DView.h"
#include "Iguana/QtGUI/interface/Ig3DBaseModel.h"
#include "Iguana/QtGUI/interface/IGUANA_SPLASH.xpm"
#include "Iguana/QtGUI/interface/IgCollectionTableModel.h"
#include "Iguana/QtGUI/interface/IgMultiStorageTreeModel.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Framework/interface/IgParser.h"
#include "Iguana/Framework/interface/IgArgsElement.h"
#include "Iguana/Framework/interface/IgEnvsElement.h"
#include "Iguana/Framework/interface/IgPluginLoader.h"
#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/QtGUI/interface/ISpySelectionService.h"
#include "Iguana/QtGUI/interface/ISpySceneGraphService.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/QtGUI/interface/ISpyRecoContent.h"
#include "Iguana/QtGUI/interface/IgSoGL2PSAction.h"
#include "Iguana/QtGUI/interface/gl2ps.h"
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoNodes.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <QtGui>
#include <QApplication>
#include <QByteArray>
#include <QFileDialog>
#include <QGLWidget>
#include <QMessageBox>
#include <QPixmap>
#include <QPointF>
#include <QPrintDialog>
#include <QPrinter>
#include <QScrollArea>
#include <QSettings>
#include <QSplashScreen>
#include <QString>
#include <QTemporaryFile>
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iotools/InputStream.h"
#include "classlib/utils/Error.h"
#include "classlib/utils/Log.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/zip/ZipMember.h"
#include <iostream>
#include <vector>
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgParticleChar.h"
#include "Iguana/Inventor/interface/IgSoShapeKit.h"
#include "Iguana/Inventor/interface/IgSo2DArrow.h"
#include "Iguana/Inventor/interface/IgSo3DErrorBar.h"
#include "Iguana/Inventor/interface/IgSoArrow.h"
#include "Iguana/Inventor/interface/IgSoAxis.h"
#include "Iguana/Inventor/interface/IgSoCalHit.h"
#include "Iguana/Inventor/interface/IgSoCircleArc.h"
#include "Iguana/Inventor/interface/IgSoCoordinateAxis.h"
#include "Iguana/Inventor/interface/IgSoCrystalHit.h"
#include "Iguana/Inventor/interface/IgSoCube.h"
#include "Iguana/Inventor/interface/IgSoEllipsoid.h"
#include "Iguana/Inventor/interface/IgSoFieldPlane.h"
#include "Iguana/Inventor/interface/IgSoFieldPlaneMap.h"
#include "Iguana/Inventor/interface/IgSoG4Box.h"
#include "Iguana/Inventor/interface/IgSoG4Trap.h"
#include "Iguana/Inventor/interface/IgSoG4Trd.h"
#include "Iguana/Inventor/interface/IgSoGrid.h"
#include "Iguana/Inventor/interface/IgSoHits.h"
#include "Iguana/Inventor/interface/IgSoIdealTrack.h"
#include "Iguana/Inventor/interface/IgSoJet.h"
#include "Iguana/Inventor/interface/IgSoLegoPlot.h"
#include "Iguana/Inventor/interface/IgSoLegoTowers.h"
#include "Iguana/Inventor/interface/IgSoPcon.h"
#include "Iguana/Inventor/interface/IgSoPolyVol.h"
#include "Iguana/Inventor/interface/IgSoQuad.h"
#include "Iguana/Inventor/interface/IgSoRectHist.h"
#include "Iguana/Inventor/interface/IgSoRectColHist.h"
#include "Iguana/Inventor/interface/IgSoRotSolid.h"
#include "Iguana/Inventor/interface/IgSoRZHist.h"
#include "Iguana/Inventor/interface/IgSoSiStrips.h"
#include "Iguana/Inventor/interface/IgSoSimpleTrajectory.h"
#include "Iguana/Inventor/interface/IgSoSphereHit.h"
#include "Iguana/Inventor/interface/IgSoSplineTrack.h"
#include "Iguana/Inventor/interface/IgSoTower.h"
#include "Iguana/Inventor/interface/IgSoTowerRadii.h"
#include "Iguana/Inventor/interface/IgSoXYZGrid.h"
#include "Iguana/Inventor/interface/IgSoAnimator.h"
#include "Iguana/Inventor/interface/IgSoClipPlane.h"
#include "Iguana/Inventor/interface/IgSoSlicer.h"
#include "Iguana/Inventor/interface/IgSoSlicerEngine.h"
#include "Iguana/Inventor/interface/IgSoViewpoint.h"
#include "Iguana/Inventor/interface/IgSoPlaneManip.h"
#include "Iguana/Inventor/interface/IgSoCircularHist.h"
#include "Iguana/Inventor/interface/IgSoGridPlane.h"
#include "Iguana/Inventor/interface/IgSoGridPlaneMap.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>

lat::logflag LFiggi = { 0, "iggi", true, -1 };

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

static void initShapes (void)
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
    IgSoRectHist::initClass ();
    IgSoRectColHist::initClass ();
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
    IgSoGridPlane::initClass ();
    IgSoGridPlaneMap::initClass ();
    IgSoGL2PSAction::initClass ();
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace lat;

ISpyApplication::ISpyApplication (void)
    : m_state (0),
      m_argc (-1),
      m_argv (0),
      m_archive (0),
      m_geomArchive (0),
      m_appname (0),
      m_collectionModel (0),
      m_storageModel (new IgMultiStorageTreeModel),
      m_storage (0),
      m_geomStorage (0),
      m_init (false),
      m_timer (new QTimer (this)),
      m_auto (false)
{
    QCoreApplication::setOrganizationName ("Iguana");
    QCoreApplication::setOrganizationDomain ("iguana.cern.ch");
    QCoreApplication::setApplicationName ("iSpy");
    
    defaultSettings ();
}

ISpyApplication::~ISpyApplication (void)
{}

void
ISpyApplication::onExit (void)
{
    if (ISpyReadService* readService = ISpyReadService::get (state ()))
    {
	readService->postEventProcessing ();
	readService->postESProcessing ();
    }
    closeFile ();
    closeGeomFile ();
    if (ISpySceneGraphService* sceen = ISpySceneGraphService::get (state ()))
    {
	dynamic_cast<SoSeparator *>(sceen->sceneGraph ())->removeAllChildren ();
	dynamic_cast<SoSeparator *>(sceen->overlaySceneGraph ())->removeAllChildren ();
    }

    if (m_mainWindow->actionSaveSettings->isChecked ())
    {
	saveSettings ();
    }
    exit ();    
}

void
ISpyApplication::exit (void)
{
    qApp->closeAllWindows ();
}

IgState *
ISpyApplication::state (void) const
{ return m_state; }

int
ISpyApplication::argc (void) const
{ return m_argc; }

char **
ISpyApplication::argv (void) const
{ return m_argv; }

//////////////////////////////////////////////////////////////////////
int
ISpyApplication::run (int argc, char *argv[])
{    
    m_appname = argv [0];
    int i = 1;

    while (*++argv && argv [0][0] == '-')
    {
	i++;
	if (!strcmp (*argv, "--help"))
	{
	    return usage ();	    
	}
	else if (!strcmp (*argv, "--version"))
	{
	    return version ();
	}
	else 
	    break;
    }
    lat::Filename fakeApp (m_appname);
    
    m_argc = argc;
    m_argv = new char *[m_argc+1];

    m_argv [0] = const_cast<char *> (static_cast<const char *> (fakeApp));
    for (m_argc = 1; *argv; ++argv, ++m_argc)
        m_argv [m_argc] = *argv;
    m_argv [m_argc] = 0;

    int status;

    // Initialise the state
    if ((status = initState ()) != EXIT_SUCCESS)
        return status;

    return doRun ();
}

const char *
ISpyApplication::appname (void) const
{
    const char *name = m_appname;
    if (const char *ptr = strrchr (name, '/'))
	name = ptr + 1;
    return name;
}

int
ISpyApplication::usage (void)
{
    const char *app = appname ();
    std::cerr << "Usage: " << app << " [OPTION-OR-FILENAME]...\n"
	      << "   or: " << app << " --help\n"
	      << "   or: " << app << " --version\n";

    return EXIT_FAILURE;
}

int
ISpyApplication::version (void)
{
    const char *app = appname ();
    std::cerr << app << " 1.0a\n"
	      << "Copyright (C) 2009 CMS\n";

    return EXIT_SUCCESS;
}

int
ISpyApplication::initState (void)
{
    // FIXME: install other services?
    m_state = new IgState;
    new IgArgsElement (m_state, m_argc, m_argv);
    new IgEnvsElement (m_state);
    new IgPluginLoader (m_state);
    new ISpyReadService (m_state);
    new ISpyRecoContent (m_state);
    new IgDrawFunctions (m_state);

    return EXIT_SUCCESS;
}

void
ISpyApplication::defaultSettings (void) 
{
    QSettings settings;

    //
    // Open file dialog settings
    //
    if (! settings.contains ("igfiles/home"))
    {	
	QUrl url ("file:/afs/cern.ch/user/i/iguana/www/ispy/igfiles/");
	settings.setValue ("igfiles/home", url);
    }

    // 
    // Default geometry file
    // 
    if (! settings.contains ("igfiles/geometry"))
    {	
	QString fileName ("default-geometry.ig");
	settings.setValue ("igfiles/geometry", fileName);
    }

    //
    // Network connection configuration
    //
    if (! settings.contains ("igsource/host"))
    {
	QString hostName ("localhost");
	settings.setValue ("igsource/host", hostName);
    }
    if (! settings.contains ("igsource/port"))
    {
	int port = 9000;
	settings.setValue ("igsource/port", port);
    }
    if (! settings.contains ("igsource/debug"))
    {
	settings.setValue ("igsource/debug", false);
    }
    if (! settings.contains ("igevents/auto"))
    {
	settings.setValue ("igevents/auto", false);
    }
    if (! settings.contains ("igevents/timeout"))
    {
	int timeout = 15000;
	settings.setValue ("igevents/timeout", timeout);
    }
    if (! settings.contains ("igevents/cuts/jets/energy"))
    {
	settings.setValue ("igevents/cuts/jets/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/calotowers/energy"))
    {
	settings.setValue ("igevents/cuts/calotowers/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/ecal/barrel/rechits/energy"))
    {
	settings.setValue ("igevents/cuts/ecal/barrel/rechits/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/ecal/endcap/rechits/energy"))
    {
	settings.setValue ("igevents/cuts/ecal/endcap/rechits/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/hcal/barrel/rechits/energy"))
    {
	settings.setValue ("igevents/cuts/hcal/barrel/rechits/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/hcal/endcap/rechits/energy"))
    {
	settings.setValue ("igevents/cuts/hcal/endcap/rechits/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/hcal/forward/rechits/energy"))
    {
	settings.setValue ("igevents/cuts/hcal/forward/rechits/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/hcal/outer/rechits/energy"))
    {
	settings.setValue ("igevents/cuts/hcal/outer/rechits/energy", 0.1);
    }

    //
    // Main window configuration
    //
    if (! settings.contains ("mainwindow/configuration/save"))
    {
	settings.setValue ("mainwindow/configuration/save", false);
    }
    if (! settings.contains ("mainwindow/treeview/shown"))
    {
	settings.setValue ("mainwindow/treeview/shown", true);
    }
    if (! settings.contains ("mainwindow/treeview/floating"))
    {
	settings.setValue ("mainwindow/treeview/floating", false);
    }
    if (! settings.contains ("mainwindow/tableview/shown"))
    {
	settings.setValue ("mainwindow/tableview/shown", true);
    }
    if (! settings.contains ("mainwindow/tableview/floating"))
    {
	settings.setValue ("mainwindow/tableview/floating", false);
    }
}

void
ISpyApplication::restoreSettings (void) 
{
    QSettings settings;
    if (settings.contains ("igevents/auto"))
    {	
	m_auto = settings.value ("igevents/auto").value<bool> ();
	
	m_mainWindow->actionAuto->setEnabled (m_auto);
	m_mainWindow->actionAuto->setChecked (m_auto);
	autoEvents ();
    }
}

int
ISpyApplication::doRun (void)
{
    QApplication app (m_argc, m_argv);
    QWidget * mainwin = SoQt::init (m_argc, m_argv, m_argv[0]);
    
    setupMainWindow ();
    m_mainWindow->mdiArea->addSubWindow (mainwin);
    mainwin->setWindowTitle ("iSpy 3D");

    restoreSettings ();

    initShapes ();
    
    // FIXME: The scene graph will be moved out
    SoSeparator *root = new SoSeparator;
    root->addChild(new SoPerspectiveCamera);

    SoSeparator *sel = new SoSeparator;
    sel->setName (SbName ("CollectionSelection"));
    SoDrawStyle *sty = new SoDrawStyle;
    sty->style = SoDrawStyle::LINES;
    sty->lineWidth.setValue (3);
    sel->addChild (sty);

    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0xC0000000, rgbcomponents); // Free Speech Red
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sel->addChild (mat);
    SoSeparator *selSep = new SoSeparator;
    sel->addChild (selSep);
    root->addChild (sel);
    
    SoSeparator *mainScene = new SoSeparator;
    root->addChild (mainScene);
    SoSeparator *mainSep = new SoSeparator;
    mainScene->addChild (mainSep);

    SoSeparator *overlayScene = new SoSeparator;
    root->addChild (overlayScene);
    
    SoPerspectiveCamera *pcam = new SoPerspectiveCamera;
    pcam->position = SbVec3f (0, 0, 5);
    pcam->nearDistance = 0.1;
    pcam->farDistance = 10;
    overlayScene->addChild (pcam);

    SoAnnotation *overlaySep = new SoAnnotation;
    overlayScene->addChild (overlaySep);
  
    new ISpySceneGraphService (state (), mainSep, overlaySep);
    new ISpySelectionService (state (), selSep);

    Ig3DBaseModel *model = new Ig3DBaseModel (state ());
    model->sceneGraph ()->addChild (root);
    
    ISpy3DView *view = new ISpy3DView (state (), model, mainwin);
    view->setFeedbackVisibility (true);
    view->setTitle ("iSpy 3D");

    QSettings settings;
    QString fileName = settings.value ("igfiles/geometry").value<QString> ();
    loadGeomFile (fileName);

    ASSERT (m_splash);
    delete m_splash;
    
    if (m_argc == 2)
    {
	std::string fileName (m_argv [1]);
	if (fileName.size () > 2 && fileName.substr (fileName.size () - 3) == ".ig")
	{
	    loadFile (fileName.c_str ());
	    m_mainWindow->actionNext->setEnabled (true);
	    m_mainWindow->actionAuto->setEnabled (true);
	    nextEvent ();	    
	}
    }
    else
	open ();

    QObject::connect(m_mainWindow->actionQuit, SIGNAL(triggered()), this, SLOT(onExit()));
    QObject::connect(m_mainWindow->actionClose, SIGNAL(triggered()), this, SLOT(onExit()));
    QObject::connect(m_mainWindow->actionConnect, SIGNAL(triggered()), this, SLOT(connect()));

    SoQt::show(mainwin);
    SoQt::mainLoop();
  
    delete view;
    
    return 0;
}

void
ISpyApplication::setupMainWindow (void) 
{
    m_splash = new QSplashScreen(QPixmap (IGUANA_SPLASH));
    m_splash->show ();

    m_mainWindow = new ISpyMainWindow;
    QObject::connect (m_mainWindow->actionFileOpen, SIGNAL(triggered()), this, SLOT(open()));
    QObject::connect (m_mainWindow->actionAuto, SIGNAL(triggered()), this, SLOT(autoEvents()));
    QObject::connect (m_mainWindow->actionNext, SIGNAL(triggered()), this, SLOT(nextEvent()));
    QObject::connect (m_mainWindow->actionPrevious, SIGNAL(triggered()), this, SLOT(previousEvent()));
    QObject::connect (m_mainWindow->actionRewind, SIGNAL(triggered()), this, SLOT(rewind()));
    QObject::connect (m_mainWindow->actionPrint, SIGNAL(triggered()), this, SLOT(print ()));
    QObject::connect (m_mainWindow->actionSave, SIGNAL(triggered()), this, SLOT(save ()));
    
    m_mainWindow->actionNext->setEnabled (false);
    m_mainWindow->actionPrevious->setEnabled (false);
    m_mainWindow->treeView->setModel (m_storageModel);
    m_mainWindow->tableView->setModel (m_storageModel);
    restoreMainWindowSettings ();

    m_mainWindow->treeView->setSortingEnabled (true);
    m_mainWindow->show ();

    QObject::connect (m_mainWindow->treeView, SIGNAL(clicked(const QModelIndex)),this,SLOT(collectionChanged(const QModelIndex)));
    QObject::connect (m_mainWindow->treeView, SIGNAL(clicked(const QModelIndex)),this,SLOT(displayCollection(const QModelIndex)));
    QObject::connect (m_mainWindow->tableView, SIGNAL(clicked(const QModelIndex)),this,SLOT(displayItem(const QModelIndex)));
}

void 
ISpyApplication::restoreMainWindowSettings (void) 
{
    QSettings settings;
    m_mainWindow->dockTreeWidget->setShown (settings.value ("mainwindow/treeview/shown").value<bool> ());
    m_mainWindow->dockTreeWidget->setFloating(settings.value ("mainwindow/treeview/floating").value<bool> ());
    m_mainWindow->dockTableWidget->setShown (settings.value ("mainwindow/tableview/shown").value<bool> ());
    m_mainWindow->dockTableWidget->setFloating(settings.value ("mainwindow/tableview/floating").value<bool> ());
    m_mainWindow->actionSaveSettings->setChecked (settings.value ("mainwindow/configuration/save").value<bool> ());
}

void
ISpyApplication::saveSettings (void)
{
    bool treeFlag = m_mainWindow->actionTwig_Explorer->isChecked ();
    bool tableFlag = m_mainWindow->actionObject_Inspector->isChecked ();

    QSettings settings;
    settings.setValue ("mainwindow/configuration/save", m_mainWindow->actionSaveSettings->isChecked ());
    settings.setValue ("mainwindow/treeview/shown", treeFlag);
    settings.setValue ("mainwindow/treeview/floating", m_mainWindow->dockTreeWidget->isFloating ());
    settings.setValue ("mainwindow/tableview/shown", tableFlag);
    settings.setValue ("mainwindow/tableview/floating", m_mainWindow->dockTableWidget->isFloating ());

    settings.setValue ("igevents/auto", m_auto);
}

void 
ISpyApplication::collectionChanged(const QModelIndex &index)
{
    QString collectionName = m_storageModel->data(index, Qt::DisplayRole).toString();
    qDebug() << collectionName << " selected";
    if (!index.parent().isValid())
    {
	return;
    }
    if (m_collectionModel == 0)
    {	
	m_collectionModel = new IgCollectionTableModel (&(m_storage->getCollectionByIndex (1)));
        m_mainWindow->tableView->setModel (m_collectionModel);
    }
    if (m_storage != 0 && m_storage->hasCollection (collectionName.toAscii()))
    {	
	m_collectionModel->setCollection (m_storage->getCollectionPtr(collectionName.toAscii()));
    }
    else if (m_geomStorage != 0 && m_geomStorage->hasCollection (collectionName.toAscii()))
    {
	m_collectionModel->setCollection (m_geomStorage->getCollectionPtr(collectionName.toAscii()));
    }
}

void 
ISpyApplication::displayCollection(const QModelIndex &index)
{
    QString collectionName = m_storageModel->data(index, Qt::DisplayRole).toString();
    qDebug() << collectionName << " displayed";
    
    IgCollection *collection = 0;
    
    if (!index.parent().isValid())
    {
	return;
    }
    if (m_storage->hasCollection (collectionName.toAscii()))
    {	
	collection = m_storage->getCollectionPtr(collectionName.toAscii());
    }
    else if (m_geomStorage->hasCollection (collectionName.toAscii()))
    {
	collection = m_geomStorage->getCollectionPtr(collectionName.toAscii());
    }
    
    if (collection != NULL && collection->size () > 0)
    {
	drawCollection (collection);
    }
}	

void 
ISpyApplication::displayItem(const QModelIndex &index)
{
    SoSeparator *node = dynamic_cast<SoSeparator *>(ISpySceneGraphService::get (state ())->sceneGraph ());
    ASSERT (node);	
    node->removeAllChildren ();	

    QModelIndex selected = m_mainWindow->treeView->selectionModel ()->currentIndex ();
    qDebug() << selected << " parent selection";
    
    QString collectionName = m_storageModel->data(selected, Qt::DisplayRole).toString();
    qDebug() << collectionName << " collection";

    IgCollection *collection = 0;
    if (m_storage->hasCollection (collectionName.toAscii()))
    {	
	collection = m_storage->getCollectionPtr(collectionName.toAscii());
    }
    else if (m_geomStorage->hasCollection (collectionName.toAscii()))
    {
	collection = m_geomStorage->getCollectionPtr(collectionName.toAscii());
    }
    if (collection != NULL && collection->size () > 0)
    {
	SoVertexProperty *vertices = new SoVertexProperty;
	    
	int i = 0;
	std::vector<int> lineIndices;
	std::vector<SbVec3f> corners;		
	    
	std::string shape;

	if (collection->hasProperty ("front_1") &&  
	    collection->hasProperty ("front_2") &&
	    collection->hasProperty ("front_3") &&
	    collection->hasProperty ("front_4"))
	{		
	    shape = "hexahedron";
	} 
	else if (collection->hasProperty ("pos_1") && collection->hasProperty ("pos_2"))
	{
	    shape = "line";	    
	}
	else if (collection->hasProperty ("pos"))
	{
	    shape = "point";	    
	}
	IgCollectionIterator cit = collection->begin ();
	qDebug() << index.row () << " selected";
	cit += index.row ();
	IgCollectionItem vtxShape = *cit;
	if (collection->hasProperty ("detid"))
	{
	    int detid = vtxShape.get<int> ("detid");	    
	    qDebug() << detid << " detid";
	}
	
	if (shape == "hexahedron")
	{		
	    IgV3d p1  = vtxShape.get<IgV3d> ("front_1");
	    corners.push_back (SbVec3f (static_cast<double>(p1.x ()), 
					static_cast<double>(p1.y ()),
					static_cast<double>(p1.z ())));		    
	    IgV3d p2  = vtxShape.get<IgV3d> ("front_2");
	    corners.push_back (SbVec3f (static_cast<double>(p2.x ()), 
					static_cast<double>(p2.y ()),
					static_cast<double>(p2.z ())));
	    IgV3d p3  = vtxShape.get<IgV3d> ("front_3");
	    corners.push_back (SbVec3f (static_cast<double>(p3.x ()), 
					static_cast<double>(p3.y ()),
					static_cast<double>(p3.z ())));
	    IgV3d p4  = vtxShape.get<IgV3d> ("front_4");
	    corners.push_back (SbVec3f (static_cast<double>(p4.x ()), 
					static_cast<double>(p4.y ()),
					static_cast<double>(p4.z ())));
	    IgV3d p5  = vtxShape.get<IgV3d> ("back_1");
	    corners.push_back (SbVec3f (static_cast<double>(p5.x ()), 
					static_cast<double>(p5.y ()),
					static_cast<double>(p5.z ())));
	    IgV3d p6  = vtxShape.get<IgV3d> ("back_2");
	    corners.push_back (SbVec3f (static_cast<double>(p6.x ()), 
					static_cast<double>(p6.y ()),
					static_cast<double>(p6.z ())));
	    IgV3d p7  = vtxShape.get<IgV3d> ("back_3");
	    corners.push_back (SbVec3f (static_cast<double>(p7.x ()), 
					static_cast<double>(p7.y ()),
					static_cast<double>(p7.z ())));
	    IgV3d p8  = vtxShape.get<IgV3d> ("back_4");
	    corners.push_back (SbVec3f (static_cast<double>(p8.x ()), 
					static_cast<double>(p8.y ()),
					static_cast<double>(p8.z ())));	    
	    lineIndices.push_back (i);
	    lineIndices.push_back (i + 1);
	    lineIndices.push_back (i + 2);
	    lineIndices.push_back (i + 3);
	    lineIndices.push_back (i);
	    lineIndices.push_back (SO_END_LINE_INDEX);
		    
	    lineIndices.push_back (i + 4);
	    lineIndices.push_back (i + 5);
	    lineIndices.push_back (i + 6);
	    lineIndices.push_back (i + 7);
	    lineIndices.push_back (i + 4);
	    lineIndices.push_back (SO_END_LINE_INDEX);

	    lineIndices.push_back (i);
	    lineIndices.push_back (i + 4);
	    lineIndices.push_back (SO_END_LINE_INDEX);
	    lineIndices.push_back (i + 1);
	    lineIndices.push_back (i + 5);
	    lineIndices.push_back (SO_END_LINE_INDEX);
	    lineIndices.push_back (i + 2);
	    lineIndices.push_back (i + 6);
	    lineIndices.push_back (SO_END_LINE_INDEX);
	    lineIndices.push_back (i + 3);
	    lineIndices.push_back (i + 7);
	    lineIndices.push_back (SO_END_LINE_INDEX);		
	    i += 8;
	}
	else if (shape == "point")
	{
	    IgV3d p1  = vtxShape.get<IgV3d> ("pos");
	    corners.push_back (SbVec3f (static_cast<double>(p1.x ()), 
					static_cast<double>(p1.y ()),
					static_cast<double>(p1.z ())));
	}
	else if (shape == "line")
	{
	    IgV3d p1  = vtxShape.get<IgV3d> ("pos_1");
	    corners.push_back (SbVec3f (static_cast<double>(p1.x ()), 
					static_cast<double>(p1.y ()),
					static_cast<double>(p1.z ())));
	    IgV3d p2  = vtxShape.get<IgV3d> ("pos_2");
	    corners.push_back (SbVec3f (static_cast<double>(p2.x ()), 
					static_cast<double>(p2.y ()),
					static_cast<double>(p2.z ())));
	    lineIndices.push_back (i);
	    lineIndices.push_back (i + 1);
	    lineIndices.push_back (SO_END_LINE_INDEX);
	    i += 2;
	}		
	vertices->vertex.setValues (0, corners.size (), &corners [0]);
	vertices->vertex.setNum (corners.size ());	    

	if (shape == "hexahedron" 
	    || shape == "line")
	{			
	    SoIndexedLineSet *lineSet = new SoIndexedLineSet;
	    lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
	    lineSet->vertexProperty = vertices;

	    node->addChild (lineSet);
	}
	else if (shape == "point")
	{
	    SoPointSet *pointSet = new SoPointSet;
	    pointSet->vertexProperty.setValue (vertices);
	    pointSet->numPoints.setValue (corners.size ());

	    node->addChild (pointSet);
	}
    }
}

void
ISpyApplication::drawCollection (IgCollection *collection) 
{
    if (ISpySelectionService *sel = ISpySelectionService::get (state ()))
    {
	SoSeparator *selSep = dynamic_cast<SoSeparator *>(sel->selection ());	
	ASSERT (selSep);
	selSep->removeAllChildren ();

	std::string collName = collection->name ();
    
	qDebug() << "Draw " << collName.c_str ();

	if (collName == "EcalRecHits_V1") 
	{	
	    IgDrawFactoryService *drawService = IgDrawFactoryService::get (state ());
	    std::string what = "CrystalHits";
	    std::string repName = "3D";
    
	    SoSeparator *rep = dynamic_cast<SoSeparator *>(drawService->draw (what, state (), collection->name (), repName));
	    selSep->addChild (rep);
	}
	else if (collName == "HBRecHits_V1" || 
		 collName == "HERecHits_V1" ||
		 collName == "HFRecHits_V1" ||
		 collName == "HORecHits_V1")
	{
	    IgDrawFactoryService *drawService = IgDrawFactoryService::get (state ());
	    std::string what = "CrystalHits";
	    std::string repName = "3D";
    
	    SoSeparator *rep = dynamic_cast<SoSeparator *>(drawService->draw (what, state (), collection->name (), repName));
	    selSep->addChild (rep);
	}    
	else
	{	
	    SoVertexProperty *vertices = new SoVertexProperty;
	    
	    int i = 0;
	    std::vector<int> lineIndices;
	    std::vector<SbVec3f> corners;		
	    
	    std::string shape;

	    if (collection->hasProperty ("front_1") &&  
		collection->hasProperty ("front_2") &&
		collection->hasProperty ("front_3") &&
		collection->hasProperty ("front_4"))
	    {		
		shape = "hexahedron";
	    } 
	    else if (collection->hasProperty ("pos_1") && collection->hasProperty ("pos_2"))
	    {
		shape = "line";	    
	    }
	    else if (collection->hasProperty ("pos"))
	    {
		shape = "point";	    
	    }
	
	    IgCollectionIterator cit = collection->begin ();
	    IgCollectionIterator cend = collection->end ();
	    for (; cit != cend ; cit++) 
	    {
		IgCollectionItem vtxShape = *cit;
		
		if (shape == "hexahedron")
		{		
		    IgV3d p1  = vtxShape.get<IgV3d> ("front_1");
		    corners.push_back (SbVec3f (static_cast<double>(p1.x ()), 
						static_cast<double>(p1.y ()),
						static_cast<double>(p1.z ())));		    
		    IgV3d p2  = vtxShape.get<IgV3d> ("front_2");
		    corners.push_back (SbVec3f (static_cast<double>(p2.x ()), 
						static_cast<double>(p2.y ()),
						static_cast<double>(p2.z ())));
		    IgV3d p3  = vtxShape.get<IgV3d> ("front_3");
		    corners.push_back (SbVec3f (static_cast<double>(p3.x ()), 
						static_cast<double>(p3.y ()),
						static_cast<double>(p3.z ())));
		    IgV3d p4  = vtxShape.get<IgV3d> ("front_4");
		    corners.push_back (SbVec3f (static_cast<double>(p4.x ()), 
						static_cast<double>(p4.y ()),
						static_cast<double>(p4.z ())));
		    IgV3d p5  = vtxShape.get<IgV3d> ("back_1");
		    corners.push_back (SbVec3f (static_cast<double>(p5.x ()), 
						static_cast<double>(p5.y ()),
						static_cast<double>(p5.z ())));
		    IgV3d p6  = vtxShape.get<IgV3d> ("back_2");
		    corners.push_back (SbVec3f (static_cast<double>(p6.x ()), 
						static_cast<double>(p6.y ()),
						static_cast<double>(p6.z ())));
		    IgV3d p7  = vtxShape.get<IgV3d> ("back_3");
		    corners.push_back (SbVec3f (static_cast<double>(p7.x ()), 
						static_cast<double>(p7.y ()),
						static_cast<double>(p7.z ())));
		    IgV3d p8  = vtxShape.get<IgV3d> ("back_4");
		    corners.push_back (SbVec3f (static_cast<double>(p8.x ()), 
						static_cast<double>(p8.y ()),
						static_cast<double>(p8.z ())));	    
		    lineIndices.push_back (i);
		    lineIndices.push_back (i + 1);
		    lineIndices.push_back (i + 2);
		    lineIndices.push_back (i + 3);
		    lineIndices.push_back (i);
		    lineIndices.push_back (SO_END_LINE_INDEX);
		    
		    lineIndices.push_back (i + 4);
		    lineIndices.push_back (i + 5);
		    lineIndices.push_back (i + 6);
		    lineIndices.push_back (i + 7);
		    lineIndices.push_back (i + 4);
		    lineIndices.push_back (SO_END_LINE_INDEX);

		    lineIndices.push_back (i);
		    lineIndices.push_back (i + 4);
		    lineIndices.push_back (SO_END_LINE_INDEX);
		    lineIndices.push_back (i + 1);
		    lineIndices.push_back (i + 5);
		    lineIndices.push_back (SO_END_LINE_INDEX);
		    lineIndices.push_back (i + 2);
		    lineIndices.push_back (i + 6);
		    lineIndices.push_back (SO_END_LINE_INDEX);
		    lineIndices.push_back (i + 3);
		    lineIndices.push_back (i + 7);
		    lineIndices.push_back (SO_END_LINE_INDEX);		
		    i += 8;
		}
		else if (shape == "point")
		{
		    IgV3d p1  = vtxShape.get<IgV3d> ("pos");
		    corners.push_back (SbVec3f (static_cast<double>(p1.x ()), 
						static_cast<double>(p1.y ()),
						static_cast<double>(p1.z ())));
		}
		else if (shape == "line")
		{
		    IgV3d p1  = vtxShape.get<IgV3d> ("pos_1");
		    corners.push_back (SbVec3f (static_cast<double>(p1.x ()), 
						static_cast<double>(p1.y ()),
						static_cast<double>(p1.z ())));
		    IgV3d p2  = vtxShape.get<IgV3d> ("pos_2");
		    corners.push_back (SbVec3f (static_cast<double>(p2.x ()), 
						static_cast<double>(p2.y ()),
						static_cast<double>(p2.z ())));
		    lineIndices.push_back (i);
		    lineIndices.push_back (i + 1);
		    lineIndices.push_back (SO_END_LINE_INDEX);
		    i += 2;
		}		
	    }
	    vertices->vertex.setValues (0, corners.size (), &corners [0]);
	    vertices->vertex.setNum (corners.size ());	    

	    if (shape == "hexahedron" 
		|| shape == "line")
	    {			
		SoIndexedLineSet *lineSet = new SoIndexedLineSet;
		lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
		lineSet->vertexProperty = vertices;

		selSep->addChild (lineSet);
	    }
	    else if (shape == "point")
	    {
		SoPointSet *pointSet = new SoPointSet;
		pointSet->vertexProperty.setValue (vertices);
		pointSet->numPoints.setValue (corners.size ());

		selSep->addChild (pointSet);
	    }
	}
    }
}
void
ISpyApplication::cleanScene (void) 
{    
    if (ISpySelectionService *sel = ISpySelectionService::get (state ()))
    {
	dynamic_cast<SoSeparator *>(sel->selection ())->removeAllChildren ();	
    }
    if (ISpySceneGraphService* sceen = ISpySceneGraphService::get (state ()))
    {
	dynamic_cast<SoSeparator *>(sceen->sceneGraph ())->removeAllChildren ();
	dynamic_cast<SoSeparator *>(sceen->overlaySceneGraph ())->removeAllChildren ();
    }    
}

void
ISpyApplication::open (void) 
{
    QString fileName;
    QFileDialog f (m_mainWindow->centralwidget, tr("Open File"), ".",
		   tr("Ig Files (*.ig *.iggi *.iguana)"));
    f.setFileMode(QFileDialog::ExistingFiles);
    QList<QUrl> shortcuts = f.sidebarUrls ();
    
    QSettings settings;
    QUrl url = settings.value ("igfiles/home").value<QUrl> ();
    shortcuts.append (url);
    f.setSidebarUrls (shortcuts);

    if (f.exec ())
    {
	fileName = f.selectedFiles ().front();

	if (! fileName.isEmpty ())
	{
	    m_mainWindow->statusBar ()->showMessage(QString("Opening ") + fileName + tr("..."));
	    if (m_archive != 0)
		closeFile ();

	    loadFile (fileName);
	    m_mainWindow->actionNext->setEnabled (true);
	    m_mainWindow->actionAuto->setEnabled (true);
	    nextEvent ();	
	}
	else 
	{	
	    m_mainWindow->actionNext->setEnabled (false);
	    m_mainWindow->actionAuto->setEnabled (false);
	}
    }
}

void
ISpyApplication::connect (void)
{
    m_consumer.nextEvent ();
}

void
ISpyApplication::autoEvents (void)
{
    QSettings settings;
    int timeout = 15000;
    
    if (settings.contains ("igevents/timeout"))
	timeout = settings.value ("igevents/timeout").value<int> ();

    m_auto = m_mainWindow->actionAuto->isChecked ();

    if (m_auto)
    {
	QObject::connect (m_timer, SIGNAL (timeout ()), SLOT (nextEvent ()));
        m_timer->start (timeout); // set 5 seconds apart
    }
    else
    {
	m_timer->stop ();
        m_timer->disconnect ();
    }
}

void
ISpyApplication::nextEvent (void)
{    
    cleanScene ();
    
    if (! m_init)
    {
	m_init = true;	
	readGeomZipMember (m_geomIt);
    }    
    if (m_it != m_archive->end ())
    {
	if ((*m_it)->name () == "Header")
	{
	    m_it++;	    
	    nextEvent ();
	}
	else 
	{	    
	    ISpyEventMessage event;
	    event.message = (*m_it)->name ();
	
	    m_mainWindow->statusBar ()->showMessage (QString ((*m_it)->name ()));
	
	    readZipMember (m_it);
	    m_mainWindow->actionPrevious->setEnabled (true);
	
	    ISpyReadService* readService = ISpyReadService::get (state ());
	    ASSERT (readService);
    
	    readService->dispatcher (ISpyReadService::NewEvent)->broadcast (event);
	    ++m_it;
	}	
    }
    else 
	m_mainWindow->actionNext->setEnabled (false);
}

void
ISpyApplication::previousEvent (void)
{    
    cleanScene ();

    if (m_it != m_archive->begin ())
    {
	--m_it;
    
	ISpyEventMessage event;
	event.message = (*m_it)->name ();

	m_mainWindow->statusBar ()->showMessage (QString ((*m_it)->name ()));

	readZipMember (m_it);
	m_mainWindow->actionNext->setEnabled (true);
	
	ISpyReadService* readService = ISpyReadService::get (state ());
	ASSERT (readService);
	
	readService->dispatcher (ISpyReadService::NewEvent)->broadcast (event);
    }
    else 
	m_mainWindow->actionPrevious->setEnabled (false);
}

void
ISpyApplication::rewind (void)
{    
    m_it = m_archive->begin ();
    
    m_mainWindow->statusBar ()->showMessage (QString ((*m_it)->name ()));
    nextEvent ();
}

void
ISpyApplication::print (void) 
{
#ifndef QT_NO_PRINTER
    QPrinter printer; 
    QPrintDialog dialog (&printer, m_mainWindow->centralwidget);
    dialog.setWindowTitle (tr("Print To File"));
    if (dialog.exec () == QDialog::Accepted)
    {
	QPrinter printer(QPrinter::HighResolution);
	printer.setPaperSize(QPrinter::A4);

	QPainter painter(&printer);
// 	m_mainWindow->graphicsView->render (&painter);
    }
#endif
}

void
ISpyApplication::printVector (QString file, QString format, int level)
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

	if (m_mainWindow->mdiArea->currentSubWindow () != 0)
	{	    
	    if (! gl2psAction )
		gl2psAction = new IgSoGL2PSAction (dynamic_cast<SoQtExaminerViewer *>(m_mainWindow->mdiArea->currentSubWindow ())->getViewportRegion ());
	
	    gl2psAction->setViewportRegion (dynamic_cast<SoQtExaminerViewer *>(m_mainWindow->mdiArea->currentSubWindow ())->getViewportRegion ());
	    SoGLRenderAction* prevAction = dynamic_cast<SoQtExaminerViewer *>(m_mainWindow->mdiArea->currentSubWindow ())->getGLRenderAction ();
	    dynamic_cast<SoQtExaminerViewer *>(m_mainWindow->mdiArea->currentSubWindow ())->setGLRenderAction (gl2psAction);
	
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
		// FIXME: dynamic_cast<SoQtExaminerViewer *>(m_mainWindow->mdiArea->currentSubWindow ())->actualRedraw ();
		state = gl2psEndPage();
		if (state == GL2PS_OVERFLOW)
		    m_gl2psFBBufferSize += 1024*1024;
	    }
	    fclose (output);
	    dynamic_cast<SoQtExaminerViewer *>(m_mainWindow->mdiArea->currentSubWindow ())->setGLRenderAction (prevAction);
	}
    }
    else
    {
        LOG(0, trace, LFiggi, QString(file +": Failed to open file for writing.\n").toStdString ());
	QMessageBox::warning (m_mainWindow, "System Error",
			      "Failed to open file \""+file+"\" for writing.",
			      "Ok");	
    }
}

int
ISpyApplication::getGL2PSOptions (void)
{ return m_gl2psOptions; }

void
ISpyApplication::save (void) 
{
    QPixmap pixmap;
    QPainter painter (&pixmap);
    painter.setRenderHint (QPainter::Antialiasing);
    painter.end ();

    pixmap.save ("scene.png");
}

void
ISpyApplication::processEvent (unsigned int runNum, unsigned int eventNum)
{}

void
ISpyApplication::processEventDialog (void) 
{}

void
ISpyApplication::skipEvent (long num)
{}

void
ISpyApplication::skipEventDialog (void)
{}

void
ISpyApplication::closeFile (void)
{
    if (m_collectionModel != 0)
    {
	delete m_collectionModel;
	m_collectionModel = 0;	
    }
    ASSERT (m_storageModel);    
    m_storageModel->clear ();

    if (m_archive)
    {
	m_archive->close ();
	delete m_archive;
	m_archive = 0;
    }
}

void
ISpyApplication::closeGeomFile (void)
{
    if (m_geomArchive)
    {
	m_geomArchive->close ();
	delete m_geomArchive;
	m_geomArchive = 0;
    }
}

void
ISpyApplication::loadFile (const QString &filename)
{
    Filename inputFilename (Filename (filename.toStdString ()).substitute (ShellEnvironment ()));
    if (inputFilename.exists())
    {
	m_mainWindow->statusBar ()->showMessage(QString("Loading ") + filename + tr("..."));
	m_archive = new ZipArchive (inputFilename, IOFlags::OpenRead);
	m_it = m_archive->begin ();
    }
    else
	std::cout << "Event file " << inputFilename << " does not exist." << std::endl;
}

void
ISpyApplication::loadGeomFile (const QString &filename)
{
    Filename inputFilename (Filename (filename.toStdString ()).substitute (ShellEnvironment ()));
    if (inputFilename.exists())
    {
	m_mainWindow->statusBar ()->showMessage(QString("Loading ") + filename + tr("..."));
	m_geomArchive = new ZipArchive (inputFilename, IOFlags::OpenRead);
	m_geomIt = m_geomArchive->begin ();
	m_mainWindow->statusBar ()->clearMessage ();
    }
    else
    {
	m_init = true;	
	std::cout << "Geometry file " << inputFilename << " does not exist." << std::endl;
    }
}

void
ISpyApplication::readZipMember (lat::ZipArchive::Iterator i)
{
    m_mainWindow->statusBar ()->showMessage(QString("Reading ") + (*i)->name () + tr("..."));

    ISpyReadService* readService = ISpyReadService::get (state ());
    ASSERT (readService);
    readService->preEventProcessing ();

    ASSERT (m_archive);
    InputStream *is = m_archive->input (*i);
    
    IOSize length = (*i)->size (ZipMember::UNCOMPRESSED);
    std::vector<char> buffer(length+1, 0);
    is->xread(&buffer[0], length);
    is->close ();
    delete is;

    m_storage = readService->dataStorage ();
    IgParser parser (m_storage);
    try
    {
	parser.parse (&buffer[0]);
	m_storageModel->clear ();
	if (m_collectionModel != 0)
	{
	    delete m_collectionModel;
	    m_collectionModel = 0;	
	}
	if (m_geomArchive != 0)
	{
	    m_storageModel->addStorage (m_geomStorage, (*m_geomIt)->name ().name ());
	}
	
	m_storageModel->addStorage (m_storage, (*i)->name ().name ());
	m_mainWindow->treeView->expandAll ();
    }
    catch (ParseError &e)
    {
	std::cout << "**** IguanaApplication::readZipMember ****\n ParseError at char n. " << std::endl;
    }
    catch (lat::Error &e) 
    {
	std::cout << "**** IguanaApplication::readZipMember ****\n lat::Error " << e.explainSelf () << std::endl;
    }
    catch (std::exception &e) 
    {
	std::cout << "**** IguanaApplication::readZipMember ****\n Standard library exception caught: " << e.what () << std::endl;
    }
    catch (...) 
    {	
	std::cout << "**** IguanaApplication ****\n Unknown Exception" << std::endl;
    }
    m_mainWindow->statusBar ()->showMessage (tr((*i)->name ()));
}

void
ISpyApplication::readGeomZipMember (lat::ZipArchive::Iterator i)
{
    m_mainWindow->statusBar ()->showMessage(QString("Reading ") + (*i)->name () + tr("..."));

    ISpyReadService* readService = ISpyReadService::get (state ());
    ASSERT (readService);
    readService->preESProcessing ();

    ASSERT (m_geomArchive);
    InputStream *is = m_geomArchive->input (*i);
    
    IOSize length = (*i)->size (ZipMember::UNCOMPRESSED);
    std::vector<char> buffer(length+1, 0);
    is->xread(&buffer[0], length);
    is->close ();
    delete is;

    m_geomStorage = readService->esStorage ();
    IgParser parser (m_geomStorage);
    try
    {
	parser.parse (&buffer[0]);
	m_storageModel->addStorage (m_geomStorage, (*i)->name ().name ());
    }
    catch (ParseError &e)
    {
	std::cout << "**** IguanaApplication::readGeomZipMember ****\n ParseError at char n. " << std::endl;
    }
    catch (lat::Error &e) 
    {
	std::cout << "**** IguanaApplication::readGeomZipMember ****\n lat::Error " << e.explainSelf () << std::endl;
    }
    catch (std::exception &e) 
    {
	std::cout << "**** IguanaApplication::readGeomZipMember ****\n Standard library exception caught: " << e.what () << std::endl;
    }
    catch (...) 
    {	
	std::cout << "**** IguanaApplication::readGeomZipMember ****\n Unknown Exception" << std::endl;
    }
    m_mainWindow->statusBar ()->showMessage (tr((*i)->name ()));
}
