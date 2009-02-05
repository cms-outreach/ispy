//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtAppDebug.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppDebugService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgPluginLoader.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgExtensionDB.h"
#include "Ig_Framework/IgPluginManager/interface/IgPluginManager.h"
#include "Ig_Framework/IgPluginManager/interface/IgModule.h"
#include "Ig_Framework/IgPluginManager/interface/IgModuleCache.h"
#include "Ig_Framework/IgPluginManager/interface/IgModuleDescriptor.h"
#include <classlib/utils/ResourceInfo.h>
#include <classlib/utils/ProcessInfo.h>
#include <classlib/utils/SharedLibrary.h>
#include <classlib/utils/Signal.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/sysapi/Filename.h>
#include <qdialog.h>
#include <qframe.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <qheader.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qtimer.h>
#include <qnamespace.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qpixmap.h>
#include <stdio.h>
#include <set>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtAppDebugInfo::IgQtAppDebugInfo (QWidget *parent /* = 0 */,
				    bool autoRefresh /* = true */)
    : QDialog (parent),
      m_autoRefresh (autoRefresh),
      m_data (0),
      m_timer (0)
{
    m_data		 = new QListView (this);
    QHBox	*buttons = new QHBox (this);
    QPushButton	*ok	 = new QPushButton ("Close", buttons);
    QPushButton	*refresh = new QPushButton ("Refresh", buttons);
    QPushButton *save    = new QPushButton ("Save List", buttons);
	
    buttons->setMargin (5);
    buttons->setSpacing (25);

    QVBoxLayout	*vbox	 = new QVBoxLayout (this);

    vbox->addWidget (m_data);
    vbox->addWidget (buttons);

    connect (ok, SIGNAL(clicked ()), this, SLOT(hide()));
    connect (refresh, SIGNAL(clicked ()), this, SLOT(update()));
    connect (save, SIGNAL(clicked ()), this, SLOT(exportList ()));
}

IgQtAppDebugInfo::~IgQtAppDebugInfo (void)
{
    // FIXME: Does qt do these automatically?
    delete m_timer;
}

void
IgQtAppDebugInfo::info (void)
{
    update ();
    show ();
    start ();
}

void
IgQtAppDebugInfo::exportList (void)
{
    if (!m_data->firstChild ())
        return;

    QFileDialog* fileDialog = new QFileDialog (this, "File Dialog", TRUE);
    fileDialog->setMode (QFileDialog::AnyFile);
  
    QString fileName;
    if (fileDialog->exec () == QDialog::Accepted) 
    {
        fileName = fileDialog->selectedFile ();
    
	QFile file (fileName);
	if (!file.open (IO_WriteOnly))
	    return;

	QTextStream textStream (&file);

	QListViewItemIterator it (m_data);
        QHeader *header =  m_data->header ();

	for ( ; it.current(); ++it) 
	{
	    std::string tab("");
	    int depth = it.current()->depth ();
	    for(int i=0;i<depth; i++)
	      tab+="  ";
	    
	    for (int i = 0; i < m_data->columns (); i++) 
	    {
		int orderedItem = header->mapToSection (i);
		textStream <<tab.c_str ()<< it.current ()->text (orderedItem) << (it.current ()->pixmap (orderedItem) ? "*" : QString::null) << "\t";
	    }
	    textStream << "\n";
	}
	file.close();
    }
}

void
IgQtAppDebugInfo::hideEvent (QHideEvent* event) 
{
    delete m_timer;
    m_timer = 0;
    QWidget::hideEvent (event);
}

void
IgQtAppDebugInfo::start (void)
{
    if (m_autoRefresh)
    {
	// Create the update timer
	if (! m_timer)
	{
	    m_timer = new QTimer (this);
	    connect (m_timer, SIGNAL(timeout()), this, SLOT(update()));
	}
	m_timer->start (5000);
    }
}

