//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT
#include "Iguana/QtGUI/interface/ISpyApplication.h"
#include "Iguana/QtGUI/interface/ISpyEventFilter.h"
#include "Iguana/QtGUI/interface/IgDrawFunctions.h"
#include "Iguana/QtGUI/interface/IgDrawFactoryService.h"
#include "Iguana/QtGUI/interface/ISpyMainWindow.h"
#include "Iguana/QtGUI/interface/ISpy3DView.h"
#include "Iguana/QtGUI/interface/Ig3DBaseModel.h"
#include "Iguana/QtGUI/interface/IgCollectionTableModel.h"
#include "Iguana/QtGUI/interface/IgMultiStorageTreeModel.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Framework/interface/IgParser.h"
#include "Iguana/Framework/interface/IgArgsElement.h"
#include "Iguana/Framework/interface/IgPluginLoader.h"
#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/QtGUI/interface/ISpySplashScreen.h"
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
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <QtGui>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iotools/InputStream.h"
#include "classlib/utils/Error.h"
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
      m_mainWindow (0),
      m_archive (0),
      m_geomArchive (0),
      m_appname (0),
      m_collectionModel (0),
      m_storageModel (new IgMultiStorageTreeModel),
      m_3DModel (0),
      m_storage (0),
      m_geomStorage (0),
      m_onExit (false),
      m_timer (new QTimer (this)),
      m_auto (false)
{
#ifndef Q_WS_MAC
    QCoreApplication::setApplicationName ("ispy");
#else
    QCoreApplication::setApplicationName ("iSpy");
#endif
    QCoreApplication::setApplicationVersion ("1.x (beta)");
    QCoreApplication::setOrganizationDomain ("iguana");
    QCoreApplication::setOrganizationName ("iguana");
    
    if (QDir::home ().isReadable ())
	defaultSettings ();

    // FIXME: We don't have selection currently. QObject::connect (this, SIGNAL(modelChanged ()), this, SLOT(cleanSelection ()));
    QObject::connect (&m_readThread, SIGNAL(readMember (const QString&)), this, SLOT (newMember (const QString&)));
}



ISpyApplication::~ISpyApplication (void)
{}



void
ISpyApplication::onExit (void)
{
    m_onExit = true;    
    m_readThread.quit ();
    
    if (ISpyReadService* readService = ISpyReadService::get (state ()))
    {
	readService->postEventProcessing ();
	readService->postESProcessing ();
    }
    closeFile ();
    closeGeomFile ();
    if (ISpySceneGraphService* scene = ISpySceneGraphService::get (state ()))
    {
	dynamic_cast<SoSeparator *>(scene->sceneGraph ())->removeAllChildren ();
    }

    m_mainWindow->saveSettings ();

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
    std::cerr << QCoreApplication::applicationName ().toStdString () << " " 
	      << QCoreApplication::applicationVersion ().toStdString () << "\n";

    return EXIT_SUCCESS;
}



int
ISpyApplication::initState (void)
{
    // FIXME: install other services? Remove some?
    m_state = new IgState;
    new IgArgsElement (m_state, m_argc, m_argv);
    new ISpyReadService (m_state);
    new IgDrawFunctions (m_state);

    return EXIT_SUCCESS;
}



void
ISpyApplication::defaultSettings (void) 
{
    QSettings settings;

    if (settings.isWritable ())
    {	
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

	// HLR
	if (! settings.contains ("igdisplay/hadtowers/view3d/hiddenlineremoval"))
	{
	    settings.setValue ("igdisplay/hadtowers/view3d/hiddenlineremoval", false);	    
	}	
	if (! settings.contains ("igdisplay/crystalhits/view3d/hiddenlineremoval"))
	{
	    settings.setValue ("igdisplay/crystalhits/view3d/hiddenlineremoval", false);	    
	}	
	if (! settings.contains ("igdisplay/crystals/view3d/hiddenlineremoval"))
	{
	    settings.setValue ("igdisplay/crystals/view3d/hiddenlineremoval", false);	    
	}	
	if (! settings.contains ("igdisplay/detector/view3d/hiddenlineremoval"))
	{
	    settings.setValue ("igdisplay/detector/view3d/hiddenlineremoval", false);	    
	}	
    }
}



