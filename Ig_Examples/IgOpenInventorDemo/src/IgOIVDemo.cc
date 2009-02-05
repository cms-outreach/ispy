//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Examples/IgOpenInventorDemo/interface/IgOIVDemo.h"
#include "Ig_Extensions/IgIcons/interface/fileprint.xpm"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppWindowService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgEnvsElement.h"
#include <qapplication.h>
#include <qbuttongroup.h>
#include <qfile.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qprinter.h>
#include <qprocess.h>
#include <qradiobutton.h>
#include <qstatusbar.h>
#include <qstring.h>
#include <qsplitter.h>
#include <qtextedit.h>
#include <qtextstream.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qlistview.h>
// #include <fstream>
// #include <iostream>
// #include <sys/types.h> 
// #include <dirent.h> 
// #include <sys/stat.h>
// #include <signal.h>
// #include <unistd.h>
// #include <sys/wait.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOIVDemo::IgOIVDemo (IgState *state)
    : m_state(state),
      m_proc (0),
      m_printer (new QPrinter),
      m_pid (0)
{       
    ASSERT (state);

    QPixmap printIcon;
    
    // Main window from the studio.
    m_mainWindow = static_cast<QMainWindow*> (IgQtAppWindowService::get (state)->mainWindow ());
    ASSERT (m_mainWindow);
    m_mainWindow->setCaption ("IgOpenInventor Demo - Application");
            
    QToolBar * fileTools = new QToolBar (m_mainWindow, "file operations");
    fileTools->setLabel ("File Operations");
    
    printIcon = QPixmap (fileprint);
    QToolButton * filePrint
	    = new QToolButton (printIcon, "Print File", 
	                       QString::null,
			       this, SLOT (print ()), 
			       fileTools, "print file");

    (void) QWhatsThis::whatsThisButton (fileTools);

    const char * filePrintText = "Click this button to print the file you "
				 "are editing.\nYou can also select the Print "
				 "command from the File menu.";

    QWhatsThis::add (filePrint, filePrintText);
    
    ASSERT (IgQtAppMenuService::get (state));    
    QPopupMenu *file = IgQtAppMenuService::get (state)->subMenu
		       (IgQtAppMenuService::MENU_ID_FILE);

    int id;

    id = file->insertItem (printIcon, "&Print...",
			   this, SLOT (print ()), CTRL + Key_P, -1, 0);
    file->setWhatsThis (id, filePrintText);

    file->setWhatsThis (id, 
                        "Click this menu item to quit from the application.\n"
			"You can also click on the \"x\" button on the right "
			"top corner to quit");

    m_mainWindow->menuBar ()->insertSeparator ();

    m_mainWindow->setCentralWidget (new QWidget (m_mainWindow, "qt_central_widget"));
    QGridLayout *flayout = new QGridLayout (m_mainWindow->centralWidget (), 1, 1, 1, 1, "FrameLayout");

    m_buttonGroup = new QButtonGroup (m_mainWindow->centralWidget (), "buttonGroup");
    m_buttonGroup->setColumnLayout (0, Qt::Vertical);
    m_buttonGroup->layout ()->setSpacing (0);
    m_buttonGroup->layout ()->setMargin (0);
    m_buttonGroup->setTitle (QString::null);

    QGridLayout *buttonGroupLayout = new QGridLayout (m_buttonGroup->layout ());
    buttonGroupLayout->setAlignment (Qt::AlignTop);

    QRadioButton *radioButton2 = new QRadioButton (m_buttonGroup, "radioButton2");
    radioButton2->setChecked (true);
    radioButton2->setText (tr ("Release: " + getIGUANATop(0)));

    buttonGroupLayout->addWidget (radioButton2, 0, 0);

    QRadioButton *radioButton1 = new QRadioButton (m_buttonGroup, "radioButton1");
    radioButton1->setChecked (false);
    radioButton1->setText (tr ("Local: " + getIGUANATop(1)));

    buttonGroupLayout->addWidget (radioButton1, 1, 0);
    
    m_demo = new QListView (m_buttonGroup, "IgOIVDemoList");
    id = m_demo->addColumn ("IgOpenInventor Tests");
    m_demo->setColumnWidthMode (id, QListView::Maximum);
    QWhatsThis::add (m_demo, "List of all IgOpenInventor tests available."
		     " Click on any demo to run.");
	    
    m_flist = new QListView (m_buttonGroup, "IgOIVDemoFileList");
    id = m_flist->addColumn ("File List");    
    m_flist ->setColumnWidthMode (id, QListView::Maximum);
    QWhatsThis::add (m_flist, "List of all source files for IgOpenInventor tests selected in "
		     "the demo list.\nClick on file name to see its "
		     "source in the source text window");    

    QHBoxLayout *layout1 = new QHBoxLayout (0, 0, 3, "layout1");
    layout1->addWidget (m_demo);    
    layout1->addWidget (m_flist);    
    buttonGroupLayout->addLayout (layout1, 2, 0);

    QSplitter *splitter = new QSplitter (QSplitter::Vertical, m_mainWindow->centralWidget());
    
    m_file = new QTextEdit (splitter, "FileEditor");
    m_file->setReadOnly (true);
    m_file->setMinimumSize (QSize (500, 100));
    QWhatsThis::add (m_file, "Text window to show the source file of IgOpenInventor tests.");

    flayout->addWidget (m_buttonGroup, 0, 0);
    flayout->addWidget (splitter, 0, 1);

    m_demo->setFocus ();

    QObject::connect (m_demo, SIGNAL (selectionChanged (QListViewItem *)),
                      this, SLOT (demoChanged (QListViewItem *)));
    QObject::connect (m_flist, SIGNAL (selectionChanged (QListViewItem *)),
                      this, SLOT (flistChanged (QListViewItem *)));
    QObject::connect (m_buttonGroup, SIGNAL (clicked (int)),
                      this, SLOT (dirChanged ()));
    QObject::connect (qApp, SIGNAL (aboutToQuit ()),
		      this, SLOT (cleanup ()));
    
    init (getIGUANATop(0));
    m_mainWindow->statusBar ()->message ("Ready", 2000);
    
    m_mainWindow->resize (1000, 760);
    m_mainWindow->centralWidget()->show ();
}

