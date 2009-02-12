//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgModule.h"
#include "Iguana/Framework/interface/IgModuleDef.h"
#include "Iguana/Framework/interface/IgModuleDescriptor.h"
#include "Iguana/Framework/interface/IgPluginCapabilities.h"
#include "Iguana/Framework/interface/IgPluginManager.h"
#include "Iguana/Framework/interface/IgPluginInfo.h"
#include "Iguana/Framework/interface/IgPluginError.h"
#include "debug.h"
#include "classlib/utils/SharedLibrary.h"
#include "classlib/utils/SharedLibraryError.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Log.h"
#include <algorithm>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

const char IgModule::s_modEntryPoint [] = "IGUANA_MODULE";
const char IgModule::s_capEntryPoint [] = "IGUANA_CAPABILITIES";
const char IgModule::s_tag [] = "module";

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgModule *
IgModule::builtin (void)
{
    static IgModule *myself = new IgModule (IgPluginManager::get (),
					    "" /* magic library name */,
					    "0", "good");
    return myself;
}

IgModule::IgModule (IgPluginManager *manager,
		    const lat::Filename &libraryName,
		    const std::string &time,
		    const std::string &flag)
    : m_manager (manager),
      m_libraryName (libraryName),
      m_library (0),
      m_cache (0),
      m_time (strtoul (time.c_str (), 0, 10)),
      m_bad (flag != "good"),
      m_attached (libraryName.empty ()),
      m_definition (0)
{
    ASSERT (flag == "good" || flag == "bad");
    ASSERT (! time.empty ());
    LOG (0, trace, LFplugin_manager,
	 "constructing module for library `" << m_libraryName << "'\n");
    makeCache ();
}

IgModule::~IgModule (void)
{
    LOG (0, trace, LFplugin_manager,
         "destroying module for library `" << m_libraryName << "'\n");

    unload ();
    ASSERT (! m_library);

    while (! m_infos.empty ())
	delete *m_infos.begin ();

    delete m_cache;
}

//////////////////////////////////////////////////////////////////////
IgPluginManager *
IgModule::manager (void) const
{ return m_manager; }

const lat::Filename &
IgModule::libraryName (void) const
{ return m_libraryName; }

/** Check if the module registration has been marked bad.  */
bool
IgModule::bad (void) const
{ return m_bad; }

/** Mark the module registration good or bad according to @a value.
    Setting the value to @c true resets the module contents.  */
void
IgModule::bad (bool value)
{ if ((m_bad = value)) clearCache (); }

lat::SharedLibrary *
IgModule::library (void) const
{
    // FIXME: return SharedLibrary::self if m_libraryName is empty?
    if (m_libraryName.empty ())
	return 0;

    if (! m_library)
    {
	LOG (0, trace, LFplugin_manager,
	     "loading shared library `" << m_libraryName << "'\n");

	try
	{
	    m_manager->feedback (IgPluginManager::StatusLoading, m_libraryName);
	    // FIXME: remove cast when filename is in the same namespace/gcc bug fixed
	    m_library = lat::SharedLibrary::load ((const std::string &) m_libraryName);
	}
	catch (lat::Error &e)
	{
	    m_manager->feedback (IgPluginManager::ErrorLoadFailure, m_libraryName, &e);
	    throw IgPluginError (e.clone ());
	}
    }

    ASSERT (m_library);
    return m_library;
}

bool
IgModule::loaded (void) const
{ return m_libraryName.empty () || m_library;}

void
IgModule::load (void)
{
    if (m_libraryName.empty ())
	return;

    LOG (0, trace, LFplugin_manager, "loading `" << m_libraryName << "'\n");

    VERIFY (library ());
    VERIFY (checkEntryPoints ());
}