void
ISpyApplication::restoreSettings (void) 
{
    QSettings settings;
    if (settings.contains ("igevents/auto"))
    {	
	m_auto = settings.value ("igevents/auto").value<bool> ();
	autoEvents ();
    }
}



int
ISpyApplication::doRun (void)
{
    IgArgsElement *args = IgArgsElement::get (m_state);
    QApplication app (args->args (), args->argv ());
    ISpyEventFilter filter;
    app.installEventFilter (&filter);
    QObject::connect (&filter, SIGNAL(open (const QString &)), this, SLOT(openFile (const QString &)));

    QWidget * mainwin = SoQt::init (args->args (), args->argv (), args->argv ()[0]);
    
    setupMainWindow ();
    m_mainWindow->workspace ()->addSubWindow (mainwin);
    mainwin->setWindowTitle ("iSpy 3D");
    
    //     QGraphicsView *rphiView = new QGraphicsView;
    //     m_mainWindow->mdiArea->addSubWindow (rphiView);
    //     rphiView->setWindowTitle ("iSpy RPhi");
    //     rphiView->show ();
    
    //     QGraphicsView *rzView = new QGraphicsView;
    //     m_mainWindow->mdiArea->addSubWindow (rzView);
    //     rzView->setWindowTitle ("iSpy RZ");
    //     rzView->show ();

    restoreSettings ();

    initShapes ();

    m_3DModel = new Ig3DBaseModel (state ());
    QObject::connect (this, SIGNAL(modelChanged ()), m_3DModel, SIGNAL(changed ()));

    ISpy3DView *view = new ISpy3DView (state (), m_3DModel, mainwin);
    view->setFeedbackVisibility (true);
    m_mainWindow->addToolBar (Qt::TopToolBarArea, view->toolBar ());
    mainwin->show ();
    	
    QList <QMdiSubWindow *> winList = m_mainWindow->workspace ()->subWindowList ();
    if (winList.length () > 1)
	m_mainWindow->workspace ()->tileSubWindows ();
    else
	winList.first ()->showMaximized ();    

    QObject::connect (this, SIGNAL(save ()), view, SLOT(save ()));
    m_mainWindow->actionSave->setEnabled (true);
    QObject::connect (this, SIGNAL(print ()), view, SLOT(print ()));
    m_mainWindow->actionPrint->setEnabled (true);
    new ISpyRecoContent (state ());

    if (m_archive == 0)
    {
	if (args->args () == 2)
	{
	    std::string fileName (args->argv ()[1]);
	    if (fileName.size () > 2 && fileName.substr (fileName.size () - 3) == ".ig")
	    {
		loadFile (fileName.c_str ());
		m_mainWindow->actionNext->setEnabled (true);
		m_mainWindow->actionAuto->setEnabled (true);
	    }
	}
	else if (! m_fileNames.isEmpty ()) 
	{
	    foreach (QString str, m_fileNames) 
		{
		    open (str);
		}
	    m_fileNames.clear ();
	    QObject::disconnect (&filter, SIGNAL(open (const QString &)), this, SLOT(openFile (const QString &)));
	    QObject::connect (&filter, SIGNAL(open (const QString &)), this, SLOT(open (const QString &)));
	} 
	else 
	{
	    // QObject::connect (m_splash, SIGNAL(splashDone()), this, SLOT(openFileDialog ()));
	    openFileDialog ();	    
	}
    }
	
    QObject::connect(m_mainWindow->actionQuit, SIGNAL(triggered()), this, SLOT(onExit()));
    QObject::connect(m_mainWindow->actionClose, SIGNAL(triggered()), this, SLOT(onExit()));
    //    QObject::connect(m_mainWindow->actionConnect, SIGNAL(triggered()), this, SLOT(connect()));
    //    m_mainWindow->actionConnect->setEnabled (true);
    
    QObject::connect(this, SIGNAL(showMessage (const QString &)), m_mainWindow->statusBar (), SLOT(showMessage (const QString &)));

    SoQt::mainLoop();
  
    delete view;
    
    return 0;
}



void
ISpyApplication::cleanSplash (void)
{
    ASSERT (m_splash);
    delete m_splash;
}