IgOIVDemo::~IgOIVDemo ()
{
    terminateChild ();
    delete m_printer;
    delete m_demo;
    delete m_flist;
    delete m_file;
}

void 
IgOIVDemo::cleanup () 
{
    terminateChild ();
}

void 
IgOIVDemo::init (const QString &name)
{
    QDir testDir (name + "/tests/bin/slc3_ia32_gcc323/");
    testDir.setNameFilter ("test_*");
    const QFileInfoList *files = testDir.entryInfoList ();
    if (files) 
    {
	QFileInfoListIterator it (*files);
	QFileInfo *fi;
	while (( fi = it.current ()) != 0) 
	{
	    ++it;
	    checkDemoItem (name, fi->fileName ());
	}
    }
}

void
IgOIVDemo::checkDemoItem (const QDir &/*dir*/, const QString& file)
{
    QListViewItem *item1 = new QListViewItem (m_demo);
    item1->setText (0, file);
}

QString
IgOIVDemo::getIGUANATop (unsigned int id /* = 0 */)
{
    QString dir ("");
    
    IgEnvsElement* envs = IgEnvsElement::get(m_state);
    std::vector<std::string> iguanaPath;
    if (envs->getEnv("IGUANA_PATH", iguanaPath))
    {
        if (iguanaPath.size() == 1)
	    dir = iguanaPath[0];
	else if(iguanaPath.size() > 1)
	{
	    if (id == 0)
	        dir = iguanaPath[iguanaPath.size()-1];
            else
	        dir = iguanaPath[0];
	}
    }
    return dir;
}

