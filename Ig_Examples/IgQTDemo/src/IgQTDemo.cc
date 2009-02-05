#include "Ig_Examples/IgQTDemo/interface/IgQTDemo.h"
#include "Ig_Examples/IgQTDemo/interface/fileprint.xpm"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppWindowService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"

#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qtextedit.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qapplication.h>
#include <qaccel.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qlistview.h>
#include <fstream>
#include <iostream>
#include <sys/types.h> 
#include <dirent.h> 
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

IgQTDemoItem::IgQTDemoItem(const QString& demofile)
{
    m_dir="";
    m_name="";
    m_status=false;
    init(demofile);
}

void IgQTDemoItem::init(const QString& demofile)
{
    DIR *dir;
    struct dirent *file;
    int pos=demofile.findRev("/");
    if(pos>=0)
    {
	m_dir=demofile.left(pos);
	m_name=demofile.right(demofile.length()-pos-1);
	m_status=false; 
	dir=opendir(m_dir.latin1());
	if(dir != NULL)
	{
	    while((file=readdir(dir)))
	    {
		QString fname(file->d_name);
		if((fname==".") || (fname=="..") || 
		    ((fname.right(2).lower() != ".h") && 
		     (fname.right(4).lower() != ".cpp") &&
		     (fname.right(3).lower() != ".cc")))
		    continue;
		m_files.insert(m_files.end(),fname);
	    }
	}
    }
}	

IgQTDemo::IgQTDemo(IgState *state, const QString& demofile)
{
       
    ASSERT (state);
    
    m_demofile=demofile;
    m_lastitem=-1;
    m_pid=0;
    printer = new QPrinter;
    QPixmap printIcon;
    
    
    // Main window from the studio.
    m_mainWindow = static_cast<QMainWindow*>(IgQtAppWindowService::get (state)->mainWindow ());
    ASSERT (m_mainWindow);
    m_mainWindow->setCaption( "Qt Example - Application" );
            
    QToolBar * fileTools = new QToolBar( m_mainWindow, "file operations" );
    fileTools->setLabel( "File Operations" );
    
    printIcon = QPixmap( fileprint );
    QToolButton * filePrint
	    = new QToolButton( printIcon, "Print File", 
	                       QString::null,
			       this, SLOT(print()), 
			       fileTools, "print file" );

    (void)QWhatsThis::whatsThisButton( fileTools );

    const char * filePrintText = "Click this button to print the file you "
				 "are editing.\nYou can also select the Print "
				 "command from the File menu.";

    QWhatsThis::add( filePrint, filePrintText );
    
    ASSERT (IgQtAppMenuService::get (state));    
    QPopupMenu *file = IgQtAppMenuService::get (state)->subMenu
	(IgQtAppMenuService::MENU_ID_FILE);

    int id;

    id = file->insertItem( printIcon, "&Print...",
			   this, SLOT(print()), CTRL+Key_P, -1, 0 );
    file->setWhatsThis( id, filePrintText );

    file->setWhatsThis( id, 
                        "Click this menu item to quit from the application.\n"
			"You can also click on the \"x\" button on the right "
			"top corner to quit");

    m_mainWindow->menuBar()->insertSeparator();

    QFrame *frame=new QFrame(m_mainWindow,"MainFrame");
    QHBoxLayout *flayout = new QHBoxLayout(frame, 0, 5 ,"frame_layout"); 
    
    m_demo=new QListView(frame,"IgQTDemoList");
    m_demo->setFixedWidth(150);
    id=m_demo->addColumn("QT Demo");
    m_demo->setColumnWidthMode(id,QListView::Maximum);
    QWhatsThis::add(m_demo,"List of all QT Demos available."
                           " Click on any demo to run.");
	    
    m_flist=new QListView(frame,"IgQTDemoFileList");
    m_flist->setFixedWidth(150);
    id=m_flist->addColumn("File List");    
    m_flist ->setColumnWidthMode(id,QListView::Maximum);
    QWhatsThis::add(m_flist,"List of all source files for QT Demo selected in "
                            "the demo list.\nClick on file name to see its "
			    "source in the source text window");    
    
    m_file = new QTextEdit(frame, "FileEditor" );
    m_file->setReadOnly(true);
    QWhatsThis::add(m_file,"Text window to show the source file of QT Demos.");
    
    flayout->addWidget(m_demo);
    flayout->addWidget(m_flist);
    flayout->addWidget(m_file);
    m_demo->setFocus();
    m_mainWindow->setCentralWidget(frame);
    QObject::connect( m_demo, SIGNAL(selectionChanged ( QListViewItem * )),
                      this, SLOT(demoChanged( QListViewItem *) ) );
    QObject::connect( m_flist, SIGNAL(selectionChanged ( QListViewItem * )),
                      this, SLOT(flistChanged( QListViewItem *) ) );
    init();
    m_mainWindow->statusBar()->message( "Ready", 2000 );
    
    m_mainWindow->resize( 1000, 760 );
    frame->show ();
}