void
ISpyApplication::setupMainWindow (void) 
{
    m_splash = new ISpySplashScreen;
    m_splash->show ();
    QTimer::singleShot (6000, this, SLOT(cleanSplash()));
    
    m_mainWindow = new ISpyMainWindow;

    QObject::connect (m_mainWindow, SIGNAL(open()),          this, SLOT(openFileDialog()));
    QObject::connect (m_mainWindow, SIGNAL(autoEvents()),    this, SLOT(autoEvents()));
    QObject::connect (m_mainWindow, SIGNAL(nextEvent()),     this, SLOT(nextEvent()));
    QObject::connect (m_mainWindow, SIGNAL(previousEvent()), this, SLOT(previousEvent()));
    QObject::connect (m_mainWindow, SIGNAL(rewind()),        this, SLOT(rewind()));
    QObject::connect (m_mainWindow, SIGNAL(print()),         this, SIGNAL(print ()));
    QObject::connect (m_mainWindow, SIGNAL(save()),          this, SIGNAL(save ()));
    
    m_mainWindow->actionNext->setEnabled (false);
    m_mainWindow->actionPrevious->setEnabled (false);
    m_mainWindow->treeView->setModel (m_storageModel);
    m_mainWindow->treeView->hide ();
    
    m_treeWidget = new QTreeWidget(m_mainWindow->dockTreeWidgetContents);
    QStringList headers;
    headers << "Collection" << "Size" << "Visibility";
	
    m_treeWidget->setHeaderLabels (headers);
    m_treeWidget->setColumnWidth (1, 50);
    m_treeWidget->setColumnWidth (2, 30);
    m_treeWidget->setAlternatingRowColors (true);
    m_mainWindow->gridLayout_3->addWidget(m_treeWidget);

    m_mainWindow->restoreSettings ();
    m_mainWindow->show ();
    
    QObject::connect (m_treeWidget, SIGNAL(clicked(const QModelIndex)),this,SLOT(twigChanged(const QModelIndex)));
}



void 
ISpyApplication::twigChanged (const QModelIndex &index)
{
    QSettings settings;

    QString collectionName = m_treeWidget->currentItem ()->text (0);
    int flag = m_treeWidget->currentItem ()->checkState (2);
    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append (collectionName);
    if (settings.contains (visSettings))
    {
	if (settings.value (visSettings).value<int> () != flag)
	{
	    settings.setValue (visSettings, flag);
	
	    ISpyEventMessage event;
	    event.message = visSettings.toStdString ();
	    ISpyReadService* readService = ISpyReadService::get (state ());
	    ASSERT (readService);	
	    readService->dispatcher (ISpyReadService::NewEvent)->broadcast (event);
	}
    }
    else
    {
	settings.setValue (visSettings, flag);
    }
    
    if (m_storage != 0 && m_storage->hasCollection (collectionName.toAscii()))
    {
	ASSERT (m_storage);
	
	if (m_collectionModel == 0)
	    createCollectionModel (m_storage);
	
	m_collectionModel->setCollection (m_storage->getCollectionPtr(collectionName.toAscii()));
	displayTwigCollection (m_storage);    
    }
    else if (m_geomStorage != 0 && m_geomStorage->hasCollection (collectionName.toAscii()))
    {
	ASSERT (m_geomStorage);

	if (m_collectionModel == 0)
	    createCollectionModel (m_geomStorage);

	m_collectionModel->setCollection (m_geomStorage->getCollectionPtr(collectionName.toAscii()));
	displayTwigCollection (m_geomStorage);    
    }
}



void
ISpyApplication::createCollectionModel (IgDataStorage *storage)
{
    ASSERT (storage);
    m_collectionModel = new IgCollectionTableModel (&(storage->getCollectionByIndex (1)));
    m_mainWindow->tableView->setModel (m_collectionModel);
}

void 
ISpyApplication::displayTwigCollection (IgDataStorage *storage)
{
    ASSERT (storage);
    QString collectionName = m_treeWidget->currentItem ()->text (0);
    
    IgCollection *collection = storage->getCollectionPtr(collectionName.toAscii());
    
    if (collection != NULL && collection->size () > 0)
    {
	drawCollection (collection);
    }
}
	
