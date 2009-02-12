//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgPluginFactoryBase.h"
#include "Iguana/Framework/interface/IgPluginManager.h"
#include "Iguana/Framework/interface/IgModuleDescriptor.h"
#include "Iguana/Framework/interface/IgModuleCache.h"
#include "Iguana/Framework/interface/IgModule.h"
#include "classlib/utils/DebugAids.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgPluginFactoryBase::IgPluginFactoryBase (const std::string &tag)
    : m_tag (tag)
{
    ASSERT (IgPluginManager::get ());
    ASSERT (! m_tag.empty ());
    IgPluginManager::get ()->addFactory (this);
}

IgPluginFactoryBase::~IgPluginFactoryBase (void)
{
    ASSERT (IgPluginManager::get ());
    ASSERT (! m_tag.empty ());
    IgPluginManager::get ()->removeFactory (this);
}

const std::string &
IgPluginFactoryBase::category (void) const
{ return m_tag; }

void
IgPluginFactoryBase::rebuild (void)
{
    IgPluginManager			*db = IgPluginManager::get ();
    IgPluginManager::DirectoryIterator	dir;
    IgModuleCache::Iterator		module;
    IgModuleDescriptor			*cache;
    unsigned				i;

    // The modules cannot have infos already cached that we need to
    // avoid recreating.  This is because the infos can be created
    // only in one of two ways, and in either case the factory knows
    // *and* has already cleared out those infos in the derived
    // rebuild() before invoking us.  Infos restored from the cache
    // are managed by the factory, and thus the known and already
    // deleted.  When infos are created via the factory's describe()
    // method, the factory must already exist and thus the infos are
    // automatically registered (and thus already deleted).  The
    // latter is because the factory must be in the same library as the
    // one invoking describe(), or one of its dependents; either way
    // since the factory is a global object, it has been constructed (the
    // query happens after global constructors).
    //
    // Which brings us back to the point: there cannot be infos we
    // care about at this point.  As we start, the derived factory has no
    // infos (guaranteed by derived rebuild()), so the only infos that
    // exist are those we create here below.
    for (dir = db->beginDirectories (); dir != db->endDirectories (); ++dir)
	for (module = (*dir)->begin (); module != (*dir)->end (); ++module)
	    for (cache=(*module)->cacheRoot(), i=0; i < cache->children(); ++i)
		if (cache->child (i)->token (0) == category ())
		    restore (*module, cache->child (i));
}
