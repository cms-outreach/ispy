//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgApplication.h"
#include "Iguana/Framework/interface/IgLibraryPreloader.h"
#include "Iguana/Framework/interface/IgOnCrashService.h"
#include "Iguana/Framework/interface/IgOnLowMemoryService.h"
#include "Iguana/Framework/interface/IgDriverDB.h"
#include "Iguana/Framework/interface/IgPluginLoader.h"
#include "Iguana/Framework/interface/IgArgsElement.h"
#include "Iguana/Framework/interface/IgEnvsElement.h"
#include "Iguana/Framework/interface/IgDriver.h"
#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/Framework/interface/IgModule.h"
#include "Iguana/Framework/interface/IgModuleCache.h"
#include "Iguana/Framework/interface/IgModuleDescriptor.h"
#include "classlib/utils/SearchPath.h"
#include "classlib/utils/StringOps.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/TimeInfo.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/Error.h"
#include <typeinfo>
#include <exception>
#include <algorithm>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <cstring>
#include <set>

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define IGUANA_APP "iguana"
#define IGUANA_VERSION "undefined"
#define IGUANA_UNAME "undefined"
#define IGUANA_HOST "undefined"
#define IGUANA_CXX "undefined"
#define IGUANA_COMPILER "undefined"
#define IGUANA_WHO "undefined"

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgApplication::IgApplication (void)
    : m_verbose (false),
      m_state (0),
      m_driver (0),
      m_argc (-1),
      m_argv (0),
      m_appname (0),
      m_all (false)
{}

IgApplication::~IgApplication (void)
{}

IgState *
IgApplication::state (void) const
{ return m_state; }

const char *
IgApplication::driver (void) const
{ return m_driver; }

const char *
IgApplication::appname (void) const
{
    const char *name = m_appname;
    if (const char *ptr = strrchr (name, '/'))
	name = ptr + 1;
    return name;
}

bool
IgApplication::verbose (void) const
{ return m_verbose; }

int
IgApplication::argc (void) const
{ return m_argc; }

char **
IgApplication::argv (void) const
{ return m_argv; }

//////////////////////////////////////////////////////////////////////
int
IgApplication::run (int argc, char *argv[])
{
    lat::TimeInfo::init ();
    initDebugging (argv [0]);

    bool list = false;
    bool help = false;
    char defDriver [] = { "IGUANA" };
    m_driver = defDriver;
    m_appname = argv [0];

    if (char *preloads = ::getenv ("IGUANA_PRELOADS"))
    {
	char *start = preloads;
	char *end = strchr (start, ':');
	for ( ; end; start = end + 1, end = strchr (start, ':'))
	    if (end - start)
		m_preload.push_back (std::string (start, end - start));

	if (*start)
	    m_preload.push_back (start);
    }

    int i = 1;    

    while (*++argv && argv [0][0] == '-')
    {
	i++;
	if (!strcmp (*argv, "--list"))
	{
	    list = true;
	    if ((i < argc) && (!strcmp (*++argv, "all")))
		m_all = true;
	}
	else if (!strcmp (*argv, "--version"))
	{
	    std::cout << "IGUANA " IGUANA_VERSION " (" IGUANA_HOST ")\n\n"
		      << "Built by " IGUANA_WHO " on " __DATE__ " " __TIME__ " as:\n"
		      << "   " IGUANA_UNAME "\n"
		      << "   " IGUANA_CXX " (" IGUANA_COMPILER ")\n";
	    return EXIT_SUCCESS;
	}
	else if (!strcmp (*argv, "--help"))
	    help = true;
	else if (!strcmp (*argv, "--profile"))
	    startProfiler ();
	else if (!strcmp (*argv, "--memstats"))
	    startMemStats ();
	else if (!strcmp (*argv, "--driver") || !strcmp (*argv, "-D"))
	{
	    if (! *++argv)
	    {
		help = true;
		std::cerr << "--driver requires an argument\n";
	    }
	    else
		m_driver = *argv;
	}
	else if (!strcmp (*argv, "--verbose"))
	    m_verbose = true;
	else if (!strcmp (*argv, "--preload"))
	{
	    if (! *++argv)
	    {
		help = true;
		std::cerr << "--preload requires an argument\n";
	    }
	    else
		m_preload.push_back (*argv);
	}
	else
	    // Assume the rest is for the driver.
	    break;
    }

    // Collect new argv vector for the driver, dropping out the main program and it's
    // options.  OS X doesn't like running programs without full path, so manufacture
    // a new full path from the program name and the driver.
    lat::Filename fakeApp (m_appname, m_driver);
    m_argc = argc;
    m_argv = new char *[m_argc+1];
    m_argv [0] = const_cast<char *> (static_cast<const char *> (fakeApp));
    for (m_argc = 1; *argv; ++argv, ++m_argc)
	m_argv [m_argc] = *argv;
    m_argv [m_argc] = 0;

    // Give help if requested or arguments are insufficient
    if (help || (! list && ! m_driver))
	return usage ();

    // Preload requested shared libraries
    int			status;
    IgLibraryPreloader	loader (m_argv [0], m_verbose);
    std::for_each (m_preload.begin (), m_preload.end (), loader);
    if ((status = loader.status ()) != EXIT_SUCCESS)
	return status;

    // Initialise the plugin database
    if ((status = initDatabase ()) != EXIT_SUCCESS)
	return status;

    // Initialise the state
    if ((status = initState ()) != EXIT_SUCCESS)
	return status;

    // Do the work: if we want just a list, do so, otherwise create
    // and run the driver.
    status = (!list ? loadDriver () : dumpDatabase ());

    if (m_verbose)
	// FIXME: This should be done in the app driver, setting the
	// status bar message just before quitting.  Uninteractive app
	// drivers shouldn't do this.  We might want to have some mini
	// service that makes info like this available to app drivers
	// (along with crash protection and low-memory stuff above).
	std::cout << "\nThanks for using " IGUANA_APP " " IGUANA_VERSION "!"
		  << (lat::Signal::crashed () ? "  (We apologize for the inconvenience.)"
		      : "") << std::endl
		  << IGUANA_APP " took "
		  << lat::TimeInfo::processUserTime () * 1e-9 << "s user, "
		  << lat::TimeInfo::processSystemTime () * 1e-9 << "s system, "
		  << lat::TimeInfo::processRealTime () * 1e-9 << "s real ("
		  << lat::TimeInfo::processIdleTime () * 1e-9 << "s idle)\n";

    return status;
}