void 
ISpyApplication::displayCollection(const QModelIndex &index)
{
    QString collectionName = m_storageModel->data(index, Qt::DisplayRole).toString();
    
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

  std::cout << "LT debug -- ISpyApplication::displayItem invoked" << std::endl;


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
	// 	qDebug() << index.row () << " selected";
	cit += index.row ();
	IgCollectionItem vtxShape = *cit;
	if (collection->hasProperty ("detid"))
	{
	    // 	    int detid = vtxShape.get<int> ("detid");	    
	    // 	    qDebug() << detid << " detid";
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
ISpyApplication::cleanSelection (void) 
{
    qDebug () << "ISpyApplication::cleanSelection is not implemented yet.\n";
//     if (ISpySelectionService *sel = ISpySelectionService::get (state ()))
//     {
// 	SoSeparator *selSep = dynamic_cast<SoSeparator *>(sel->selection ());	
// 	ASSERT (selSep);
// 	selSep->removeAllChildren ();
//     }    
}



void
ISpyApplication::drawCollection (IgCollection *collection) 
{

    std::string collName = collection->name ();

    std::cout << "LT debug -- ISpyApplication::drawCollection invoked for collection = " << collName << std::endl;


    // We assume these reps will not change and
    // we exclude event data collections for now
    // which are represented by twig classes
    if (collName == "Tracker_V1" ||
	collName == "EcalBarrel_V1" ||
	collName == "EcalEndcap_V1" ||
	collName == "EcalPreshower_V1" ||
	collName == "HcalBarrel_V1" ||
	collName == "HcalEndcap_V1" ||
	collName == "HcalOuter_V1" ||
	collName == "HcalForward_V1" ||
	collName == "DTs_V1" ||
	collName == "CSC_V1" ||
	collName == "RPC_V1" )

      // 	collName == "HBRecHits_V1" ||
      //	collName == "HERecHits_V1" ||
      //	collName == "HFRecHits_V1" ||
      //	collName == "HORecHits_V1" ||

// 	collName == "Event_V1" ||
// 	collName == "BasicClusters_V1" ||
// 	collName == "Tracks_V1" ||
// 	collName == "Extras_V1" ||
// 	collName == "Hits_V1" ||
// 	collName == "TrackingRecHits_V1" ||
// 	collName == "Errors_V1" ||
// 	collName == "CSCSegments_V1" ||
// 	collName == "CaloTowers_V1" ||
// 	collName == "DTRecSegment4D_V1" ||
// 	collName == "EcalRecHits_V1" ||
// 	collName == "METs_V1" ||
// 	collName == "Muons_V1" ||
// 	collName == "Points_V1" ||
// 	collName == "DetIds_V1" ||
// 	collName == "SiPixelCluster_V1" ||
// 	collName == "SiStripClusters_V1" ||
// 	collName == "SiStripDigis_V1" ||
// 	collName == "Jets_V1" ||
// 	collName == "EBRecHits_V1" ||
// 	collName == "EERecHits_V1" ||
// 	collName == "CaloHits_V1" ||
// 	collName == "DTDigis_V1" ||
// 	collName == "DTRecHits_V1")
    {
	CollDrawMap::const_iterator i = m_collDraw.find (collName);

	QSettings settings;
	QString visSettings ("igtwigs/visibility/");
	visSettings.append (QString::fromStdString (collName));
	if (settings.contains (visSettings) && 
	    Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	{
	    // Hide it
	    if (i == m_collDraw.end ())
		return;
	
	    (i->second)->whichChild = SO_SWITCH_NONE;

	    qDebug() << "Hide " << collName.c_str ();
	    return;
	}

	bool hlrMode = false;
	if (settings.contains ("igdisplay/detector/view3d/hiddenlineremoval"))
	{
	    hlrMode = settings.value ("igdisplay/detector/view3d/hiddenlineremoval").value<bool> ();
	}

	SoSeparator *topSep = dynamic_cast<SoSeparator *>(ISpySceneGraphService::get (state ())->sceneGraph ());
	ASSERT (topSep);

	SoSwitch *sep;
	
	if (i == m_collDraw.end ())
	{
	    sep = new SoSwitch;
	    topSep->addChild (sep);
	    
	    sep->setName (collName.c_str ());
	    SoGroup *selSep = new SoGroup;
	    sep->addChild (selSep);

	    m_collDraw.insert (CollDrawMap::value_type(collName, sep));
	    	
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
	      
	        std::cout << "LT debug: ISpyApplication::drawCollection:   drawing collection = " << collName << std::endl;

		IgDrawFactoryService *drawService = IgDrawFactoryService::get (state ());
		std::string what = "CrystalHits";
		std::string repName = "3D";

		SoMaterial *mat = new SoMaterial;
		mat->diffuseColor.setValue (1.0, 0.0, 0.0);	
		mat->transparency.setValue (0.25);
		selSep->addChild (mat);
 
		SoSeparator *rep = dynamic_cast<SoSeparator *>(drawService->draw (what, state (), collection->name (), repName));
		selSep->addChild (rep);
	    }    
	    else
	    {	
		SoVertexProperty *vertices = new SoVertexProperty;
	    
		int i = 0;
		std::vector<int> lineIndices;
		std::vector<int> indices;
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
			// Face set indices
			indices.push_back (i + 3);
			indices.push_back (i + 2);
			indices.push_back (i + 1);
			indices.push_back (i + 0);
			indices.push_back (SO_END_FACE_INDEX);

			indices.push_back (i + 4);
			indices.push_back (i + 5);
			indices.push_back (i + 6);
			indices.push_back (i + 7);
			indices.push_back (SO_END_FACE_INDEX);
		    
			indices.push_back (i + 5); 
			indices.push_back (i + 1);
			indices.push_back (i + 2);
			indices.push_back (i + 6);   
			indices.push_back (SO_END_FACE_INDEX);

			indices.push_back (i + 2);
			indices.push_back (i + 3);
			indices.push_back (i + 7);
			indices.push_back (i + 6);
			indices.push_back (SO_END_FACE_INDEX);
		    
			indices.push_back (i + 7);
			indices.push_back (i + 3);
			indices.push_back (i);
			indices.push_back (i + 4);
			indices.push_back (SO_END_FACE_INDEX);
		    
			indices.push_back (i + 1);
			indices.push_back (i + 5);
			indices.push_back (i + 4);
			indices.push_back (i);
			indices.push_back (SO_END_FACE_INDEX); // end of crystal vertices: 6*5
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
		    if (hlrMode)
		    {		    
			SoSeparator *top = new SoSeparator;
			selSep->addChild (top);
		
			SoSeparator *local = new SoSeparator;
			top->addChild (local);
			SoPolygonOffset *offset = new SoPolygonOffset;
			local->addChild (offset);
		
			SoMaterial *black = new SoMaterial;
			black->ambientColor.setValue (0.0, 0.0, 0.0);
			black->diffuseColor.setValue (0.0, 0.0, 0.0);
			black->specularColor.setValue (0.0, 0.0, 0.0);
			black->emissiveColor.setValue (0.0, 0.0, 0.0);
			local->addChild (black);

			SoIndexedFaceSet *faces = new SoIndexedFaceSet;
			faces->coordIndex.setValues (0, indices.size (), &indices [0]);
			faces->vertexProperty = vertices;
			local->addChild (faces);
		
			SoDrawStyle *style = new SoDrawStyle;
			style->style = SoDrawStyle::LINES;
			top->addChild (style);
			SoMaterial *grey = new SoMaterial;
			grey->ambientColor.setValue (0.2, 0.2, 0.2);
			grey->diffuseColor.setValue (0.2, 0.2, 0.2);
			grey->specularColor.setValue (0.2, 0.2, 0.2);
			grey->emissiveColor.setValue (0.2, 0.2, 0.2);
			top->addChild (grey);
			SoMaterialBinding *bind = new SoMaterialBinding;
			bind->value = SoMaterialBindingElement::OVERALL;
			top->addChild (bind);		
			top->addChild (faces);
		    }
		    else
		    {		    
			SoIndexedLineSet *lineSet = new SoIndexedLineSet;
			lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
			lineSet->vertexProperty = vertices;
			selSep->addChild (lineSet);		    
		    }
		}
		else if (shape == "point")
		{
		    SoPointSet *pointSet = new SoPointSet;
		    pointSet->vertexProperty.setValue (vertices);
		    pointSet->numPoints.setValue (corners.size ());

		    selSep->addChild (pointSet);
		}
	    }
	    sep->whichChild = SO_SWITCH_ALL;
	    return;	    
	}
	(i->second)->whichChild = SO_SWITCH_ALL;
    }
}




void
ISpyApplication::openFileDialog (void) 
{
    QString fileName;
    QFileDialog f (m_mainWindow->centralwidget, tr("Open File"), ".",
		   tr("Ig Files (*.ig *.iguana)"));
    f.setFileMode(QFileDialog::ExistingFiles);
    QList<QUrl> shortcuts = f.sidebarUrls ();
    
    QSettings settings;
    QUrl url = settings.value ("igfiles/home").value<QUrl> ();
    shortcuts.append (url);
    QUrl url1 ("file:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/EventDisplay");
    shortcuts.append (url1);
    f.setSidebarUrls (shortcuts);

    if (f.exec ())
    {
	fileName = f.selectedFiles ().front ();
	open (fileName);
    }
}

void
ISpyApplication::open (const QString &fileName) 
{
    qDebug () << "Try to open " << fileName;
    
    // We always keep geometry 
    if (! fileName.isEmpty ())
    {
	showMessage(QString("Opening ") + fileName + tr("..."));

	QSettings settings;
	QString geomFileName = settings.value ("igfiles/geometry", "default-geometry.ig").value<QString> ();
	qDebug () << "Geometry? " << geomFileName;
	if (fileName.contains (geomFileName, Qt::CaseInsensitive))
	{
	    qDebug () << "Yep!";

	    if (m_geomArchive == 0)
	    {
		qDebug () << "First time!";
	    
		//closeGeomFile ();	    
		loadGeomFile (fileName);
	    }
	    else
		qDebug () << "Ignore for now!!!";
	}
	else
	{    
	    if (m_archive)
	    {
		closeFile ();
	    }
	
	    loadFile (fileName);
	    if (m_mainWindow != 0)
	    {
		m_mainWindow->actionNext->setEnabled (true);
		m_mainWindow->actionAuto->setEnabled (true);
		m_mainWindow->actionPrevious->setEnabled (false);
	    }
	}	
    }
    else 
    {	
	if (m_mainWindow != 0)
	{
	    m_mainWindow->actionNext->setEnabled (false);
	    m_mainWindow->actionAuto->setEnabled (false);
	    m_mainWindow->actionPrevious->setEnabled (false);
	}	
    }   
}

void
ISpyApplication::openFile (const QString &fileName) 
{
    qDebug () << "Open file on double click " << fileName;
    m_fileNames <<  fileName;
    open (fileName);
}

void
ISpyApplication::connect (void)
{
    ISpyReadService* readService = ISpyReadService::get (state ());
    ASSERT (readService);
    readService->preEventProcessing ();
    m_storage = readService->dataStorage ();

    m_consumer.nextEvent (m_storage);
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
    modelChanged ();

    if (++m_it != m_archive->end ())
    {
	ISpyEventMessage event;
	event.message = (*m_it)->name ();
	
	showMessage (QString ((*m_it)->name ()));
	
	readZipMember (m_it);
	m_mainWindow->actionPrevious->setEnabled (true);
	//	m_model->setData (m_model->index (0, 0),  QVariant ("CMS Event"), Qt::EditRole);

	ISpyReadService* readService = ISpyReadService::get (state ());
	ASSERT (readService);
    
	readService->dispatcher (ISpyReadService::NewEvent)->broadcast (event);
    }
    else
    {
	lastEvent ();
		
	m_mainWindow->actionAuto->setChecked (false);	
	m_mainWindow->actionAuto->setEnabled (false);
	m_timer->stop ();
        m_timer->disconnect ();
		
	m_mainWindow->actionNext->setEnabled (false);
    }
}

void
ISpyApplication::previousEvent (void)
{    
    modelChanged ();

    if (--m_it != m_archive->begin ()
	|| (m_it == m_archive->begin () 
	    && (*m_it)->name () != "Header"))
    {
	ISpyEventMessage event;
	event.message = (*m_it)->name ();

	showMessage (QString ((*m_it)->name ()));

	readZipMember (m_it);
	m_mainWindow->actionNext->setEnabled (true);
	m_mainWindow->actionAuto->setEnabled (true);
	if (m_it == m_archive->begin ())
	    m_mainWindow->actionPrevious->setEnabled (false);

	ISpyReadService* readService = ISpyReadService::get (state ());
	ASSERT (readService);
	
	readService->dispatcher (ISpyReadService::NewEvent)->broadcast (event);
    }
    else 
    {
	m_mainWindow->actionPrevious->setEnabled (false);
    }
}

void
ISpyApplication::rewind (void)
{    
    m_it = m_archive->begin ();
    
    showMessage (QString ((*m_it)->name ()));
    nextEvent ();
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
ISpyApplication::newMember (const QString &name)
{
    qDebug () << QString("Read in ") << name << tr(".");
    initTreeItems (m_geomStorage);
}

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
    m_treeWidget->clear ();
    m_items.clear ();

    if (m_geomArchive != 0 && (! m_onExit))
    {
 	ASSERT (m_geomStorage);
 	updateTreeItems (m_geomStorage);
    }
    
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
    if (m_collectionModel != 0)
    {
	delete m_collectionModel;
	m_collectionModel = 0;	
    }
    ASSERT (m_storageModel);
    m_storageModel->clear ();
    m_treeWidget->clear ();
    m_items.clear ();
    if (m_archive != 0 && (! m_onExit))
    {
 	ASSERT (m_storage);
 	updateTreeItems (m_storage);
    }
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
	showMessage(QString("Loading ") + filename + tr("..."));
	m_archive = new ZipArchive (inputFilename, IOFlags::OpenRead);
	m_it = m_archive->begin ();
	parseZipMember (m_it);
    }
    else
	std::cout << "Event file " << inputFilename << " does not exist." << std::endl;
}

