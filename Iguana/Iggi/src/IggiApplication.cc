//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT
#include "Iguana/Iggi/interface/IggiApplication.h"
#include "Iguana/Iggi/interface/IggiMainWindow.h"
#include "Iguana/Iggi/interface/IggiScene.h"
#include "Iguana/Iggi/interface/IGUANA_SPLASH.xpm"
#include "Iguana/Iggi/interface/IggiTreeModel.h"
#include "Iguana/Iggi/interface/IggiTreeItem.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Framework/interface/IgParser.h"
#include "Iguana/Framework/interface/IgArgsElement.h"
#include "Iguana/Framework/interface/IgEnvsElement.h"
#include "Iguana/Framework/interface/IgPluginLoader.h"
#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/Iggi/interface/IgTrack.h"
#include "Iguana/Iggi/interface/IgHits.h"
#include "Iguana/Iggi/interface/IgRectangle.h"
#include "Iguana/View/interface/IViewReadService.h"
#include "Iguana/View/interface/IViewRecoContent.h"
#include "Iguana/View/interface/IViewQWindowService.h"
#include <QApplication>
#include <QByteArray>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QPixmap>
#include <QPointF>
#include <QPrintDialog>
#include <QPrinter>
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

//<<<<<< PRIVATE DEFINES                                                >>>>>>

lat::logflag LFiggi = { 0, "iggi", true, -1 };

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

using namespace lat;

IggiApplication::IggiApplication (void)
    : m_state (0),
      m_argc (-1),
      m_argv (0),
      m_model (new IggiTreeModel ("CMS Event")),
      m_archive (0),
      m_appname (0)
{}

IggiApplication::~IggiApplication (void)
{}

IgState *
IggiApplication::state (void) const
{ return m_state; }

int
IggiApplication::argc (void) const
{ return m_argc; }

char **
IggiApplication::argv (void) const
{ return m_argv; }

//////////////////////////////////////////////////////////////////////
int
IggiApplication::run (int argc, char *argv[])
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
	    return EXIT_SUCCESS;
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

    return doRun (argc, argv);
}

const char *
IggiApplication::appname (void) const
{
    const char *name = m_appname;
    if (const char *ptr = strrchr (name, '/'))
	name = ptr + 1;
    return name;
}

int
IggiApplication::usage (void)
{
    const char *app = appname ();
    std::cerr << "Usage: " << app << " [OPTION-OR-FILENAME]...\n"
	      << "   or: " << app << " --help\n"
	      << "   or: " << app << " --version\n";

    return EXIT_FAILURE;
}

int
IggiApplication::initState (void)
{
    // FIXME: install other services?
    m_state = new IgState;
    new IgArgsElement (m_state, m_argc, m_argv);
    new IgEnvsElement (m_state);
    new IgPluginLoader (m_state);
    new IViewReadService (m_state);
    new IViewRecoContent (m_state);
    
    return EXIT_SUCCESS;
}