void
IgModule::unload (void)
{
    if (m_libraryName.empty ())
	return;

    detach ();

    // Do not unload libraries on platforms with broken handling of
    // global destructors in shared libraries (e.g. KCC 4.0f on
    // Linux).  The problem is that global destructors are not
    // deregistered on library unload, causing instability with
    // subsequent dynamic loading operations, program exit etc.
    if (m_library)
    {
        LOG (0, trace, LFplugin_manager, "unloading `" << m_libraryName << "'\n");

	if (lat::ShellEnvironment ().has ("IGUANA_RELEASE_MODULES"))
	{
	    m_manager->feedback (IgPluginManager::StatusUnloading, m_libraryName);
	    m_library->release ();
	}
	else
	    m_library->abandon ();

	m_library = 0;
    }
}

//////////////////////////////////////////////////////////////////////
void
IgModule::makeCache (void)
{
    char time [40]; // 128+ bits
    const char *bad = m_bad ? "bad" : "good";
    sprintf (time, "%lu", (unsigned long) m_time);

    // Ask each module to dump itself under myself.
    m_cache = new IgModuleDescriptor (0, s_tag,
				      m_libraryName.nondirectory ().name (),
				      time, bad);
}

void
IgModule::clearCache (void)
{ delete m_cache; makeCache (); }

IgModuleDescriptor *
IgModule::cacheRoot (void) const
{ return m_cache; }

//////////////////////////////////////////////////////////////////////
IgModule::InfoIterator
IgModule::beginInfos (void) const
{ return m_infos.begin (); }

IgModule::InfoIterator
IgModule::endInfos (void) const
{ return m_infos.end (); }

void
IgModule::addInfo (IgPluginInfo *info, bool needCache)
{
    if (needCache)
	// This info isn't cached yet -- add it.
	info->cache (cacheRoot ());

    m_infos.push_back (info);
    m_manager->addInfo (info);
}

void
IgModule::detachInfo (IgPluginInfo *info)
{
    // Remove from the list but keep the details in the cache.  Also
    // notify the plug-in database so that this gets removed from the
    // factories.
    InfoList::iterator pos = std::find (m_infos.begin (), m_infos.end (), info);
    ASSERT (pos != m_infos.end ());
    m_infos.erase (pos);
    m_manager->removeInfo (info);
}

//////////////////////////////////////////////////////////////////////
void
IgModule::restore (IgModuleDescriptor *from)
{
    // FIXME: If factory is active, this will result in creation of a new
    // info, which we'll want to add to our cache.  Make sure we don't
    // make duplicates...
    from->dup (m_cache);
    m_manager->restore (this, from);
}

/** Dump the contents of the module registration into the cache @a to.  */
void
IgModule::cache (IgModuleDescriptor *to) const
{
    // Internal cache is already in sync (we update it every time a
    // new info item is created), so let base class do all the work.
    m_cache->dup (to);
}

//////////////////////////////////////////////////////////////////////
bool
IgModule::checkEntryPoints (void) const
{
    // Check for module entry points.  It's legal to have just one or both
    // of the normal and capability entry points, but not to have neither.
    // First check for the capability entry point, then for the standard one.
    // If either succeeds, we declare success.  If both fail, we record the
    // error from the standard entry point for more meaningful errors.
    m_manager->feedback (IgPluginManager::StatusEntries, m_libraryName);

    try
    {
	if (library ()->function (s_capEntryPoint))
	    return true;
    }
    catch (lat::Error &)
    {
    }

    try
    {
	return library ()->function (s_modEntryPoint);
    }
    catch (lat::Error &e)
    {
	m_manager->feedback (IgPluginManager::ErrorEntryFailure, m_libraryName, &e);
	throw IgPluginError (e.clone ());
    }
}

IgModuleEntry
IgModule::moduleEntryPoint (void)
{
    lat::SharedLibrary::Function entry = 0;

    if (! libraryName ().empty ()) 
    {
	// Swallow errors, just return null if it fails
	try { entry = library ()->function (s_modEntryPoint); }
	catch (lat::Error &) {}

	LOG (0, trace, LFplugin_manager, "module entry point ("
	     << s_modEntryPoint << ") = " << (void *) entry << '\n');
    }

    return reinterpret_cast<IgModuleEntry> (entry);
}