IgQTDemo::~IgQTDemo()
{
    terminateChild();
    delete printer;
    delete m_demo;
    delete m_flist;
    delete m_file;
}

void IgQTDemo::init()
{
    if (!m_demofile.isEmpty ())
    {
      std::ifstream input;
      char data[512];
      input.open(m_demofile.latin1());
      while(input)
      {
	input.getline(data,512);
	checkDemoItem(data);
      }
    } 
    else
    {
      QString base("/afs/cern.ch/cms/external/gcc-3.2/Qt/3.2.1");
      checkDemoItem(base+"/examples/aclock/aclock");
      checkDemoItem(base+"/examples/action/action");
      checkDemoItem(base+"/examples/action/toggleaction/toggleaction");
      checkDemoItem(base+"/examples/addressbook/addressbook");
      checkDemoItem(base+"/examples/application/application");
      checkDemoItem(base+"/examples/buttongroups/buttongroups");
      checkDemoItem(base+"/examples/canvas/canvas");
      checkDemoItem(base+"/examples/chart/chart");
      checkDemoItem(base+"/examples/checklists/checklists");
      checkDemoItem(base+"/examples/cursor/cursor");
      checkDemoItem(base+"/examples/customlayout/customlayout");
      checkDemoItem(base+"/examples/dclock/dclock");
      checkDemoItem(base+"/examples/demo/demo");
      checkDemoItem(base+"/examples/dirview/dirview");
      checkDemoItem(base+"/examples/dragdrop/dragdrop");
      checkDemoItem(base+"/examples/drawdemo/drawdemo");
      checkDemoItem(base+"/examples/drawlines/drawlines");
      checkDemoItem(base+"/examples/extension/extension");
      checkDemoItem(base+"/examples/fileiconview/fileiconview");
      checkDemoItem(base+"/examples/fonts/simple-qfont-demo/fontdemo");
      checkDemoItem(base+"/examples/forever/forever");
      checkDemoItem(base+"/examples/gridview/gridview");
      checkDemoItem(base+"/examples/hello/hello");
      checkDemoItem(base+"/examples/helpviewer/helpviewer");
      checkDemoItem(base+"/examples/i18n/i18n");
      checkDemoItem(base+"/examples/iconview/iconview");
      checkDemoItem(base+"/examples/iconview/simple_dd/simple_dd");
      checkDemoItem(base+"/examples/layout/layout");
      checkDemoItem(base+"/examples/life/life");
      checkDemoItem(base+"/examples/lineedits/lineedits");
      checkDemoItem(base+"/examples/listbox/listbox");
      checkDemoItem(base+"/examples/listboxcombo/listboxcombo");
      checkDemoItem(base+"/examples/listviews/listviews");
      checkDemoItem(base+"/examples/mdi/mdi");
      checkDemoItem(base+"/examples/menu/menu");
      checkDemoItem(base+"/examples/movies/movies");
      checkDemoItem(base+"/examples/network/clientserver/server/server");
      checkDemoItem(base+"/examples/network/ftpclient/ftpclient");
      checkDemoItem(base+"/examples/network/httpd/httpd");
      checkDemoItem(base+"/examples/network/infoprotocol/infoclient/infoclient");
      checkDemoItem(base+"/examples/network/infoprotocol/infoserver/infoserver");
      checkDemoItem(base+"/examples/network/infoprotocol/infourlclient/infourlclient");
      checkDemoItem(base+"/examples/network/mail/mail");
      checkDemoItem(base+"/examples/network/networkprotocol/networkprotocol");
      checkDemoItem(base+"/examples/network/remotecontrol/remotecontrol");
      checkDemoItem(base+"/examples/opengl/box/box");
      checkDemoItem(base+"/examples/opengl/gear/gear");
      checkDemoItem(base+"/examples/opengl/glpixmap/glpixmap");
      checkDemoItem(base+"/examples/opengl/sharedbox/sharedbox");
      checkDemoItem(base+"/examples/opengl/texture/texture");
      checkDemoItem(base+"/examples/popup/popup");
      checkDemoItem(base+"/examples/process/process");
      checkDemoItem(base+"/examples/progress/progress");
      checkDemoItem(base+"/examples/progressbar/progressbar");
      checkDemoItem(base+"/examples/qdir/qdir");
      checkDemoItem(base+"/examples/qfd/qfd");
      checkDemoItem(base+"/examples/qmag/qmag");
      checkDemoItem(base+"/examples/qwerty/qwerty");
      checkDemoItem(base+"/examples/rangecontrols/rangecontrols");
      checkDemoItem(base+"/examples/richtext/richtext");
      checkDemoItem(base+"/examples/rot13/rot13");
      checkDemoItem(base+"/examples/scribble/scribble");
      checkDemoItem(base+"/examples/scrollview/scrollview");
      checkDemoItem(base+"/examples/showimg/showimg");
      checkDemoItem(base+"/examples/splitter/splitter");
      checkDemoItem(base+"/examples/tabdialog/tabdialog");
      checkDemoItem(base+"/examples/table/bigtable/bigtable");
      checkDemoItem(base+"/examples/table/small-table-demo/smalltable");
      checkDemoItem(base+"/examples/table/statistics/statistics");
      checkDemoItem(base+"/examples/tetrix/tetrix");
      checkDemoItem(base+"/examples/textedit/textedit");
      checkDemoItem(base+"/examples/themes/themes");
      checkDemoItem(base+"/examples/thread/prodcons/prodcons");
      checkDemoItem(base+"/examples/thread/semaphores/semaphores");
      checkDemoItem(base+"/examples/tictac/tictac");
      checkDemoItem(base+"/examples/tooltip/tooltip");
      checkDemoItem(base+"/examples/tux/tux");
      checkDemoItem(base+"/examples/widgets/widgets");
      checkDemoItem(base+"/examples/wizard/wizard");
      checkDemoItem(base+"/examples/xform/xform");
      checkDemoItem(base+"/examples/xml/outliner/outliner");
      checkDemoItem(base+"/examples/xml/tagreader-with-features/tagreader-with-features");
    }
}