void
ISpyApplication::parseZipMember (lat::ZipArchive::Iterator it)
{
    if ((*it)->name () == "Header") 
    {
	showMessage (QString ((*it)->name ()));
	nextEvent ();
    }
    else // Assume if ((*it)->name () == "Event")
    {	    
	ISpyEventMessage event;
	event.message = (*it)->name ();
	
	showMessage (QString ((*it)->name ()));
	
	readZipMember (it);
	
	m_mainWindow->actionPrevious->setEnabled (true);
	
	ISpyReadService* readService = ISpyReadService::get (state ());
	ASSERT (readService);
    
	readService->dispatcher (ISpyReadService::NewEvent)->broadcast (event);
    } 
}

void
ISpyApplication::initTreeItems (IgDataStorage *storage)
{
    QSettings settings;
    QList<QTreeWidgetItem *> items;    

    for (IgDataStorage::CollectionNames::iterator it = storage->collectionNames ().begin (), 
						 end = storage->collectionNames ().end (); 
	 it < end; ++it)
    {
	IgCollection &coll = storage->getCollection ((*it).c_str ());
	QString fullCollName ((*it).c_str ());
	
 	qDebug () << QString("Initializing ") << fullCollName << tr("...");

	QStringList nameList;	
	nameList = fullCollName.split (QString ("/"));
	
	// Count the items in the collection 
	int size = 0;
	IgCollectionIterator cit = coll.begin ();
	IgCollectionIterator cend = coll.end ();	
	for (; cit != cend ; cit++, ++size) 
	{}
	QStringList collName;
	collName << nameList [0] << QString ("%1").arg (size);
	QTreeWidgetItem *treeItem = new QTreeWidgetItem((QTreeWidget*)0, collName);
	QString visSettings ("igtwigs/visibility/");
	visSettings.append (nameList [0]);
	if (settings.contains (visSettings))
	    treeItem->setCheckState (2, Qt::CheckState (settings.value (visSettings).value<int> ()));
	else
	{
	    settings.setValue (visSettings, 2);
	    treeItem->setCheckState (2, Qt::Checked);
	}
	
	items.append (treeItem);
	if (treeItem->checkState (2) == Qt::Checked)
	{	    
	    qDebug () << "Draw " << nameList [0];
	    drawCollection (&coll);
	}	
    }
    qDebug () << "Before " << m_items;
    qDebug () << " add " << items;

    m_treeWidget->insertTopLevelItems (0, items);
    m_items.append (items);

    qDebug () << "After " << m_items;
}

