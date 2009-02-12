//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgPluginManager.h"
#include "Iguana/Framework/interface/IgModuleDescriptor.h"
#include "Iguana/Framework/interface/IgModuleCache.h"
#include "Iguana/Framework/interface/IgModule.h"
#include "classlib/utils/StringOps.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/Error.h"
#include <iostream>
#include <utility>
#include <cstdlib>
#include <cstring>
#include <string>
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

using namespace lat;
void feedback (IgPluginManager::FeedbackData data)
{
    std::string explanation;
    if (data.error)
	explanation = StringOps::replace (data.error->explain (), '\n', "\n\t");

    if (data.code == IgPluginManager::StatusLoading)
	std::cerr << "Note: Loading " << data.scope << "\n";

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

    // This cannot actually trigger in SealPlugin* utility, for example only
    else if (data.code == IgPluginManager::ErrorNoFactory)
	std::cerr << "  *** WARNING: module `" << data.scope
		  << "' missing one or more factories for plug-ins\n";
}

int main (int, char **argv)
{
    Signal::handleFatal (argv [0]);

    // List all categories and items in them.  Set avoids duplicates.
    typedef std::pair<std::string,std::string>	Seen;
    typedef std::set<Seen>			SeenSet;
    typedef SeenSet::iterator			SeenSetIterator;

    IgPluginManager::destroyOnExit (argv[1] && !strcmp (argv[1], "-clean"));

    IgPluginManager			*db = IgPluginManager::get ();
    IgPluginManager::DirectoryIterator	dir;
    IgModuleCache::Iterator		plugin;
    IgModuleDescriptor			*cache;
    SeenSet				seen;
    unsigned				i;

    db->addFeedback (CreateCallback (&feedback));
    db->initialise ();
    for (dir = db->beginDirectories (); dir != db->endDirectories (); ++dir)
	for (plugin = (*dir)->begin (); plugin != (*dir)->end (); ++plugin)
	    for (cache=(*plugin)->cacheRoot(), i=0; i < cache->children(); ++i)
		seen.insert (Seen (cache->child (i)->token (0),
				   cache->child (i)->token (1)));

    SeenSetIterator cat = seen.begin ();
    SeenSetIterator current; 
    while (cat != seen.end ())
    {
	std::cout << "Category " << cat->first << ":\n";
	current = cat;
	while (current != seen.end () && current->first == cat->first)
	{
	    std::cout << "  " << current->second << std::endl;
	    ++current;
	}
	cat = current;
    }

    return EXIT_SUCCESS;
}