void
IgQTDemo::checkDemoItem(const QString& file)
{
    if(!file.isEmpty())
    {
      struct stat st;
      if((stat(file.latin1(),&st)!=0) || 
         (S_ISREG(st.st_mode)==0) ||
         (access(file.latin1(),X_OK)!=0))
        return;
    }
    else
      return;
    IgQTDemoItem item(file);
    QString file1=item.name();	 
    if(!file1.isEmpty())
    {
      m_demoitems.insert(m_demoitems.end(),item);
      QListViewItem *item1=new QListViewItem(m_demo);
      item1->setText(0,file1);
    }
}

void IgQTDemo::demoChanged( QListViewItem *item)
{
      int pos=-1;
      for(unsigned int i=0;i<m_demoitems.size();i++)
      {
	  if(item->text(0) == m_demoitems[i].name())
	  {
	      pos=i;
	      break;
	  }
      }
      if((m_lastitem!=pos) && (pos<(int)m_demoitems.size()) && (pos>=0))
      {
	  QWhatsThis::remove(m_flist);
	  m_lastitem=pos;
	  pos=m_demoitems[pos].fileCount();
	  m_flist->clear();
	  for(int i=0;i<pos;i++)
	  {
	      QListViewItem *item1=new QListViewItem(m_flist);
	      item1->setText(0,m_demoitems[m_lastitem].file(i));
	  }
	  if(pos > 0)
	  {
	      QWhatsThis::add(m_flist,
	                     QString("List of all source files for QT Demo \"")
			     +item->text(0)+"\" .\nClick on file name to see "
			      "its source in the source text window"); 
	      m_flist->setSelected(m_flist->firstChild (),true);
	  }
	  else
	  {
	      QWhatsThis::add(m_flist,
	                      QString("List of all source files for QT Demo \"")
			      +item->text(0)+"\" .\nNo source files available "
			       "for this demo."); 
	  }
	  terminateChild();
	  startChild();
      }
}