void
IgApplication::startProfiler (void)
{
#ifdef __linux
    try
    {
	if (! getenv ("JPROF_FLAGS"))
	{
	    static char value [] = "JPROF_FLAGS=JP_START JP_PERIOD=.0015";
	    putenv (value);
	}

	lat::SharedLibrary *lib = lat::SharedLibrary::load (std::string ("libJProfLib.so"));
	void (*func) (void) = lib->function ("setupProfilingStuff");
	(*func) ();
    }
    catch (lat::Error &error)
    {
	std::cerr << "Cannot load or initialise jprof runtime\n";
	std::cerr << "Error was: " << error.explain () << "\n";
    }
#else
    std::cerr << "Profiling not available on this platform.  Continuing without.\n";
#endif
}

void
IgApplication::startMemStats (void)
{
#ifdef __linux
    try
    {
	lat::SharedLibrary::load (std::string ("libMemProfLib.so"));
    }
    catch (lat::Error &error)
    {
	std::cerr << "Cannot load or initialise memprof runtime\n";
	std::cerr << "Error was: " << error.explain () << "\n";
    }
#else
    std::cerr << "Memory profiling not available on this platform.  Continuing without.\n";
#endif
}

int
IgApplication::initDebugging (const char *appname)
{
    // Install basic debugging service
    if (! IgOnCrashService::init (appname))
	return EXIT_FAILURE;

    if (! IgOnLowMemoryService::init (appname))
	return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int
IgApplication::initState (void)
{
    // FIXME: install other services?
    m_state = new IgState;
    new IgArgsElement (m_state, m_argc, m_argv);
    new IgEnvsElement (m_state);
    new IgPluginLoader (m_state);
    return EXIT_SUCCESS;
}

int
IgApplication::usage (void)
{
    const char *app = appname ();

    std::cerr << "Usage: " << app << " --help\n"
	      << "   or: " << app << " --version\n"
	      << "   or: " << app << " --list [all]\n"
	      << "   or: " << app
	      << " [--verbose] [--preload LIBRARY]... [--profile] [--memstats]\n"
	      << "           [--driver|-D DRIVER] [DRIVER-OPTIONS...]\n\n"
	      << "The $IGUANA_PLUGINS environment variable should be set to point to\n"
	      << "the module registration directories, with directories separated\n"
	      << "'" << lat::SearchPath::separator () << "' as with normal paths.\n\n"
	      << "If no DRIVER is given, It will attempt to load `IGUANA'.\n"
	      << "'IGUANA' DRIVER-OPTIONS are .....\n"
	      << "    [--iguana-session|-is <iguana-session>]\n\n"
	      << "where '<iguana-session>' could be the name of any studio session type e.g.\n"
	      << "'Vis Example--Qt Demo' or 'Vis Example--Open Inventor File Reader' etc.\n\n"
	      << "One can also use the '; ' separated short names e.g. 'IvReader'\n\n"
	      << "'IvReader' also support an optional command-line argument\n"
	      << "    [--ivfile|-iv <ivfile>]\n"
	      << "for loading an open inventor IV file in the IV Reader.\n";

    return EXIT_FAILURE;
}

int
IgApplication::initDatabase (void)
{
    IgPluginManager::get ()->addFeedback
	(lat::CreateCallback (this, &IgApplication::pluginFeedback));
    IgPluginManager::get ()->initialise ();
    return EXIT_SUCCESS;
}

void
IgApplication::pluginFeedback (IgPluginManager::FeedbackData data)
{
    std::string explanation;
    if (data.error)
	explanation = lat::StringOps::replace (data.error->explain (), '\n', "\n\t");

    if (data.code == IgPluginManager::StatusLoading)
	std::cerr << "Info: Loading module " << data.scope << "\n";

    else if (data.code == IgPluginManager::ErrorLoadFailure)
	std::cerr << "  *** WARNING: module `" << data.scope
		  << "' failed to load for the following reason\n\t"
		  << explanation << "\n";

    else if (data.code == IgPluginManager::ErrorBadModule)
	std::cerr << "  *** WARNING: module `" << data.scope
		  << "' ignored until problems with it are fixed.\n\n";

    else if (data.code == IgPluginManager::ErrorBadCacheFile)
	std::cerr << "  *** WARNING: cache file " << data.scope
		  << " is corrupted.\n";

    else if (data.code == IgPluginManager::ErrorEntryFailure)
	std::cerr << "  *** WARNING: module `" << data.scope
		  << "' does not have the required entry point, reason was\n\t"
		  << explanation << "\n";
}

int
IgApplication::dumpDatabase (void)
{
    // List all categories and items in them.  Set avoids duplicates.
    typedef std::pair<std::string,std::string>	Seen;
    typedef std::set<Seen>			SeenSet;
    typedef SeenSet::iterator			SeenSetIterator;

    IgPluginManager				*db = 	IgPluginManager::get ();
    IgPluginManager::DirectoryIterator		dir;
    IgModuleCache::Iterator			plugin;
    IgModuleDescriptor				*cache;
    SeenSet					seen;
    unsigned					i;

    for (dir = db->beginDirectories (); dir != db->endDirectories (); ++dir)
	for (plugin = (*dir)->begin (); plugin != (*dir)->end (); ++plugin)
	    for (cache=(*plugin)->cacheRoot(), i=0; i < cache->children(); ++i)
		seen.insert (Seen (cache->child (i)->token (0),
				   cache->child (i)->token (1)));

    SeenSetIterator cat = seen.begin ();
    SeenSetIterator current; 
    while (cat != seen.end ())
    {
	if (m_all || !lat::StringOps::find (cat->first, "IGUANA"))
	    std::cout << "Category " << cat->first << ":\n";

	    current = cat;

	    while (current != seen.end () && current->first == cat->first)
	    {
		if (m_all || !lat::StringOps::find (cat->first, "IGUANA"))
		    std::cout << "  " << current->second << std::endl;
		++current;
	    }
	    cat = current;
    }

    if (seen.empty ())
	std::cout << "No plug-ins\n";

    return EXIT_SUCCESS;
}

int
IgApplication::loadDriver (void)
{
    try
    {
	ASSERT (m_driver);

	if (m_verbose)
	    std::cout << "Loading application driver `" << m_driver << "'.\n";

	IgDriver *app = IgDriverDB::get ()->create (m_driver, m_state);
	if (! app)
	{
	    std::cout << "No such application driver `" << m_driver << "'.\n\n";
	    return usage ();
	}

	if (m_verbose)
	    std::cout << "Executing the driver.\n";

	return app->run ();
    }
    catch (lat::Error &error)
    {
	IgOnCrashService::fatalException
	    (typeid(error).name (), error.explain ().c_str ());
	std::cerr << m_argv [0] << ": cannot run the driver: "
		  << error.explain () << "\n";
	return EXIT_FAILURE;
    }
    catch (std::exception &error)
    {
	IgOnCrashService::fatalException
	    (typeid(error).name (), error.what ());
	std::cerr << m_argv [0] << ": cannot run the driver\n";
	return EXIT_FAILURE;
    }
    catch (...)
    {
	IgOnCrashService::fatalException (0, 0);
	std::cerr << m_argv [0] << ": cannot run the driver\n";
	return EXIT_FAILURE;
    }
}