int
IggiApplication::doRun (int argc, char *argv[])
{
    QApplication app (argc, argv);
    setupMainWindow ();
    
    if (argc == 2)
    {
	std::string fileName (argv [0]);
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

    return app.exec ();
}

void
IggiApplication::setupMainWindow (void) 
{
    QSplashScreen *splash = new QSplashScreen(QPixmap (IGUANA_SPLASH));
    splash->show ();

    m_mainWindow = new IggiMainWindow;
    QObject::connect (m_mainWindow->actionFileOpen, SIGNAL(triggered()), this, SLOT(open()));
    QObject::connect (m_mainWindow->actionNext, SIGNAL(triggered()), this, SLOT(nextEvent()));
    QObject::connect (m_mainWindow->actionPrevious, SIGNAL(triggered()), this, SLOT(previousEvent()));
    QObject::connect (m_mainWindow->actionRewind, SIGNAL(triggered()), this, SLOT(rewind()));
    QObject::connect (m_mainWindow->actionPrint, SIGNAL(triggered()), this, SLOT(print ()));
    QObject::connect (m_mainWindow->actionSave, SIGNAL(triggered()), this, SLOT(save ()));
    
    m_mainWindow->actionNext->setEnabled (false);
    m_mainWindow->actionPrevious->setEnabled (false);
    m_mainWindow->actionAuto->setEnabled (false);
    m_mainWindow->treeView->setModel (m_model);
    m_mainWindow->treeView->setSortingEnabled (true);
    m_mainWindow->tableView->setModel (m_model);
    m_mainWindow->tableView->setSelectionModel (m_mainWindow->treeView->selectionModel ());
    m_mainWindow->graphicsView->setInteractive (true);

    m_mainWindow->show ();

    new IViewQWindowService (state (), m_mainWindow);

    delete splash;
}

void
IggiApplication::open (void) 
{
    QString fileName = QFileDialog::getOpenFileName (m_mainWindow->centralwidget,
 						     tr("Open File"), ".", 
 						     tr("Ig Files (*.ig *.iggi *.iguana)"));
    if (! fileName.isEmpty ())
    {
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

void
IggiApplication::nextEvent (void)
{
    if (++m_it != m_archive->end ())
    {
	IViewEventMessage event;
	event.message = (*m_it)->name ();
	
	m_mainWindow->statusBar ()->showMessage (QString ((*m_it)->name ()));
	IggiScene *scene = dynamic_cast<IggiScene*>(m_mainWindow->graphicsView->scene ());
	ASSERT (scene);
	scene->clear ();
	readZipMember (m_it);
	m_mainWindow->actionPrevious->setEnabled (true);
	
	IViewReadService* readService = IViewReadService::get (m_state);
	ASSERT (readService);
    
	readService->dispatcher (IViewReadService::NewEvent)->broadcast (event);		
    }
    else 
	m_mainWindow->actionNext->setEnabled (false);
}

void
IggiApplication::previousEvent (void)
{    
    if (m_it != m_archive->begin ())
    {
	--m_it;
    
	IViewEventMessage event;
	event.message = (*m_it)->name ();

	m_mainWindow->statusBar ()->showMessage (QString ((*m_it)->name ()));
	IggiScene *scene = dynamic_cast<IggiScene*>(m_mainWindow->graphicsView->scene ());
	ASSERT (scene);
	scene->clear ();
	readZipMember (m_it);
	m_mainWindow->actionNext->setEnabled (true);
	
	IViewReadService* readService = IViewReadService::get (m_state);
	ASSERT (readService);
	
	readService->dispatcher (IViewReadService::NewEvent)->broadcast (event);
    }
    else 
	m_mainWindow->actionPrevious->setEnabled (false);
}

void
IggiApplication::rewind (void)
{    
    m_it = m_archive->begin ();
    
    m_mainWindow->statusBar ()->showMessage (QString ((*m_it)->name ()));
    nextEvent ();
}

void
IggiApplication::print (void) 
{
#ifndef QT_NO_PRINTER
    QPrinter printer; 
    QPrintDialog dialog (&printer, m_mainWindow->centralwidget);
    dialog.setWindowTitle (tr("Print To File"));
    if (dialog.exec () == QDialog::Accepted)
    {
	QPainter painter;
	m_mainWindow->graphicsView->scene ()->render (&painter);
	painter.end ();
    }
#endif
}

void
IggiApplication::save (void) 
{
    QPixmap pixmap;
    QPainter painter (&pixmap);
    painter.setRenderHint (QPainter::Antialiasing);
    painter.end ();

    pixmap.save ("scene.png");
}

void
IggiApplication::processEvent (unsigned int runNum, unsigned int eventNum)
{}

void
IggiApplication::processEventDialog (void) 
{}

void
IggiApplication::skipEvent (long num)
{}

void
IggiApplication::skipEventDialog (void)
{}

void
IggiApplication::closeFile (void)
{
    ASSERT (m_model);
    m_model->clear ();
    
    IggiScene *scene = dynamic_cast<IggiScene*>(m_mainWindow->graphicsView->scene ());
    ASSERT (scene);
    scene->clear ();

    ASSERT (m_archive);
    m_archive->close ();
    delete m_archive;
    m_archive = 0;
}

void
IggiApplication::loadFile (const QString &filename)
{
    Filename inputFilename (Filename (filename.toStdString ()).substitute (ShellEnvironment ()));
    m_archive = new ZipArchive (inputFilename, IOFlags::OpenRead);
    m_it = m_archive->begin ();
}

void
IggiApplication::readZipMember (lat::ZipArchive::Iterator i)
{
    IViewReadService* readService = IViewReadService::get (m_state);
    ASSERT (readService);
    readService->preEventProcessing ();

    ASSERT (m_archive);
    InputStream *is = m_archive->input (*i);
    
    IOSize length = (*i)->size (ZipMember::UNCOMPRESSED);
    std::vector<char> buffer(length+1, 0);
    is->xread(&buffer[0], length);
    is->close ();
    delete is;

    IgDataStorage *storage = readService->dataStorage ();
    IgParser parser (storage);
    try
    {
	parser.parse (&buffer[0]);
    }
    catch (ParseError &e)
    {
	std::cout << "**** IguanaApplication ****\n ParseError at char n. " << std::endl;
    }
    catch (lat::Error &e) 
    {
	std::cout << "**** IguanaApplication ****\n lat::Error " << e.explainSelf () << std::endl;
    }
    catch (std::exception &e) 
    {
	std::cout << "**** IguanaApplication ****\n Standard library exception caught: " << e.what () << std::endl;
    }
    catch (...) 
    {	
	std::cout << "**** IguanaApplication ****\n Unknown Exception" << std::endl;
    }
    QByteArray ba (&buffer[0], length);
    QStringList stringList;
    stringList << ba.data ();
    
    // m_model->setupModelData (stringList, m_model->rootItem ());
    if (m_model != 0)
    {
	m_model->clear ();
    }
    m_model->setData (m_model->index (0, 0),  QVariant ("CMS Event"), Qt::EditRole);

    int row = 0;
    
    for (IgDataStorage::CollectionNames::iterator it = storage->collectionNames ().begin (), end = storage->collectionNames ().end (); 
	 it < end; ++it, ++row)
    {
	IgCollection coll = storage->getCollection ((*it).c_str ());
	QString fullCollName ((*it).c_str ());
	QStringList nameList;	
	nameList = fullCollName.split (QString ("/"));
	
	// Count the items in the collection 
	int size = 0;
	IgCollectionIterator cit = coll.begin ();
	IgCollectionIterator cend = coll.end ();	
	for (; cit != cend ; cit++, ++size) 
 	{}
	QString collName = nameList [0] + QString (" [%1]").arg (size);
	m_model->setData (m_model->index (row, 1),  QVariant (collName), Qt::EditRole);
    }
}

// {
//     QByteArray ba (&buffer[0], length);
//     //std::cout << ba.data () << std::endl;
//     QStringList stringList;
//     stringList << ba.data ();
    
//     // m_model->setupModelData (stringList, m_model->rootItem ());
//     if (m_model != 0)
//     {
// 	m_model->clear ();
//     }
//     m_model->setData (m_model->index (0, 0),  QVariant ("CMS Event"), Qt::EditRole);
    
//     IgDataStorage storage;
//     IgParser parser (&storage);
//     try
//     {
// 	parser.parse (ba);
//     }
//     catch (ParseError &e)
//     {
// 	std::cout << "**** IguanaApplication ****\n ParseError at char n. " << std::endl;
//     }
//     catch (lat::Error &e) 
//     {
// 	std::cout << "**** IguanaApplication ****\n lat::Error " << e.explainSelf () << std::endl;
//     }
//     catch (std::exception &e) 
//     {
// 	std::cout << "**** IguanaApplication ****\n Standard library exception caught: " << e.what () << std::endl;
//     }
//     catch (...) 
//     {	
// 	std::cout << "**** IguanaApplication ****\n Unknown Exception" << std::endl;
//     }
    
//     int row = 1;
    
//     for (IgDataStorage::CollectionNames::iterator it = storage.collectionNames ().begin (), end = storage.collectionNames ().end (); 
// 	 it < end; ++it, ++row)
//     {
// 	IgCollection coll = storage.getCollection ((*it).c_str ());
// 	QString fullCollName ((*it).c_str ());
// 	QStringList nameList;	
// 	nameList = fullCollName.split (QString ("/"));
	
// 	// Count the items in the collection 
// 	int size = 0;
// 	IgCollectionIterator cit = coll.begin ();
// 	IgCollectionIterator cend = coll.end ();	
// 	for (; cit != cend ; cit++, ++size) 
//  	{}
// 	QString collName = nameList [0] + QString (" [%1]").arg (size);
// 	//	QModelIndex index = m_model->

// 	m_model->setData (m_model->index (row, 1),  QVariant (collName), Qt::EditRole);

// 	IggiScene *scene = dynamic_cast<IggiScene*>(m_mainWindow->graphicsView->scene ());
// 	ASSERT (scene);

// 	std::cout << "Collection : " << collName.toStdString () << std::endl;
    
    
// 	//IgCollection & myTracks = storage.getCollection("Tracks/V1");
// 	if (nameList [0] == "Tracks")// FIXME:
// // 	for (IgCollectionIterator t = myTracks.begin();
// // 	     t != myTracks.end();
// // 	     t++)
// 	{
// // 	    IgCollectionItem track = *t;
	
// 	    IgCollection &extras = storage.getCollection ("Extras/V1");
// 	    IgCollection &hits = storage.getCollection ("Hits/V1");
// 	    IgAssociationSet &trackExtras = storage.getAssociationSet ("TrackExtras/V1");
// 	    IgAssociationSet &trackHits = storage.getAssociationSet ("TrackHits/V1");
// 	    IgColumnHandle ptHandle = coll.getHandleByLabel ("pt");
// 	    IgColumnHandle xHandle = coll.getHandleByLabel ("x");
// 	    IgColumnHandle yHandle = coll.getHandleByLabel ("y");
// 	    IgColumnHandle zHandle = coll.getHandleByLabel ("z");
// 	    IgColumnHandle pxHandle = coll.getHandleByLabel ("px");
// 	    IgColumnHandle pyHandle = coll.getHandleByLabel ("py");
// 	    IgColumnHandle pzHandle = coll.getHandleByLabel ("pz");
// 	    int n = 2;
// 	    for (cit = coll.begin (); cit != cend ; cit++, ++size, ++n) 
// 	    {
// 		IgCollectionItem itrack = *cit;
// 		QString trackName = QString ("[%1, %2] Track %3 GeV (%4, %5, %6)")
// 				    .arg (row)
// 				    .arg (n)
// 				    .arg (ptHandle.get<double> (n))
// 				    .arg (xHandle.get<double> (n))
// 				    .arg (yHandle.get<double> (n))
// 				    .arg (zHandle.get<double> (n));
// 		std::cout << "Track : " << trackName.toStdString () << std::endl;
	    
// 		m_model->setData (m_model->index (row, n), QVariant (trackName), Qt::EditRole);
	    
// 		std::vector<QPointF> points;
// 		std::vector<QPointF> tangents;
// 		std::vector<QPointF> hitPoints;
// 		// Reference point
// 		{
// 		    points.push_back (QPointF (xHandle.get<double> (n), yHandle.get<double> (n)));
// 		    tangents.push_back (QPointF (pxHandle.get<double> (n) * 10., pyHandle.get<double> (n) * 10.));
// 		}
// 		// Innermost and outermost sates
// 		{
// 		    for (IgAssociationSet::Iterator a = trackExtras.begin ();
// 			 a != trackExtras.end ();
// 			 a++)
// 		    {
// 			if (a->first ().objectId () == itrack.currentRow ())
// 			{
// 			    IgCollectionItem m (&extras, a->second ().objectId ());
// 			    points.push_back (QPointF (m.get<double>("ix"), m.get<double>("iy")));
// 			    tangents.push_back (QPointF (m.get<double>("ipx"), m.get<double>("ipy")));
// 			    points.push_back (QPointF (m.get<double>("ox"), m.get<double>("oy")));
// 			    tangents.push_back (QPointF (m.get<double>("opx"), m.get<double>("opy")));
// 			}
// 		    }
// 		}
// 		// Hits
// 		{
// 		    for (IgAssociationSet::Iterator ha = trackHits.begin ();
// 			 ha != trackHits.end ();
// 			 ha++)
// 		    {
// 			if (ha->first ().objectId () == itrack.currentRow ())
// 			{
// 			    IgCollectionItem hm (&hits, ha->second ().objectId ());
// 			    double x = hm.get<double> ("x");
// 			    double y = hm.get<double> ("y");
// 			    double z = hm.get<double> ("z");
// 			    if (hm.get<double>("x") != 0 && hm.get<double>("y") != 0 && hm.get<double>("z") != 0)
// 			    {				
// 				hitPoints.push_back (QPointF (hm.get<double>("x"), hm.get<double>("y")));
// 				//tangents.push_back (QPointF (hm.get<double>("px"), hm.get<double>("py")));
// 				// std::cout << hm.get<double>("x") << ", " << hm.get<double>("y") << std::endl;

// 				QString hitName = QString ("[%1, %2] Hit (%3, %4, %5)")
// 						    .arg (row)
// 						    .arg (n)
// 						    .arg (x)
// 						    .arg (y)
// 						    .arg (z);
// 				std::cout << "Hit : " << hitName.toStdString () << std::endl;
// 				m_model->setData (m_model->index (1, 1), QVariant (hitName), Qt::EditRole);
// 			    }
// 			}
// 		    }
// 		}

// 		QGraphicsItemGroup* group = new QGraphicsItemGroup;
// 		group->setAcceptHoverEvents (true);
		
// 		IgTrack* track = new IgTrack (points, tangents, IgTrack::SPLINE);
// 		track->setBoundingRegionGranularity (1);
// 		// track->showTangents (true);		
// 		// track->showBBox (true);		
// 		track->setToolTip (trackName);
// 		scene->addItem (track);

//  		QPen pen;
// 		// 		pen.setBrush (Qt::blue);
// 		// 		pen.setWidth (1);
// 		// 		track->setPen (pen);
		
// 		// 		group->addToGroup (track);

// 		IgHits* hits = new IgHits (hitPoints);
// 		pen.setBrush (Qt::red);
// 		pen.setWidth (1);
// 		hits->setPen (pen);		
// 		scene->addItem (hits);

//  		group->addToGroup (hits);
//  		scene->addItem (group);
// 		scene->update ();
		
// 		//std::cout << " " << ptHandle.get<double> (n++) <<  " GeV" << std::endl;
// 	    }
// 	} 
// 	else if (nameList [0] == "Hits")
// 	{
// 	    IgCollection &hits = storage.getCollection ("Hits/V1");
// 	    IgColumnHandle xHandle = hits.getHandleByLabel ("x");
// 	    IgColumnHandle yHandle = hits.getHandleByLabel ("y");
// 	    IgColumnHandle zHandle = hits.getHandleByLabel ("z");
// 	    std::vector<QPointF> points;
// 	    int n = 0;
// 	    for (cit = coll.begin (); cit != cend ; cit++, ++size, ++n) 
// 	    {
// 		points.push_back (QPointF (xHandle.get<double> (n), yHandle.get<double> (n)));
// 	    }
// 	    IgHits* hitColl = new IgHits (points);
// 	    QPen pen;
// 	    pen.setBrush (Qt::green);
// 	    pen.setWidth (2);
// 	    hitColl->setPen (pen);		
		
// 	    scene->addItem (hitColl);
// 	    scene->update ();
// 	}	
// 	QPointF center (0,0);
// 	IgRectangle* rect = new IgRectangle (50, 50, center);
// 	scene->addItem (rect);
// 	scene->update ();
	
// 	//std::cout << "Collection : " << (*it) << std::endl;

// 	std::vector<std::string> check;
// 	std::vector<IgCollection::LabelColumn> &labels = coll.columnLabels ();
// 	for(std::vector<IgCollection::LabelColumn>::iterator i = labels.begin ();
// 	    i != labels.end ();
// 	    i++)
// 	{
// 	    check.push_back (i->first);
// 	    //std::cout << i->first << std::endl;	
// 	}
// 	// 	for (IgCollectionIterator cit = coll.begin (), cend = coll.end (); cit != cend ; cit++) 
// 	// 	{
// 	// 	    std::string name = (*cit).get<std::string> ("name");
// 	// 	    std::cout << name << std::endl;
// 	// 	    m_model->setData (m_model->index (1, 1),  QVariant (name.c_str ()), Qt::EditRole);
// 	// 	}
//     }
// }
   