void
ISpyApplication::updateTreeItems (IgDataStorage *storage)
{
    QSettings settings;

    for (IgDataStorage::CollectionNames::iterator it = storage->collectionNames ().begin (), 
						 end = storage->collectionNames ().end (); 
	 it < end; ++it)
    {
	IgCollection &coll = storage->getCollection ((*it).c_str ());
	QString fullCollName ((*it).c_str ());
	QStringList nameList;	
	nameList = fullCollName.split (QString ("/"));
	
	// Count the items in the collection 
	int size = 0;
	IgCollectionIterator cit = coll.begin ();
	IgCollectionIterator cend = coll.end ();	
	for (; cit != cend ; cit++, ++size) 
	{}
	QStringList collName;
	QTreeWidgetItemIterator twig (m_treeWidget);
	
	bool found = false;
	
	while (*twig) 
	{
	    if ((*twig)->text (0) == nameList [0])
	    {
		found = true;		
		(*twig)->setText (1, QString ("%1").arg (size));
	    }
	    
	    ++twig;
	}
	qDebug () << nameList [0] << ": " << found;
	if (! found)
	{
	    collName << nameList [0] << QString ("%1").arg (size);
	    QTreeWidgetItem *treeItem = new QTreeWidgetItem((QTreeWidget*)0, collName);
	    QString visSettings ("igtwigs/visibility/");
	    visSettings.append (nameList [0]);
	    if (settings.contains (visSettings))
		treeItem->setCheckState (2, Qt::CheckState (settings.value (visSettings).value<int> ()));
	    else
	    {
		settings.setValue (visSettings, 2);
		treeItem->setCheckState (2, Qt::Checked);
	    }
	    
	    m_items.append (treeItem);
	}
    }
    m_treeWidget->insertTopLevelItems (0, m_items);
}