IgModuleCapEntry
IgModule::capabilityEntryPoint (void)
{
    lat::SharedLibrary::Function entry = 0;

    if (! libraryName ().empty ()) 
    {
	// Swallow errors, just return null if it fails
	try { entry = library ()->function (s_capEntryPoint); }
	catch (lat::Error &) {}

	LOG (0, trace, LFplugin_manager, "capability entry point ("
	     << s_capEntryPoint << ") = " << (void *) entry << '\n');
    }

    return reinterpret_cast<IgModuleCapEntry> (entry);
}

void
IgModule::attach (void)
{
    if (! m_attached && ! libraryName ().empty ())
    {
	ASSERT (! m_definition);

	LOG (0, trace, LFplugin_manager,
	     "attaching module " << libraryName () << '\n' << lat::indent);

	// Capability-only modules don't have a normal entry; we
	// don't need a definition for them either.
	if (IgModuleEntry entry = moduleEntryPoint ())
	{
	    m_definition = (*entry) ();
	    ASSERT (m_definition);

	    m_manager->feedback (IgPluginManager::StatusAttaching, m_libraryName);
	    m_definition->bind (this);
	    m_definition->attach ();
	}

	m_attached = true;
	LOG (0, trace, LFplugin_manager, lat::undent);
    }
}

void
IgModule::detach (void)
{
    if (loaded () && m_attached && ! libraryName ().empty ())
    {
	LOG (0, trace, LFplugin_manager,
	     "detaching module " << libraryName () << '\n' << lat::indent);

	m_manager->feedback (IgPluginManager::StatusDetaching, m_libraryName);
	for (InfoIterator i = m_infos.begin (); i != m_infos.end (); ++i)
	    (*i)->detach ();

	// Capability-only modules don't have a definition.
	if (m_definition)
	{
	    m_definition->detach ();
	    m_definition->release ();
	    m_definition = 0;
	}
	LOG (0, trace, LFplugin_manager, lat::undent);
    }
}

void
IgModule::query (void)
{
    if (! libraryName ().empty ()) 
    {
	ASSERT (! m_definition);

	LOG (0, trace, LFplugin_manager,
	     "querying module " << libraryName () << '\n' << lat::indent);

	// Blast away existing infos and the cache in order to rebuild
	// a clean state from the newly created infos.
	clearCache ();
	while (! m_infos.empty ())
	    delete *m_infos.begin ();

	// Now invoke the standard entry point.
	if (IgModuleEntry entry = moduleEntryPoint ())
	{
	    m_definition = (*entry) ();
	    ASSERT (m_definition);

	    m_manager->feedback (IgPluginManager::StatusQuerying, m_libraryName);
	    m_definition->bind (this);
	    m_definition->query ();
	    m_definition->release ();
	    m_definition = 0;
	}

	// Repeat with capabilities entry, processing it manually as
	// if the code was in the entry point in the module library.
	// This allows libraries to define "caability" modules with
	// pure string labels without linking against us -- but not
	// anything that needs a factory.  These would normally be
	// used for side-effects or libraries that self-register to
	// some other factory.  NB: The code below creates a module
	// definition even if the module has a standard entry point
	// and thus an existing definition.  Since this is a query,
	// different defs don't matter, and attach will never use
	// the def anyway.
	if (IgModuleCapEntry capEntry = capabilityEntryPoint ())
	{
	    IgModuleDef	*def = new IgModuleDef;
	    const char	**names = 0;
	    int		n = -1;

	    (*capEntry) (names, n);

	    def->bind (this);
	    for (int i = 0; names && names [i] && (n < 0 || i < n); ++i)
		IgPluginCapabilities::get ()->declare (def, names [i]);
	    def->release ();
	    delete def;
	}

	LOG (0, trace, LFplugin_manager, lat::undent);
    }
}

bool
IgModule::attached (void)
{ return m_attached; }
