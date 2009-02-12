//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgModuleCache.h"
#include "Iguana/Framework/interface/IgModuleDescriptor.h"
#include "Iguana/Framework/interface/IgModule.h"
#include "Iguana/Framework/interface/IgPluginManager.h"
#include "Iguana/Framework/interface/IgPluginError.h"
#include "IgPluginParserError.h"
#include "debug.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Log.h"
#include "classlib/iobase/DirIterator.h"
#include "classlib/utils/SharedLibrary.h"
#include "classlib/utils/StringFormat.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cerrno>
#include <sys/stat.h>
#include <sstream>
#include <vector>
#include <string>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>

static const char SAFE [] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			      "abcdefghijklmnopqrstuvwxyz"
			      "01234567890.,/_-" };
static const char HEX [] = { "0123456789abcdef" };

//<<<<<< PRIVATE TYPES                                                  >>>>>>

class IgModuleCache::FileByName
{
    lat::Filename m_name;
public:
    FileByName (const lat::Filename &name) : m_name (name) {}
    bool operator() (const IgModule *info)
    { return info->libraryName().nondirectory() == m_name; }
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

const char		IgModuleCache::s_cacheFile [] = ".iglets";
const char		IgModuleCache::s_cacheTag  [] = "cache";

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

/** Encode string @a input for registry.  Replaces all unsafe characters
    with "%XX" where "XX" is the hex of the character numeric value.  */
static std::string
encode (const std::string &input)
{
    std::string result;
    result.reserve (input.size ());
    for (const char *s = input.c_str (); *s; ++s)
	if (strchr (SAFE, *s))
	    result += *s;
	else
	{
	    result += '%';
	    result += HEX [((unsigned char) *s >> 4) & 0xf];
	    result += HEX [((unsigned char) *s     ) & 0xf];
	}
    return result;
}

/** Decode registry string @a input.  Replaces all occurances of "%XX"
    with the character of interpreting the hex value "XX", and at the
    same time checks only safe characters are used in @a input.  */
static std::string
decode (const std::string &input)
{
    std::string result;
    result.reserve (input.size ());
    for (const char *s = input.c_str (); *s; ++s)
	if (strchr (SAFE, *s))
	    result += *s;
	else if (*s == '%' && isxdigit (s[1]) && isxdigit (s[2]))
	{
	    result += (char) (((strchr (HEX, tolower (s[1])) - HEX) << 4)
			      + ((strchr (HEX, tolower (s[2])) - HEX)));
	    s += 2;
	}
	else
	    throw IgPluginError (new IgPluginParserError ("bad character"));

    return result;
}


//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Initialise a module cache in @a directory.  Initialises the cache
    to clear state and then invokes #refresh() to rebuild; this causes
    for instance #Module objects to be created.  */
IgModuleCache::IgModuleCache (IgPluginManager *manager, const lat::Filename &directory)
    : m_manager (manager),
      m_directory (directory.asDirectory ())
{
    ASSERT (! m_directory.empty ());

    m_state.status = CACHE_MISSING;
    m_state.added  = m_state.removed
		   = m_state.changed
		   = m_state.skipped
		   = false;

    m_parse.module = 0;
    refresh ();
}

/** Destroy the module cache.  Destroys all modules owned by the cache.  */
IgModuleCache::~IgModuleCache (void)
{
    LOG (0, trace, LFplugin_manager,
         "destroying module cache for `" << m_directory << "'\n" << lat::indent);
    ASSERT (! m_directory.empty ());
    for (ModuleIterator m = m_cache.begin (); m != m_cache.end (); ++m)
	delete *m;

    LOG (0, trace, LFplugin_manager, lat::undent);
}

/** Return the module cache directory name.  */
lat::Filename
IgModuleCache::directory (void) const
{ return m_directory; }

/** Return an iterator to the first #Module in the cache.  */
IgModuleCache::Iterator
IgModuleCache::begin (void) const
{ return m_modules.begin (); }

/** Return an iterator one past the last #Module in the cache.  */
IgModuleCache::Iterator
IgModuleCache::end (void) const
{ return m_modules.end (); }

/** Refresh the module cache.  This causes the old ".cache" file to be
    loaded and to be compared with the current state of the directory.
    If there are any differences, a new cache will be written out.  */
void
IgModuleCache::refresh (void)
{
    load ();
    rebuild ();
    notify ();
    update ();
}

/** Parse a module cache file (".cache" or one of the "*.reg" files).  This
    both reads the file and reconstructs its contents as children of @a root;
    the descriptor tree is destroyed before the method returns.  If reading
    in ".cache", @a root should be null, if reading in a single registration
    it should be contain the initial cache structure, otherwise reconstruct
    will fail.  The reconstructed cache will be put into #m_parse.result.
    Returns @c true if the cache was successfully read and reconstructed,
    @c false otherwise.  */
bool
IgModuleCache::parse (const lat::Filename &file, IgModuleDescriptor *root /* = 0 */)
{
    // FIXME: Use platform independent facilities for all file stuff (actually, registry)
    struct stat		statbuf;
    lat::Filename	filename (m_directory, file);
    bool		errors = false;
    IgModuleDescriptor	*regdata = root;

    if (::stat (filename, &statbuf) == -1)
    {
	delete regdata;
	return true;
    }

    LOG (0, trace, LFplugin_manager, "parsing file: " << file << "\n");

    ASSERT (m_parse.result.empty ());
    ASSERT (! m_parse.module);

    // Parse the registry file
    try {
	// FIXME: use an object so we don't need to fclose() it.
	FILE *f = fopen (filename, "r");
	if (! f)
	    throw IgPluginError (new IgPluginParserError ("can't read the file"));

	typedef std::vector<IgModuleDescriptor *> ContextStack;
	ContextStack	stack;
	int		offset = 0;
	int		ch;

	// Bias the stack in case we were give a root preamble.  In
	// that case we simply descend as far as we can into the first
	// children of the preamble, and insert parse results there.
	// `regdata' is assinged
	while (root)
	{
	    ++offset;
	    stack.push_back (root);
	    root = root->children () ? root->child (0) : 0;
	}

	// Process the file
	do {
	    std::string	token;
	    int		level = offset;

	    while ((ch = fgetc (f)) != EOF && ch == ' ')
		++level;

	    if (ch == EOF)
		break;

	    if (level == 0 && regdata)
	    {
		fclose (f);
		throw IgPluginError (new IgPluginParserError ("duplicate top level line"));
	    }

	    stack.resize (level+1);
	    IgModuleDescriptor *doc
		= new IgModuleDescriptor (level ? stack [level-1] : 0);
	    stack [level] = doc;
	    if (level == 0)
		regdata = doc;

	    for ( ; ch != EOF; ch = fgetc (f))
		if (ch == '\n')
		{
		    if (! token.empty ())
			doc->addToken (decode (token));

		    if (doc->tokens () == 0)
		    {
			fclose (f);
			throw IgPluginError (new IgPluginParserError ("empty line"));
		    }

		    break;
		}
		else if (ch == ' ')
		{
		    if (! token.empty ())
			doc->addToken (decode (token));
		    token = "";
		}
		else
		    token += ch;
	} while (ch != EOF);

	fclose (f);
	reconstruct (regdata);

	ASSERT (! m_parse.module);
    } catch (lat::Error &e) {
	m_manager->feedback (IgPluginManager::ErrorBadCacheFile, file, &e);

	// FIXME: Notify owner
	LOG (0, error, LFplugin_manager,
	     file << ": error, file ignored (" << e.explain() << ")\n");

	errors = true;
    } catch (...) {
	m_manager->feedback (IgPluginManager::Other, file);

	// FIXME: Notify owner
	LOG (0, error, LFplugin_manager,
	     file << ": unknown error, file ignored\n");

	errors = true;
    }

    delete regdata;

    if (errors)
    {
	delete m_parse.module;
	m_parse.module = 0;

	for (ModuleIterator m = m_parse.result.begin ();
	     m != m_parse.result.end (); ++m)
	    delete *m;

	m_parse.result.clear ();
    }

    return !errors;
}

/** Load the module cache file ".cache".  Attempts to load and rebuild
    the cache from the file.  The cache is first set to #CACHE_MISSING
    status and all #m_state flags are cleared.  If the file exists and
    can be parsed and reconstructed successfully, #m_cache is set to
    the #IgModule list and the cache status is set to #CACHE_OLD.
     Otherwise #m_cache is left untouched.  */
void
IgModuleCache::load (void)
{
    LOG (0, trace, LFplugin_manager, "loading cache "
	 << lat::Filename (m_directory, s_cacheFile) << '\n' << lat::indent);

    m_state.status = CACHE_MISSING;
    m_state.added  = m_state.removed
		   = m_state.changed
		   = m_state.skipped
		   = false;

    if (parse (s_cacheFile))
    {
	m_state.status = CACHE_OLD;

	// Clear the cache before adding to it (in case we already
	// have something, i.e. refresh() was already invoked).
	for (ModuleIterator m = m_cache.begin (); m != m_cache.end (); ++m)
	    delete *m;

	m_cache.clear ();
	m_cache.splice (m_cache.end (), m_parse.result);
    }

    LOG (0, trace, LFplugin_manager, lat::undent << "done\n");
}

/** Construct a module descriptor for a bad @a file with timestamp @a time.  */
IgModuleDescriptor *
IgModuleCache::makeBad (const std::string &file, const std::string &time)
{
    IgPluginManager::get ()->feedback (IgPluginManager::ErrorBadCacheFile, file);
    IgModuleDescriptor *top = new IgModuleDescriptor (0, s_cacheTag);
    new IgModuleDescriptor (top, IgModule::tag (), file, time, "bad");
    return top;
}

/** Actually scan the module directory, compare it with the current
    #m_cache, and build a new @a cache that reflects the changes.
    Returns a cache status code to reflect the new state.  */
IgModuleCache::CacheStatus
IgModuleCache::scanModules (ModuleList &cache)
{
    LOG (0, trace, LFplugin_manager,
	 "scanning `" << m_directory << "'\n" << lat::indent);

    CacheStatus		result = CACHE_MISSING;
    lat::DirIterator	file (m_directory);
    lat::DirIterator	end;

    ASSERT (cache.empty ());
    for (; file != end; ++file)
    {
	// Ignore the cache file.
	if (*file == s_cacheFile)
	    continue;

	// Check, read and validate the module definition file.
	// FIXME: Use platform-independent facilities!
	struct stat	statbuf;
	lat::Filename	filename (m_directory, *file);
	if (filename.extension () != "iglet")
	    continue;

	// Ignore files we cannot stat
	int status = stat (filename, &statbuf);
	if (status == -1)
	{
	    LOG (0, error, LFplugin_manager,
		 "stat() failed with error " << errno << ", ignoring `"
		 << *file << "'\n");
	    m_state.skipped = true;
	    continue;
	}

	// Check whether the module reg fragment has changed by
	// comparing the file and cached timestamps.
	ModuleIterator match
	    = std::find_if (m_cache.begin(), m_cache.end(),
			    FileByName (*file));

	if (match == m_cache.end ())
	{
	    LOG (0, trace, LFplugin_manager,
		 "==> new module definition `" << *file << "'\n");
	    m_state.added = true;
	}
	else if ((*match)->time () != (unsigned long) statbuf.st_mtime)
	{
	    LOG (0, trace, LFplugin_manager,
		 "==> modified module definition `" << *file << "' (file is "
		 << statbuf.st_mtime << ", cached was " << (*match)->time ()
		 << ")\n");
	    m_state.changed = true;
	}
	else
	{
	    // Copy the old cache to the new cache.  This relies on
	    // the knowledge that the old cache will be thrown away
	    // and so it is ok to steal the entry from the old one to
	    // the new one.
	    LOG (0, trace, LFplugin_manager,
		 "==> unchanged definition `" << *file << "'\n");
	    cache.splice (cache.end (), m_cache, match);
	    continue;
	}

	// Construct a fake registry document root for this file
	std::string time = lat::StringFormat ("%1").arg (statbuf.st_mtime);
	IgModuleDescriptor *top = new IgModuleDescriptor (0, s_cacheTag);
	new IgModuleDescriptor (top, IgModule::tag (), *file, time, "good");
        ASSERT (m_parse.result.empty ());
        ASSERT (! m_parse.module);
	reconstruct (top);
        ASSERT (! m_parse.module);

	// Try loading the modules to check that they are well-formed:
	// ensure that each module loads and resolves the required
	// entry points.  If this fails, mark the whole registration
	// file bad so that it will be ignored until the time-stamp
	// changes.  This leaves the cache always in valid state,
	// remembering anything bad.
	ASSERT (m_parse.result.size () == 1);
	for (ModuleIterator m = m_parse.result.begin () ; m != m_parse.result.end (); ++m)
	    try
	    {
		(*m)->load (); // checks for entry points
		(*m)->query ();
		(*m)->unload ();
	    }
	    catch (lat::Error &e)
	    {
		m_manager->feedback (IgPluginManager::ErrorBadModule,
			 	     (*m)->libraryName (), &e);

		// FIXME: Notify to owner
		LOG (0, error, LFplugin_manager,
		     *file << ": bad module library `"
		     << (*m)->libraryName ()
		     << "', module ignored\n (error was: "
		     << e.explain () << ")\n");
		m_state.skipped = true;
		(*m)->bad (true);
		break;
	    }

	// Transfer into cache.
	cache.splice (cache.end (), m_parse.result);
    }

    // See if cache contains info on removed modules.
    for (ModuleIterator m = m_cache.begin (); m != m_cache.end (); ++m)
	if (std::find_if (cache.begin (), cache.end (),
			  FileByName ((*m)->libraryName ().nondirectory ()))
	    == cache.end ())
	{
	    LOG (0, trace, LFplugin_manager,
		 "==> removed module `" << (*m)->libraryName () << "'\n");
	    m_state.removed = true;
	}

    // Update status codes.  Note that CACHE_VALID means .cache is
    // valid, CACHE_OLD means it needs updating.  If directory was
    // empty, mark the cache valid so we don't try to create one.
    result = (m_state.added
              || m_state.removed
	      || m_state.changed
	      || m_state.skipped)
	      ? CACHE_OLD : CACHE_VALID;

    LOG (0, trace, LFplugin_manager,
	 lat::undent << "done (" << (int) result << ")\n");
    return result;
}

/** Rebuild the cache comparing it to the actual directory contents.
    Assumes any existing cache is already loaded.  Scans the directory
    contents and compares with the cache, updating it to the new state
    of things.  Updates #m_state to reflect what happened.  */
void
IgModuleCache::rebuild (void)
{
    LOG (0, trace, LFplugin_manager,
	 "rebuilding cache in `" << m_directory << "'\n" << lat::indent);

    // Build a new cache and stealing bits from the old one as the new
    // one is rebuilt.  Then replace the cache with the new one and
    // delete whatever was left over from the old cache (creating the
    // new one may steal bits of the old).
    ModuleList cache;
    m_state.status = scanModules (cache);
    m_cache.swap (cache);
    for (ModuleIterator m = cache.begin (); m != cache.end (); ++m)
	delete *m;

    LOG (0, trace, LFplugin_manager,
	 lat::undent << "done (" << (int) m_state.status
	 << ", added = " << m_state.added
	 << ", removed = " << m_state.removed
	 << ", changed = " << m_state.changed
	 << ", skipped = " << m_state.skipped
	 << ")\n");
}

/** Update #m_modules from the current state of the cache (#m_cache).  */
void
IgModuleCache::notify (void)
{
    LOG (0, trace, LFplugin_manager, "collecting modules\n" << lat::indent);
    for (ModuleIterator m = m_cache.begin (); m != m_cache.end (); ++m)
	if (! (*m)->bad ()) m_modules.push_back (*m);
    LOG (0, trace, LFplugin_manager, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Dump the module descriptor @a doc and all it's children into the
    stream @a s.  Output should happen at nesting @a level; omit this
    argument on the first call, it is meant to be used only in the
    internal implementation.  */
void
IgModuleCache::write (std::ostream &s, IgModuleDescriptor *doc, int level/*=0*/)
{
    s << std::string (level, ' ') << encode (doc->token (0));
    for (unsigned i = 1; i < doc->tokens (); ++i)
	s << ' ' << encode (doc->token (i));
    s << std::endl;

    for (unsigned i = 0; i < doc->children (); ++i)
	write (s, doc->child (i), level+1);
}

/** Update the ".cache" file on the disk if it is writable.  */
void
IgModuleCache::update (void)
{
    if (m_state.status == CACHE_VALID)
	return;

    // Regenerate cache file from information we have in memory
    LOG (0, trace, LFplugin_manager,
	 "updating cache in `" << m_directory << "'\n" << lat::indent);

    // Build a descriptor tree and dump it out
    IgModuleDescriptor *all = new IgModuleDescriptor (0, s_cacheTag);
    for (ModuleIterator m = m_cache.begin (); m != m_cache.end (); ++m)
	(*m)->cache (all);

    std::ostringstream buf;
    write (buf, all);
    delete all;

    lat::Filename filename (m_directory, s_cacheFile);
    FILE	  *file = STDC::fopen (filename, "wb");

    if (! file)
    {
	LOG (0, error, LFplugin_manager,
	     filename << ": error " << errno << ", unable to open the cache"
	     " for writing -- cache not updated\n" << lat::undent);
	return;
    }

    std::string data (buf.str ());
    if (STDC::fwrite (data.c_str (), 1, data.size (), file) != data.size ())
    {
	LOG (0, error, LFplugin_manager,
	     filename << ": error " << errno << " while writing cache out"
	     " -- forcing file truncation\n");

	file = STDC::freopen (filename, "wb", file);
    }
    else
	m_state.status = CACHE_VALID;

    STDC::fclose (file);

    LOG (0, trace, LFplugin_manager, lat::undent << "done\n");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Validate and reconstruct descriptor proxies from @a info.  */
void
IgModuleCache::reconstruct (IgModuleDescriptor *info)
{
    // Protect against random junk, e.g. empty files
    if (! info || ! info->tokens ())
	throw IgPluginError (new IgPluginParserError ("empty descriptor"));

    // Make sure the root is either a cache...
    if (info->token (0) == s_cacheTag)
    {
	if (info->tokens () != 1)
	    throw IgPluginError (new IgPluginParserError
			         ("`cache' does not take arguments"));
	else if (info->parent ())
	    throw IgPluginError (new IgPluginParserError
			         ("`cache' must be at the root"));

	for (unsigned i = 0; i < info->children (); ++i)
	    reconstruct (info->child (i));

	ASSERT (! m_parse.module);
    }
    // ... or a module itself
    else if (info->token (0) == IgModule::tag ())
    {
	if (info->tokens () != 4)
	    throw IgPluginError (new IgPluginParserError
			         ("`module' must have three arguments"));
	if (! info->parent () || info->parent ()->token (0) != s_cacheTag)
	    throw IgPluginError (new IgPluginParserError
			         ("`module' must be under `cache'"));
	if (info->token (3) != "good" && info->token (3) != "bad")
	    throw IgPluginError (new IgPluginParserError
			         ("`module' last argument must be"
				  " 'good' or 'bad'"));

	ASSERT (! m_parse.module);
	m_parse.module = new IgModule (m_manager,
				       lat::Filename (m_directory, info->token (1)),
				       info->token (2),
				       info->token (3));

	for (unsigned i = 0; i < info->children (); ++i)
	    reconstruct (info->child (i));

	ASSERT (m_parse.module);
	m_parse.result.push_back (m_parse.module);
	m_parse.module = 0;
    }
    // ... or junk we don't understand
    else
    {
	if (! m_parse.module)
	    throw IgPluginError (new IgPluginParserError
			         (info->token (0) + " unexpected"));

	if (info->tokens () >= 2)
            LOG (0, trace, LFplugin_manager, "processing "
		 << info->token(0) << ' ' << info->token(1) << '\n');

	m_parse.module->restore (info);
    }
}