void IgQTDemo::terminateChild()
{
    int status;
    if(m_pid>0)
    {
	kill(m_pid,SIGTERM);
	waitpid(m_pid,&status,0);    
    }
    m_pid=0;
}

void IgQTDemo::startChild()
{
    QString cmd=m_demoitems[m_lastitem].name();
    m_pid=fork();
    if(m_pid==0)
    {
	chdir(m_demoitems[m_lastitem].directory().latin1());
	execl(cmd.latin1(),cmd.latin1(),NULL);
	_exit (EXIT_FAILURE);
    }
    else if(m_pid<0)
    {
	std::cerr <<"Error running \""<<
	       m_demoitems[m_lastitem].name().latin1()<<"\""<<std::endl;
	exit(1);
    }
    else
      m_mainWindow->statusBar()->message( "Running:"+m_demoitems[m_lastitem].name(), 5000 );
}

void IgQTDemo::flistChanged( QListViewItem *item)
{
    QString name=m_demoitems[m_lastitem].directory()+"/"+item->text(0);
    load(name);
    QWhatsThis::remove(m_file);
    QWhatsThis::add(m_file,"Text window to show the source file of QT Demos.\n"
                           "Currently showing \""+item->text(0)+"\" file which"
			   " belongs to \""+m_demoitems[m_lastitem].name()
			   +"\"  QT Demo"); 
    m_mainWindow->setCaption(QString("QT Demo - ")
               +m_demoitems[m_lastitem].name()
               +" ("+item->text(0)+ ")");
}

void IgQTDemo::load( const QString &fileName )
{
    QFile f( fileName );
    if ( !f.open( IO_ReadOnly ) )
	return;

    QTextStream ts( &f );
    m_file->setText( ts.read() );
    m_file->setModified( FALSE );
    //setCaption( fileName );
    m_mainWindow->statusBar()->message( "Loaded document " + fileName, 2000 );
}

void IgQTDemo::print()
{
    // ###### Rewrite to use QSimpleRichText to print here as well
    const int Margin = 10;
    int pageNo = 1;

    if ( printer->setup(m_mainWindow) ) {		// printer dialog
	m_mainWindow->statusBar()->message( "Printing..." );
	QPainter p;
	if( !p.begin( printer ) )               // paint on printer
	    return;

	p.setFont( m_file->font() );
	int yPos	= 0;			// y-position for each line
	QFontMetrics fm = p.fontMetrics();
	QPaintDeviceMetrics metrics( printer ); // need width/height
						// of printer surface
	for( int i = 0 ; i < m_file->lines() ; i++ ) {
	    if ( Margin + yPos > metrics.height() - Margin ) {
		QString msg( "Printing (page " );
		msg += QString::number( ++pageNo );
		msg += ")...";
		m_mainWindow->statusBar()->message( msg );
		printer->newPage();		// no more room on this page
		yPos = 0;			// back to top of page
	    }
	    p.drawText( Margin, Margin + yPos,
			metrics.width(), fm.lineSpacing(),
			ExpandTabs | DontClip,
			m_file->text( i ) );
	    yPos = yPos + fm.lineSpacing();
	}
	p.end();				// send job to printer
	m_mainWindow->statusBar()->message( "Printing completed", 2000 );
    } else {
	m_mainWindow->statusBar()->message( "Printing aborted", 2000 );
    }
}
void IgQTDemo::closeEvent( QCloseEvent* ce )
{
        ce->accept();
        return;
 }

void IgQTDemo::about()
{
    QMessageBox::about( m_mainWindow, "QT Demo Application",
			"This QT demo application will allow you to "
			"go through different examples of QT.");
}


void IgQTDemo::aboutQt()
{
    QMessageBox::aboutQt( m_mainWindow, "Qt Application Example" );
}
