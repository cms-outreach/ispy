#ifndef IGUANA_FRAMEWORK_IG_PLUGIN_DB_H
# define IGUANA_FRAMEWORK_IG_PLUGIN_DB_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "classlib/iobase/Filename.h"
# include "classlib/utils/MapIterator.h"
# include "classlib/utils/SearchPath.h"
# include "classlib/utils/Callback.h"
# include "classlib/utils/Error.h"
# include <string>
# include <list>
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgModule;
class IgModuleCache;
class IgModuleDescriptor;
class IgPluginFactoryBase;
class IgPluginInfo;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Catalog of dynamically available components in the system.

    #IgPluginManager maintains a catalog of objects that can be
    created dynamically.  Most of the objects are provided by plug-in
    modules: dynamically loaded shared libraries.  Subclasses of
    #IgPluginFactoryBase provided concrete views of the catalog.

    There is only one instance of this type, provided by #get().  */
class IgPluginManager
{
public:
    typedef std::list<IgModuleCache *>		DirectoryMap;
    typedef DirectoryMap::const_iterator	DirectoryIterator;

    typedef std::map<lat::Filename, IgModule *>	ModuleMap;
    typedef lat::MapValueIterator<ModuleMap>	ModuleIterator;

    /// Types of information passed as feedback.
    enum FeedbackCode
    {
	// Status feedback: no error object
	StatusLoading,		//< About to load a module library
	StatusUnloading,	//< About to unload a module library
	StatusEntries,		//< About to check for entry points
	StatusAttaching,	//< About to attach a module library
	StatusDetaching,	//< About to detach a module library
	StatusQuerying,		//< About to query a module library

	// Error feedback: see the passed error object
	ErrorLoadFailure,	//< Module library failed to load
	ErrorEntryFailure,	//< Missing entry points in the module
	ErrorBadModule,		//< Module is being marked bad
	ErrorBadCacheFile,	//< Bad or inaccessible cache file
	ErrorNoFactory,		//< Missing factory for a plug-in

	// Fallback
	Other			//< Something else
    };

    /// Simple tuple of data passed to feedback clients.
    struct FeedbackData
    {
	FeedbackData (FeedbackCode code, const std::string &s, lat::Error *err = 0);

	FeedbackCode	code;	//< Feedback code.
	std::string	scope;	//< Subject, e.g. module or file name.
	lat::Error	*error;	//< Possible error object.
    };

    /// Feedback callback type
    typedef lat::Callback1<FeedbackData> FeedbackCB;

    static IgPluginManager *get (void);
    static void		destroyOnExit (bool destroy);

    // cache management interface
    void		initialise (void);
    void		refresh (void);

    // feedback management
    void		addFeedback (FeedbackCB callback);
    void		removeFeedback (FeedbackCB callback);
    void		feedback (FeedbackData data);
    void		feedback (FeedbackCode code,
		    		  const lat::Filename &scope,
				  lat::Error *error = 0);
    void		feedback (FeedbackCode code,
		    		  const std::string &scope,
				  lat::Error *error = 0);
 
    // access to db structure
    DirectoryIterator	beginDirectories (void) const;
    DirectoryIterator	endDirectories (void) const;
    DirectoryIterator	locateDirectory (const lat::Filename &name) const;
    IgModuleCache *	directory (const lat::Filename &name) const;

    ModuleIterator	beginModules (void) const;
    ModuleIterator	endModules (void) const;
    ModuleIterator	locateModule (const lat::Filename &libraryName) const;
    IgModule *		module (const lat::Filename &libraryName) const;

    // interface for factory management; mainly private to implementation
    void		addInfo (IgPluginInfo *info);
    void		removeInfo (IgPluginInfo *info);
    void		restore (IgModule *module, IgModuleDescriptor *from);

    void		addFactory (IgPluginFactoryBase *factory);
    void		removeFactory (IgPluginFactoryBase *factory);

private:
    typedef std::list<IgPluginFactoryBase *>	FactoryList;
    typedef FactoryList::iterator		FactoryIterator;

    typedef std::list<FeedbackCB>		FeedbackList;
    typedef FeedbackList::iterator		FeedbackIterator;

    friend class IgPluginManagerDestructor;
    IgPluginManager (const lat::SearchPath &path);
    ~IgPluginManager (void);

    void		rebuild (void);
    FactoryIterator	beginFactories (void);
    FactoryIterator	endFactories (void);
    IgPluginFactoryBase *findFactory (const std::string &name);

    bool		m_initialised;
    lat::SearchPath	m_searchPath;
    DirectoryMap	m_directories;
    ModuleMap		m_modules;
    FactoryList		m_factories;
    FeedbackList	m_feedbacks;

    // undefined semantics
    IgPluginManager (const IgPluginManager &);
    IgPluginManager &operator= (const IgPluginManager &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
IgPluginManager::FeedbackData::FeedbackData (FeedbackCode c,
					     const std::string &s,
					     lat::Error *err /* = 0 */)
    : code (c),
      scope (s),
      error (err)
{}

#endif // IGUANA_FRAMEWORK_IG_PLUGIN_DB_H