void 
IgOIVDemo::demoChanged (QListViewItem *item)
{
    m_flist->clear ();
    if (!m_demo->isSelected (item)) item = m_demo->selectedItem ();
    
    QString name = item->text (0);
    QString dirName;
    
    m_buttonGroup->selectedId () == 0 ? (dirName = getIGUANATop (0)): (dirName = getIGUANATop (1));
    QDir dir (dirName + "/src/Ig_Extensions/IgOpenInventor/test");
    dir.setNameFilter ("*.cc *.cpp *.h");

    const QFileInfoList *itemFileList = dir.entryInfoList ();
    
    QFileInfoListIterator fit (*itemFileList);
    QFileInfo *fi;
    while ((fi = fit.current ()) != 0)
    {
	++fit;
 	QListViewItem *item1 = new QListViewItem (m_flist);
 	item1->setText (0, fi->fileName ());
    }
    
    terminateChild ();
    startChild (name);
}

void 
IgOIVDemo::dirChanged (void)
{
    m_demo->clear ();
    m_buttonGroup->selectedId () == 0 ? init (getIGUANATop (0)): init (getIGUANATop (1));
}

void 
IgOIVDemo::terminateChild (void)
{
    if (m_proc != 0) 
    {
	m_proc->tryTerminate ();
    }

    m_pid = 0;
}

void 
IgOIVDemo::startChild (const QString &name)
{
    QString winName = name;
    winName = winName.replace ("test_", "IGUANA: ");
    winName = winName + " Test";
    
    QString aCommand = name;
       
    m_proc = new QProcess (aCommand, this, "test process");
    m_pid = m_proc->processIdentifier ();
    
    if (!m_proc->start ()) 
    {
        // error handling
        QMessageBox::critical( 0,
                tr("Fatal error"),
                tr("Could not start the uic command."),
                tr("Quit") );
        exit( -1 );
    }

    m_mainWindow->statusBar ()->message ("Running:" + name, 5000);
}

void 
IgOIVDemo::flistChanged (QListViewItem *item)
{
    QString name = item->text (0);
    load (name);
    QWhatsThis::remove (m_file);
    QWhatsThis::add (m_file, "Text window to show the source file of OpenInventor Demos.\n"
		     "Currently showing \"" + item->text (0) + "\" file which"
		     " belongs to \"" + "\"  IgOpenInventor Demo"); 
    m_mainWindow->setCaption (QString("IgOpenInventor Demo - ")
			      + " (" + item->text (0) + ")");
}

void 
IgOIVDemo::load (const QString &fileName)
{
    QString dirName;
    m_buttonGroup->selectedId () == 0 ? (dirName = getIGUANATop (0)): (dirName = getIGUANATop (1));
    QDir::setCurrent (dirName + "/src/Ig_Extensions/IgOpenInventor/test");
    QFile f (fileName);

    if (!f.open (IO_ReadOnly))
	return;

    QTextStream ts (&f);
    m_file->setText (ts.read ());
    m_file->setModified (false);
    m_mainWindow->statusBar ()->message ("Loaded document " + fileName, 2000);
}

void 
IgOIVDemo::print ()
{
    const int margin = 10;
    int pageNo = 1;

    if (m_printer->setup (m_mainWindow)) 
    {
	m_mainWindow->statusBar ()->message ("Printing...");
	QPainter p;
	if (!p.begin (m_printer))
	    return;

	p.setFont (m_file->font ());
	int yPos = 0;
	QFontMetrics fm = p.fontMetrics ();
	QPaintDeviceMetrics metrics (m_printer); // need width/height
	                                         // of printer surface
	for (int i = 0 ; i < m_file->lines () ; i++) 
	{
	    if (margin + yPos > metrics.height () - margin) 
	    {
		QString msg ("Printing (page ");
		msg += QString::number (++pageNo);
		msg += ")...";
		m_mainWindow->statusBar ()->message (msg);
		m_printer->newPage ();		// no more room on this page
		yPos = 0;			// back to top of page
	    }
	    p.drawText (margin, margin + yPos,
			metrics.width (), fm.lineSpacing (),
			ExpandTabs | DontClip,
			m_file->text (i));
	    yPos = yPos + fm.lineSpacing ();
	}
	p.end ();				// send job to printer
	m_mainWindow->statusBar ()->message ("Printing completed", 2000);
    } 
    else 
    {
	m_mainWindow->statusBar ()->message ("Printing aborted", 2000);
    }
}