QListView *
IgQtAppDebugInfo::list (void)
{
    return m_data;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtAppDebugResourceInfo::IgQtAppDebugResourceInfo (QWidget *parent)
    : IgQtAppDebugInfo (parent)
{
    list ()->addColumn ("Resource");
    list ()->addColumn ("Value");
    list ()->addColumn ("Units");
    list ()->setColumnAlignment (1, Qt::AlignRight);

    setCaption ("Program Resource Consumption");
    resize (320, 580);
}

void
IgQtAppDebugResourceInfo::update (void)
{
    QListView *props = list ();
    ASSERT (props);

    lat::ResourceInfo::Status		stats (lat::ResourceInfo::info_self());
    lat::ResourceInfo::Status::iterator i = stats.begin ();
    char				buf [512];

    props->clear ();
    for ( ; i != stats.end (); ++i)
	new QListViewItem (props, lat::ResourceInfo::name (i->first),
			   lat::ResourceInfo::format (i->first, i->second,
						       buf, sizeof (buf)),
			   lat::ResourceInfo::unit (i->first));
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtAppDebugLibrariesInfo::IgQtAppDebugLibrariesInfo (QWidget *parent)
    : IgQtAppDebugInfo (parent, false)
{
    list ()->addColumn ("Address");
    list ()->addColumn ("Component");
    list ()->addColumn ("File Name");
    list ()->setColumnAlignment (0, Qt::AlignRight);

    setCaption ("Shared Libraries Currently Loaded");
    resize (600, 200);
}

static void
LibraryEnumerator (const lat::SharedLibrary::LibraryInfo &info, QListView *view)
{
    char		buf [40];  // ample for 128+ bits.
    lat::Filename	fname (info.m_filename);

    sprintf (buf, "0x%08lX", info.m_text_start);
    new QListViewItem (view, buf, (const char *) fname.nondirectory (), (const char *) fname);
}

void
IgQtAppDebugLibrariesInfo::update (void)
{
    QListView *props = list ();
    ASSERT (props);

    props->clear ();
    lat::SharedLibrary::loaded (lat::CreateCallback (&LibraryEnumerator, props));
    props->setSorting (0);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtAppDumpStateInfo::IgQtAppDumpStateInfo (IgState *state,
                                                 QWidget *parent)
    : IgQtAppDebugInfo (parent, false),
      m_state (state->root ())
{
    list ()->addColumn ("States");

    setCaption ("IGUANA State tree");
    resize (600, 200);
}

void
IgQtAppDumpStateInfo::update (void)
{
    QListView *props = list ();
    ASSERT (props);

    props->clear ();
    populate (m_state, new QListViewItem (props, "State"));
}

void
IgQtAppDumpStateInfo::populate (IgState* state, QListViewItem *item)
{
    item->setOpen (true);
    
    QListViewItem* element = new QListViewItem (item, "Elements");
    bool hasElements = false;
    for (unsigned int i=0; i<state->elements(); i++)
    {
        if (state->element (i))
	{
	    new QListViewItem (element,
	      state->element (i)->stateKey().label ().c_str());
	    hasElements = true;
	}
    }
    if (!hasElements)
        delete element;
    else
        element->setOpen (true);
	
    if (state->children())
        for (unsigned int i=0; i<state->children(); i++)
	    populate (state->child(i), new QListViewItem (item, "State"));
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtAppDebugPluginsInfo::IgQtAppDebugPluginsInfo (IgState *state,
						  QWidget *parent)
    : IgQtAppDebugInfo (parent, false),
      m_state (state)
{
    list ()->addColumn ("Type");
    list ()->addColumn ("Plug-in");
    list ()->addColumn ("Attached");
    list ()->setRootIsDecorated (true);

    setCaption ("Available Plugins");
    resize (600, 200);
}

template <class I>
void
FillPlugins (I begin, I end, QListViewItem *parent)
{
}

void
IgQtAppDebugPluginsInfo::update (void)
{
    static const char	*check [] = {
	"    16    13        2            1",
	"  c none",
	"x c #000000",
	"                ",
	"               x",
	"             xx ",
	"            x   ",
	"  xx      xx    ",
	" xxxx    xx     ",
	"  xxxx  xx      ",
	"   xxxxxx       ",
	"    xxxxx       ",
	"     xxx        ",
	"      xx        ",
	"                ",
	"                "
    };
    static QPixmap	*checkPixmap = new QPixmap (check);

    // FIXME: get state from constructor?
    QListView	*props = list ();

    ASSERT (m_state);
    ASSERT (props);

    props->clear ();
    
    typedef std::pair<std::string,std::string> Seen;
    IgPluginManager			*db = IgPluginManager::get ();
    IgPluginManager::DirectoryIterator	dir;
    IgModuleCache::Iterator		plugin;
    IgModuleDescriptor			*cache;
    std::set<Seen>			seen;
    unsigned				i;

    for (dir = db->beginDirectories (); dir != db->endDirectories (); ++dir)
	for (plugin = (*dir)->begin (); plugin != (*dir)->end (); ++plugin)
	    for (cache=(*plugin)->cacheRoot(), i=0; i < cache->children(); ++i)
	    {
		IgModuleDescriptor *d = cache->child (i);

		// Get category
		const char *catname = d->token (0).c_str ();
		QListViewItem *cat = props->findItem (catname, 0);
		if (! cat)
		{
		    cat = new QListViewItem (props, catname);
		    cat->setOpen (true);
		}

		// Get last item
		QListViewItem *prev = cat ? cat->firstChild () : 0;
		while (prev && prev->nextSibling ())
		    prev = prev->nextSibling ();

		const char *name = d->token (1).c_str ();
		const char *lib = (*plugin)->libraryName ();
		if (! *name)
		    name = "(Built-in)";

		if (seen.count (Seen (catname, name)))
		    continue;
		seen.insert (Seen (catname, name));

		if (! prev)
		    prev = new QListViewItem (cat, name, lib);
		else
		    prev = new QListViewItem (cat, prev, name, lib);

		if ((*plugin)->attached ())
		    prev->setPixmap (2, *checkPixmap);
	    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool IgQtAppDebug::m_profiler =  false;

IgQtAppDebug::IgQtAppDebug (IgState *state, QWidget *parent /* = 0 */)
    : m_state (state),
      m_parent (parent)
{
    m_info [0] = m_info [1] = m_info [2] = m_info [3] = 0; 
}

IgQtAppDebug::~IgQtAppDebug (void)
{
    // FIXME: Does qt do these automatically?
    delete m_info [3];
    delete m_info [2];
    delete m_info [1];
    delete m_info [0];
}

void
IgQtAppDebug::resources (void)
{
    if (! m_info [0])
	m_info [0] = new IgQtAppDebugResourceInfo (m_parent);

    m_info [0]->info ();
}

void
IgQtAppDebug::libraries (void)
{
    if (! m_info [1])
	m_info [1] = new IgQtAppDebugLibrariesInfo (m_parent);

    m_info [1]->info ();
}

void
IgQtAppDebug::plugins (void)
{
    if (! m_info [2])
	m_info [2] = new IgQtAppDebugPluginsInfo (m_state, m_parent);

    m_info [2]->info ();
}

void
IgQtAppDebug::dumpState (void)
{
    if (! m_info [3])
	m_info [3] = new IgQtAppDumpStateInfo (m_state, m_parent);

    m_info [3]->info ();
}

void
IgQtAppDebug::debugme (void)
{
    // Install a single-shot timer to start the debugger once the
    // event queue has been drained and the menu popped down.
    QTimer::singleShot (0, this, SLOT(rundebug()));
}

void
IgQtAppDebug::rundebug (void)
{
    // FIXME: Better logic to figure out best debugger to use.

    std::string programName = lat::ProcessInfo::fullProgramName ();

    char buf [64]; // ample for 128+ pids

    sprintf (buf, "ddd %s %s %ld &",
#ifdef __SUNPRO_CC
	     "--dbx",
#else
	     "--gdb",
#endif
	     programName.c_str (),
	     (long) lat::ProcessInfo::pid ());
    
    system (buf);
}

void
IgQtAppDebug::showProfiler (void)
{
    if (!m_profiler)
    {
      //Now load the Oprofile plugin.
      IgExtensionDB::Iterator it = IgExtensionDB::get ()->locate ("Extensions/Profiler/OProfile");
      if (*it != 0)
      {
        m_profiler = true;
        QPopupMenu *profile = IgQtAppMenuService::get(m_state)->subMenu(IgQtAppDebugService::MENU_ID_DEBUG);
        profile->disconnectItem (IgQtAppDebugService::MENU_ID_DEBUG_PROFILER, this, SLOT(showProfiler()));
          
	ASSERT (IgPluginLoader::get (m_state));
        IgPluginLoader::get (m_state)->load (m_state, "Extensions/Profiler/OProfile", true);
      }
      else
        QMessageBox::critical (0, "IGUANA", "You do not seem to have"
                               "\"Extensions/Profiler/OProfile\" plugin installed.");
   }
}
