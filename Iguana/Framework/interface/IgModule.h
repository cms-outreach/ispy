#ifndef IGUANA_FRAMEWORK_IG_PLUGIN_H
# define IGUANA_FRAMEWORK_IG_PLUGIN_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "Iguana/Framework/interface/IgPluginInfo.h"
# include "Iguana/Framework/interface/IgModuleDescriptor.h"
# include "classlib/iobase/Filename.h"
# include "classlib/utils/SharedLibrary.h"
# include <string>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgModule;
class IgModuleDef;
class IgModuleCache;
class IgModuleDescriptor;
class IgPluginManager;

/** The type of the standard entry point function in a module library.  */
extern "C" { typedef IgModuleDef *(*IgModuleEntry) (void); }

/** The type of the capability entry point function in a module library.  */
extern "C" { typedef void (*IgModuleCapEntry) (const char **&names, int &n); }

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Module proxy.

    This class maintains the list of the capabilities provided by a
    plug-in module, but only in a generic manner.  Custom plug-in
    factories provide access to specific information.  The two are
    coordinated so that as new modules enter and leave the system the
    factory views are synchronised, and as new factories enter and
    leave, the information is cached and uncached automatically.

    A #IgModule exists for each module in an #IgModuleCache regardless
    of whether the module library is presently loaded.  The module is
    queried for the list of properties it provides (see #IgModuleDef,
    #DEFINE_IGUANA_MODULE, #DEFINE_IGUANA_PLUGIN).  #IgModuleDirectory
    caches the information for later accesses without having to query
    the module. If the module changes, it us automatically queried
    again to update the cache.

    #IgModule can also treat the program itself as a module through
    the #builtin() method.  This allows dynamic loading to be bypassed
    as long as clients always use custom #IgPluginManager factories and
    #IgPluginLoader.  Defining #IGUANA_BUILTIN_PLUGINS preprocessor
    macro causes #DEFINE_IGUANA_MODULE() and #DEFINE_SEAL_PLUGIN()
    to a variant that automatically does so.

    It is not safe to detach a module if there are any references
    remaining to it.  This means there can be no live objects whose
    implementation for any part, including but not limited to virtual
    function tables, methods, template functions or static data,
    belong to the library detached.  Note: this applies to any object
    created by the library, not just the plug-ins created by it!

    For purposes of #IgModuleCache the object remembers the time
    stamp of the module file when it was last examined, and whether
    it is good or bad.  "Bad" means either that the file is corrupt
    in some way, for example the module could not be loaded.  */
class IgModule
{
    typedef std::list<IgPluginInfo *>	InfoList;
public:
    typedef InfoList::const_iterator	InfoIterator;

    IgModule (IgPluginManager *manager,
	      const lat::Filename &libraryName,
	      const std::string &time,
	      const std::string &flag);
    virtual ~IgModule (void);

    // module management interface
    IgPluginManager *	manager (void) const;
    const lat::Filename &libraryName (void) const;
    unsigned		time (void) const;
    bool		bad (void) const;
    void		bad (bool value);

    virtual void	load (void);
    virtual void	unload (void);
    virtual void	attach (void);
    virtual void	detach (void);
    virtual void	query (void);

    virtual bool	attached (void);

    // program built-in module (= program itself)
    static IgModule *	builtin (void);

    // info access
    InfoIterator	beginInfos (void) const;
    InfoIterator	endInfos (void) const;
    void		addInfo (IgPluginInfo *info, bool cache);
    void		detachInfo (IgPluginInfo *info);
    IgModuleDescriptor *cacheRoot (void) const;

protected:
    friend class IgModuleCache;
    void		makeCache (void);
    void		clearCache (void);
    virtual void	restore (IgModuleDescriptor *info);
    virtual void	cache (IgModuleDescriptor *to) const;

    virtual bool	checkEntryPoints (void) const;
    IgModuleEntry	moduleEntryPoint (void);
    IgModuleCapEntry	capabilityEntryPoint (void);

    lat::SharedLibrary *library (void) const;
    bool		loaded (void) const;

    static const char *	tag (void);

private:
    IgPluginManager	*m_manager;
    lat::Filename	m_libraryName;
    mutable lat::SharedLibrary *m_library;
    IgModuleDescriptor	*m_cache;

    unsigned		m_time;		//< Registration file time stamp
    bool		m_bad;		//< Flag to mark if the file was bad
    bool		m_attached;	//< Set when library has been attached.
    IgModuleDef		*m_definition;	//< My definition proxy.
    InfoList		m_infos;	//< Info items for this module.
    static const char	s_modEntryPoint [];//< Name of the normal library entry point.
    static const char	s_capEntryPoint [];//< Name of the capability library entry point.
    static const char	s_tag [];

    // undefined semantics
    IgModule (const IgModule &);
    IgModule &operator= (const IgModule &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline const char *
IgModule::tag (void)
{ return s_tag; }

/** Return the registration file time stamp.  */
inline unsigned
IgModule::time (void) const
{ return m_time; }

#endif // IGUANA_FRAMEWORK_IG_PLUGIN_H