void
ISpyApplication::loadGeomFile (const QString &filename)
{
    Filename inputFilename (Filename (filename.toStdString ()).substitute (ShellEnvironment ()));
    if (inputFilename.exists())
    {
	showMessage(QString("Loading ") + filename + tr("..."));
	m_geomArchive = new ZipArchive (inputFilename, IOFlags::OpenRead);
	m_geomIt = m_geomArchive->begin ();
	// readGeomZipMember (m_geomIt);

	ISpyReadService* readService = ISpyReadService::get (state ());
	ASSERT (readService);
	readService->preESProcessing ();
	m_geomStorage = readService->esStorage ();

	m_readThread.read (m_geomArchive, m_geomIt, m_geomStorage);
    }
    else
    {
	qDebug () << "Geometry file " << inputFilename << " does not exist.\n";
    }
}

void
ISpyApplication::readZipMember (lat::ZipArchive::Iterator i)
{
    qDebug () << QString("Reading ") << (*i)->name () << tr("...");
    showMessage(QString("Reading ") + (*i)->name () + tr("..."));

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
	    ASSERT (m_geomStorage);	   
	    m_storageModel->addStorage (m_geomStorage, (*m_geomIt)->name ().name ());
	}
	ASSERT (m_storage);
	    
	if (m_items.isEmpty ())
	    initTreeItems (m_storage);
	else
	    updateTreeItems (m_storage);

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
    showMessage (tr((*i)->name ()));
}

void
ISpyApplication::readGeomZipMember (lat::ZipArchive::Iterator i)
{
    showMessage(QString("Reading GeomZipMember") + (*i)->name () + tr("..."));

    ISpyReadService* readService = ISpyReadService::get (state ());
    ASSERT (readService);
    readService->preESProcessing ();

    ASSERT (m_geomArchive);
    InputStream *is = m_geomArchive->input (*i);
    
    IOSize length = (*i)->size (ZipMember::UNCOMPRESSED);
    std::vector<char> buffer (length + 1, 0);
    is->xread (&buffer[0], length);
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
    showMessage (tr((*i)->name ()));
}
