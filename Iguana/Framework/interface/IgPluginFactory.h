#ifndef IGUANA_FRAMEWORK_IG_PLUGIN_DB_VIEW_H
# define IGUANA_FRAMEWORK_IG_PLUGIN_DB_VIEW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgMapUniqueIterator.h"
# include "Iguana/Framework/interface/IgPluginFactoryBase.h"
# include "Iguana/Framework/interface/IgModuleDef.h"
# include "Iguana/Framework/interface/IgModule.h"
# include "classlib/utils/DebugAids.h"
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

/** Plug-in factory base class.

    The plug-in manager is used through factories derived from this
    class.  Each factory represents a category of objects that can be
    created as a plug-in object.  The plug-in manager is in some sense
    merely a set of factories.  The clients of a factory of type T can
    create instances derived from T, just as in any factory.  Only
    with the plug-in manager the implementations for those classes can
    be loaded on demand from a shared library behind the scenes.
    Clients of the factory are thus decoupled from the physical
    decomposition of the application.

    To define a new plug-in category for some type T, with constructor
    arguments A and B, inherit your factory from IgPluginFactory<T *(A,
    B)>.  This base class provides all the functionality for
    integration to #IgPluginManager, managing modules and available
    plug-in objects, iteration over registered plug-ins and the @c
    create() methods to instantiate them.  Please refer to the
    separate how-to documentation for a more detailed example.

    The plug-in manager restricts in no way the types of objects that
    can be created through the factories.  It is possible (though not
    very meaningful) to create instances of the C++ scalar types like
    @c int via the plug-in mechanism.

    Each factory is a globally constructed singleton object like the
    plug-in manager itself (and not a pointer, nor a function-local
    static object).  This strategy is required because shared-library
    loading is an application-global operation and to ensure that the
    factory is always valid if it exists: the factories may come and
    go several times during a single program execution if modules are
    loaded and unloaded, for example during module queries by the
    caching mechanism.  Factories do not have an abstract interface
    unlike plug-ins, they are normal types that are always accessed
    through a static interface.  Clients using a particular factory
    should just use it directly without intermediaries.  It makes no
    sense to try to abstract the factory away; an attempt to do so is
    most likely a misunderstanding.  */
template <class Proto> class IgPluginFactory;
template <class Proto> class IgPluginFactoryImpl;
template <class Proto> class IgPluginFactoryImplTypes;
template <class Types> class IgPluginFactoryImplBase;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Specialisations of IgPluginInfo for various argument combinations.  Up
// to three arguments are supported now.  Arbitrary many *could* be
// supported, it's just a question of tedious work to make it happen.
// (See boost absolute and total preprocessor abuse package for another
// solution that scales up to 256 arguments -- it results in totally
// incomprehensible code, both before and after expansion.)
//
// The IgPluginFactoryImplTypes defines helper types needed to implement
// a particular plug-in factory kind.  The template argument is a
// function prototype (return value and argument list), or just "void"
// for factory that never creates anything, only loads modules.
template <>
class IgPluginFactoryImplTypes<void>
{
public:
    class Info : public IgPluginInfo
    {
    public:
	Info (IgModule *module, const std::string &name, const std::string &tag);
	Info (IgModule *module, IgModuleDescriptor *details, const std::string &tag);
	~Info (void);
    };
};

template <class R>
class IgPluginFactoryImplTypes<R * (void)>
{
public:
    typedef R Object;
    class Factory {
    public:
	virtual ~Factory (void);
	virtual R *create (void) = 0;
    };

    template <class T> class AutoFactory : public Factory
    {
    public:
	virtual R *create (void);
    };

    class Info : public IgPluginInfo
    {
    public:
	Info (IgModule *module, const std::string &name, const std::string &tag);
	Info (IgModule *module, IgModuleDescriptor *details, const std::string &tag);
	~Info (void);

	R *		create (void);
	void		attach (Factory *factory);
	virtual void	detach (void);
	

    private:
	Factory *	factory (void) const;
	Factory		*m_factory;	/*< Factory for creating instances
					    of this type, set when the
					    module is attached.  */
    };
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1>
class IgPluginFactoryImplTypes<R * (T1)>
{
public:
    typedef R Object;
    class Factory {
    public:
	virtual ~Factory (void);
	virtual R *create (T1 a1) = 0;
    };

    template <class T> class AutoFactory : public Factory
    {
    public:
	virtual R *create (T1 a1);
    };

    class Info : public IgPluginInfo
    {
    public:
	Info (IgModule *module, const std::string &name, const std::string &tag);
	Info (IgModule *module, IgModuleDescriptor *details, const std::string &tag);
	~Info (void);

	R *		create (T1 a1);
	void		attach (Factory *factory);
	virtual void	detach (void);

    private:
	Factory *	factory (void) const;
	Factory		*m_factory;	/*< Factory for creating instances
					    of this type, set when the
					    module is attached.  */
    };
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
class IgPluginFactoryImplTypes<R * (T1, T2)>
{
public:
    typedef R Object;
    class Factory {
    public:
	virtual ~Factory (void);
	virtual R *create (T1 a1, T2 a2) = 0;
    };

    template <class T> class AutoFactory : public Factory
    {
    public:
	virtual R *create (T1 a1, T2 a2);
    };

    class Info : public IgPluginInfo
    {
    public:
	Info (IgModule *module, const std::string &name, const std::string &tag);
	Info (IgModule *module, IgModuleDescriptor *details, const std::string &tag);
	~Info (void);

	R *		create (T1 a1, T2 a2);
	void		attach (Factory *factory);
	virtual void	detach (void);

    private:
	Factory *	factory (void) const;
	Factory		*m_factory;	/*< Factory for creating instances
					    of this type, set when the
					    module is attached.  */
    };
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3>
class IgPluginFactoryImplTypes<R * (T1, T2, T3)>
{
public:
    typedef R Object;
    class Factory {
    public:
	virtual ~Factory (void);
	virtual R *create (T1 a1, T2 a2, T3 a3) = 0;
    };

    template <class T> class AutoFactory : public Factory
    {
    public:
	virtual R *create (T1 a1, T2 a2, T3 a3);
    };

    class Info : public IgPluginInfo
    {
    public:
	Info (IgModule *module, const std::string &name, const std::string &tag);
	Info (IgModule *module, IgModuleDescriptor *details, const std::string &tag);
	~Info (void);

	R *		create (T1 a1, T2 a2, T3 a3);
	void		attach (Factory *factory);
	virtual void	detach (void);

    private:
	Factory *	factory (void) const;
	Factory		*m_factory;	/*< Factory for creating instances
					    of this type, set when the
					    module is attached.  */
    };
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3, class T4>
class IgPluginFactoryImplTypes<R * (T1, T2, T3, T4)>
{
public:
    typedef R Object;
    class Factory {
    public:
	virtual ~Factory (void);
	virtual R *create (T1 a1, T2 a2, T3 a3, T4 a4) = 0;
    };

    template <class T> class AutoFactory : public Factory
    {
    public:
	virtual R *create (T1 a1, T2 a2, T3 a3, T4 a4);
    };

    class Info : public IgPluginInfo
    {
    public:
	Info (IgModule *module, const std::string &name, const std::string &tag);
	Info (IgModule *module, IgModuleDescriptor *details, const std::string &tag);
	~Info (void);

	R *		create (T1 a1, T2 a2, T3 a3, T4 a4);
	void		attach (Factory *factory);
	virtual void	detach (void);

    private:
	Factory *	factory (void) const;
	Factory		*m_factory;	/*< Factory for creating instances
					    of this type, set when the
					    module is attached.  */
    };
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Definition on the main factory implementation base class.  This
// class defines all the utilities for maintaining info objects,
// including their declarations from modules, but includes no support
// for creating objects or accessing object factories.

template <class Types>
class IgPluginFactoryImplBase : public IgPluginFactoryBase
{
public:
    /** The info type.  */
    typedef typename Types::Info		Info;

    /** Info table type.  Keyed by the plug-in registration name.  The table
	is always built such that modules earlier in the module search path
	occur earlier in this table; the first interesting info item to use
	is the first of the multiple entries.  */
    typedef std::multimap<std::string, Info *>	Map;

    /** Type of the element in the info table.  */
    typedef typename Map::value_type		MapValue;

    /** Iterator over module data, filters out duplicate entries if
        the same plug-in is available from multiple modules, such as
        released and privately re-built modules. */
    typedef IgMapUniqueIterator<Map>		Iterator;

    IgPluginFactoryImplBase (const std::string &tag);
    ~IgPluginFactoryImplBase (void);

    Iterator		begin (void) const;
    Iterator		end (void) const;
    Iterator		locate (const std::string &name) const;
    Info *		info (const std::string &name) const;

    virtual void	declare (IgModuleDef *def, std::string name);

    // Notifications from IgPluginManager.
    virtual void	rebuild (void);
    virtual void	restore (IgModule *module, IgModuleDescriptor *info);
    virtual void	addInfo (IgPluginInfo *info);
    virtual void	removeInfo (IgPluginInfo *info);

private:
    Map			m_map;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Intermediate implementation base class for factories that actually
// instantiate objects.  Adds factories atop IgPluginFactoryImplBase.

template <class Proto>
class IgPluginFactoryImpl
    : public IgPluginFactoryImplBase< IgPluginFactoryImplTypes<Proto> >
{
public:
    /** Alias type definition helper for more convenient access.  */
    typedef IgPluginFactoryImplTypes<Proto>	Types;

    /** The object we create.  */
    typedef typename Types::Object		Object;

    /** The basic factory type. */
    typedef typename Types::Factory		Factory;

    /** The info type.  */
    typedef typename Types::Info		Info;

    IgPluginFactoryImpl (const std::string &name);

    virtual void	installFactory (IgModuleDef *def,
					std::string name,
					Factory *factory);
};

//////////////////////////////////////////////////////////////////////
// Custom specialisation for "void", i.e. no factory.

template <>
class IgPluginFactoryImpl<void>
    : public IgPluginFactoryImplBase< IgPluginFactoryImplTypes<void> >
{
public:
    typedef IgPluginFactoryImplTypes<void>	Types;

    IgPluginFactoryImpl (const std::string &name);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Final client-visible factory base classes, specialised for various
// constructor argument types.  These add the actual load()/create()
// method signatures and thus have a specialisation for every possible
// number of arguments (currently zero to three; see above for comments
// on why this is not expanded to more arguments).

template <>
class IgPluginFactory<void> : public IgPluginFactoryImpl<void>
{
public:
    IgPluginFactory (const std::string &name);

    virtual void	load (Iterator info) const;
    virtual void	load (const std::string &name) const;
};

//////////////////////////////////////////////////////////////////////
template <class R>
class IgPluginFactory<R * (void)> : public IgPluginFactoryImpl<R * (void)>
{
 public:
    typedef typename IgPluginFactoryImpl<R * (void)>::Iterator Iterator;
    IgPluginFactory (const std::string &name);

    virtual R *		create (Iterator info) const;
    virtual R *		create (const std::string &name) const;
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1>
class IgPluginFactory<R * (T1)> : public IgPluginFactoryImpl<R * (T1)>
{
 public:
    typedef typename IgPluginFactoryImpl<R * (T1)>::Iterator Iterator;
    IgPluginFactory (const std::string &name);

    virtual R *		create (const std::string &name, T1 a1) const;
    virtual R *		create (Iterator item, T1 a1) const;
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
class IgPluginFactory<R * (T1, T2)> : public IgPluginFactoryImpl<R * (T1, T2)>
{
 public:
    typedef typename IgPluginFactoryImpl<R * (T1, T2)>::Iterator Iterator;
    IgPluginFactory (const std::string &name);

    virtual R *		create (const std::string &name, T1 a1, T2 a2) const;
    virtual R *		create (Iterator item, T1 a1, T2 a2) const;
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3>
class IgPluginFactory<R * (T1, T2, T3)> : public IgPluginFactoryImpl<R * (T1, T2, T3)>
{
 public:
    typedef typename IgPluginFactoryImpl<R * (T1, T2, T3)>::Iterator Iterator;
    IgPluginFactory (const std::string &name);

    virtual R *		create (const std::string &name, T1 a1, T2 a2, T3 a3) const;
    virtual R *		create (Iterator item, T1 a1, T2 a2, T3 a3) const;
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3, class T4>
class IgPluginFactory<R * (T1, T2, T3, T4)> : public IgPluginFactoryImpl<R * (T1, T2, T3, T4)>
{
 public:
    typedef typename IgPluginFactoryImpl<R * (T1, T2, T3, T4)>::Iterator Iterator;
    IgPluginFactory (const std::string &name);

    virtual R *		create (const std::string &name, T1 a1, T2 a2, T3 a3, T4 a4) const;
    virtual R *		create (Iterator item, T1 a1, T2 a2, T3 a3, T4 a4) const;
};

//////////////////////////////////////////////////////////////////////
template <class I>
class IgPluginFactory : public IgPluginFactoryBase
{
public:
    typedef I					Info;

    typedef typename I::Factory			Factory;

    typedef std::multimap<std::string, Info *>	Map;

    typedef typename Map::value_type		MapValue;
 
    typedef IgMapUniqueIterator<Map>		Iterator;

    IgPluginFactory (const std::string &tag);
    ~IgPluginFactory (void);

    Iterator		begin (void) const;
    Iterator		end (void) const;
    Iterator		locate (const std::string &name) const;
    Info *		info (const std::string &name) const;

    virtual void	declare (IgModuleDef *def, std::string name);
    virtual void	installFactory (IgModuleDef *def,
					std::string name,
					Factory *factory);

    virtual void	rebuild (void);
    virtual void	restore (IgModule *module, IgModuleDescriptor *info);
    virtual void	addInfo (IgPluginInfo *info);
    virtual void	removeInfo (IgPluginInfo *info);

private:
    Map			m_map;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Construct a new plug-in factory.  Derived class should pass the
    key by which items cached by this category are identified in the
    plug-in manager cache.  It should be unique across the entire
    software system, so if you provide a library, please make an
    effort to pick a suitably unique tag label. */
template <class Types>
inline
IgPluginFactoryImplBase<Types>::IgPluginFactoryImplBase (const std::string &tag)
    : IgPluginFactoryBase (tag)
{
    ASSERT (! tag.empty ());
    rebuild ();
}

/** Destroy the factory.  */
template <class Types>
inline
IgPluginFactoryImplBase<Types>::~IgPluginFactoryImplBase (void)
{
    // Remove all my infos.
    while (! m_map.empty ())
	delete m_map.begin ()->second;
}

template <class Types>
inline typename IgPluginFactoryImplBase<Types>::Iterator
IgPluginFactoryImplBase<Types>::begin (void) const
{ return Iterator (m_map.begin (), &m_map); }

template <class Types>
inline typename IgPluginFactoryImplBase<Types>::Iterator
IgPluginFactoryImplBase<Types>::end (void) const
{ return Iterator (m_map.end (), &m_map); }

template <class Types>
inline typename IgPluginFactoryImplBase<Types>::Iterator
IgPluginFactoryImplBase<Types>::locate (const std::string &name) const
{ return Iterator (m_map.find (name), &m_map); }

template <class Types>
inline typename IgPluginFactoryImplBase<Types>::Info *
IgPluginFactoryImplBase<Types>::info (const std::string &name) const
{ Iterator pos (locate (name)); return pos != end () ? *pos : 0; }

template <class Types>
inline void
IgPluginFactoryImplBase<Types>::declare (IgModuleDef *def, std::string name)
{
    // This will eventually come back to us, see comments in add().
    ASSERT (def);
    ASSERT (def->module ());
    ASSERT (! name.empty ());
    new Info (def->module (), name, category ());
}

template <class Types>
inline void
IgPluginFactoryImplBase<Types>::rebuild (void)
{
    // First remove all my infos
    while (! m_map.empty ())
	delete m_map.begin ()->second;

    // And now regenerate them from cache contents
    IgPluginFactoryBase::rebuild ();
}

template <class Types>
inline void
IgPluginFactoryImplBase<Types>::restore (IgModule *module, IgModuleDescriptor *info)
{
    // The info item registers itself in the module on creation,
    // causing IgPluginManager to be notified, and us getting notified
    // again via addInfo() below.  On exit the info item already
    // exists in our map.
    new Info (module, info, category ());
}

template <class Types>
inline void
IgPluginFactoryImplBase<Types>::addInfo (IgPluginInfo *info)
{
    // We insert entries in a multi-map because similar entries might
    // get inserted from multiple modules.  When modules are removed
    // we still have a list of previous choices.  The infos are used
    // in the order inserted, relying on the knowledge that IgPluginManager
    // always processes modules in preference order, i.e. first those
    // earlier in the search path, putting them first on our map's
    // list of values.
    ASSERT (dynamic_cast<Info *> (info));
    m_map.insert (MapValue (info->name (), static_cast<Info *> (info)));
}

template <class Types>
inline void
IgPluginFactoryImplBase<Types>::removeInfo (IgPluginInfo *info)
{
    // Remove the info from the appropriate position.
    typename Map::iterator pos = m_map.find (info->name ());
    while (pos != m_map.end ()
	   && pos->first == info->name ()
	   && pos->second != info)
	++pos;

    ASSERT (pos != m_map.end ());
    ASSERT (pos->second == info);
    m_map.erase (pos);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class Proto>
inline
IgPluginFactoryImpl<Proto>::IgPluginFactoryImpl (const std::string &tag)
    : IgPluginFactoryImplBase<Types> (tag)
{}

template <class Proto>
inline void
IgPluginFactoryImpl<Proto>::installFactory (IgModuleDef *def,
					    std::string name,
					    Factory *factory)
{
    ASSERT (def);

    Info *info = this->info (name);

    ASSERT (info);
    ASSERT (info->module ());
    ASSERT (info->module () == def->module ());
    ASSERT (! info->name ().empty ());
    ASSERT (info->name () == name);
    ASSERT (factory);

    info->attach (factory);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
inline
IgPluginFactoryImpl<void>::IgPluginFactoryImpl (const std::string &tag)
    : IgPluginFactoryImplBase<Types> (tag)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
inline
IgPluginFactory<void>::IgPluginFactory (const std::string &tag)
    : IgPluginFactoryImpl<void> (tag)
{}

inline void
IgPluginFactory<void>::load (Iterator info) const
{ if (info != this->end ()) (*info)->module ()->attach (); }

inline void
IgPluginFactory<void>::load (const std::string &name) const
{ load (this->locate (name)); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class R>
inline
IgPluginFactory<R * (void)>::IgPluginFactory (const std::string &tag)
    : IgPluginFactoryImpl<R * (void)> (tag)
{}

template <class R>
inline R *
IgPluginFactory<R * (void)>::create (Iterator info) const
{ return info == this->end () ? 0 : (*info)->create (); }

template <class R>
inline R *
IgPluginFactory<R * (void)>::create (const std::string &name) const
{ return create (this->locate (name)); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class R, class T1>
inline
IgPluginFactory<R * (T1)>::IgPluginFactory (const std::string &tag)
    : IgPluginFactoryImpl<R * (T1)> (tag)
{}

template <class R, class T1>
inline R *
IgPluginFactory<R * (T1)>::create (Iterator info, T1 a1) const
{ return info == this->end () ? 0 : (*info)->create (a1); }

template <class R, class T1>
inline R *
IgPluginFactory<R * (T1)>::create (const std::string &name, T1 a1) const
{ return create (this->locate (name), a1); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
inline
IgPluginFactory<R * (T1, T2)>::IgPluginFactory (const std::string &tag)
    : IgPluginFactoryImpl<R * (T1, T2)> (tag)
{}

template <class R, class T1, class T2>
inline R *
IgPluginFactory<R * (T1, T2)>::create (Iterator info, T1 a1, T2 a2) const
{ return info == this->end () ? 0 : (*info)->create (a1, a2); }

template <class R, class T1, class T2>
inline R *
IgPluginFactory<R * (T1, T2)>::create (const std::string &name, T1 a1, T2 a2) const
{ return create (this->locate (name), a1, a2); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3>
inline
IgPluginFactory<R * (T1, T2, T3)>::IgPluginFactory (const std::string &tag)
    : IgPluginFactoryImpl<R * (T1, T2, T3)> (tag)
{}

template <class R, class T1, class T2, class T3>
inline R *
IgPluginFactory<R * (T1, T2, T3)>::create (Iterator info, T1 a1, T2 a2, T3 a3) const
{ return info == this->end () ? 0 : (*info)->create (a1, a2, a3); }

template <class R, class T1, class T2, class T3>
inline R *
IgPluginFactory<R * (T1, T2, T3)>::create (const std::string &name, T1 a1, T2 a2, T3 a3) const
{ return create (this->locate (name), a1, a2, a3); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3, class T4>
inline
IgPluginFactory<R * (T1, T2, T3, T4)>::IgPluginFactory (const std::string &tag)
    : IgPluginFactoryImpl<R * (T1, T2, T3, T4)> (tag)
{}

template <class R, class T1, class T2, class T3, class T4>
inline R *
IgPluginFactory<R * (T1, T2, T3, T4)>::create (Iterator info, T1 a1, T2 a2, T3 a3, T4 a4) const
{ return info == this->end () ? 0 : (*info)->create (a1, a2, a3, a4); }

template <class R, class T1, class T2, class T3, class T4>
inline R *
IgPluginFactory<R * (T1, T2, T3, T4)>::create (const std::string &name, T1 a1, T2 a2, T3 a3, T4 a4) const
{ return create (this->locate (name), a1, a2, a3, a4); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
inline
IgPluginFactoryImplTypes<void>::Info::Info (IgModule *module,
					    const std::string &name,
					    const std::string &tag)
    : IgPluginInfo (module, name, tag)
{ finish (true); }

inline
IgPluginFactoryImplTypes<void>::Info::Info (IgModule *module,
					    IgModuleDescriptor *details,
					    const std::string &tag)
    : IgPluginInfo (module, details->token (1), tag)
{ ASSERT (details->tokens () == 2); finish (false); }

inline
IgPluginFactoryImplTypes<void>::Info::~Info (void)
{ detach (); }

//////////////////////////////////////////////////////////////////////
template <class R>
inline
IgPluginFactoryImplTypes<R * (void)>::Factory::~Factory (void)
{}

template <class R>
template <class T>
inline R *
IgPluginFactoryImplTypes<R * (void)>::AutoFactory<T>::create (void)
{ return new T; }

template <class R>
inline
IgPluginFactoryImplTypes<R * (void)>::Info::Info (IgModule *module,
						  const std::string &name,
						  const std::string &tag)
    : IgPluginInfo (module, name, tag),
      m_factory (0)
{ finish (true); }

template <class R>
inline
IgPluginFactoryImplTypes<R * (void)>::Info::Info (IgModule *module,
						  IgModuleDescriptor *details,
						  const std::string &tag)
    : IgPluginInfo (module, details->token (1), tag),
      m_factory (0)
{ ASSERT (details->tokens () == 2); finish (false); }

template <class R>
inline
IgPluginFactoryImplTypes<R * (void)>::Info::~Info (void)
{ detach (); }

template <class R>
inline R *
IgPluginFactoryImplTypes<R * (void)>::Info::create (void)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    return factory ()->create ();
}

template <class R>
inline void
IgPluginFactoryImplTypes<R * (void)>::Info::attach (Factory *factory)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    ASSERT (! m_factory);
    ASSERT (factory); 
    m_factory = factory;
}

template <class R>
inline void
IgPluginFactoryImplTypes<R * (void)>::Info::detach (void)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    delete m_factory;
    m_factory = 0;
}

template <class R>
inline typename IgPluginFactoryImplTypes<R * (void)>::Factory *
IgPluginFactoryImplTypes<R * (void)>::Info::factory (void) const
{
    ASSERT (module ());
    ASSERT (! name ().empty ());

    if (! m_factory)
	module ()->attach ();

    if (! m_factory)
	noFactory ();

    return m_factory;
}

//////////////////////////////////////////////////////////////////////
template <class R, class T1>
inline
IgPluginFactoryImplTypes<R * (T1)>::Factory::~Factory (void)
{}

template <class R, class T1>
template <class T>
inline R *
IgPluginFactoryImplTypes<R * (T1)>::AutoFactory<T>::create (T1 a1)
{ return new T (a1); }

template <class R, class T1>
inline
IgPluginFactoryImplTypes<R * (T1)>::Info::Info (IgModule *module,
					        const std::string &name,
					        const std::string &tag)
    : IgPluginInfo (module, name, tag),
      m_factory (0)
{ finish (true); }

template <class R, class T1>
inline
IgPluginFactoryImplTypes<R * (T1)>::Info::Info (IgModule *module,
					        IgModuleDescriptor *details,
					        const std::string &tag)
    : IgPluginInfo (module, details->token (1), tag),
      m_factory (0)
{ ASSERT (details->tokens () == 2); finish (false); }

template <class R, class T1>
inline
IgPluginFactoryImplTypes<R * (T1)>::Info::~Info (void)
{ detach (); }

template <class R, class T1>
inline R *
IgPluginFactoryImplTypes<R * (T1)>::Info::create (T1 a1)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    return factory ()->create (a1);
}

template <class R, class T1>
inline void
IgPluginFactoryImplTypes<R * (T1)>::Info::attach (Factory *factory)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    ASSERT (! m_factory);
    ASSERT (factory); 
    m_factory = factory;
}

template <class R, class T1>
inline void
IgPluginFactoryImplTypes<R * (T1)>::Info::detach (void)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    delete m_factory;
    m_factory = 0;
}

template <class R, class T1>
inline typename IgPluginFactoryImplTypes<R * (T1)>::Factory *
IgPluginFactoryImplTypes<R * (T1)>::Info::factory (void) const
{
    ASSERT (module ());
    ASSERT (! name ().empty ());

    if (! m_factory)
	module ()->attach ();

    if (! m_factory)
	noFactory ();

    return m_factory;
}

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
inline
IgPluginFactoryImplTypes<R * (T1, T2)>::Factory::~Factory (void)
{}

template <class R, class T1, class T2>
template <class T>
inline R *
IgPluginFactoryImplTypes<R * (T1, T2)>::AutoFactory<T>::create (T1 a1, T2 a2)
{ return new T (a1, a2); }

template <class R, class T1, class T2>
inline
IgPluginFactoryImplTypes<R * (T1, T2)>::Info::Info (IgModule *module,
						    const std::string &name,
						    const std::string &tag)
    : IgPluginInfo (module, name, tag),
      m_factory (0)
{ finish (true); }

template <class R, class T1, class T2>
inline
IgPluginFactoryImplTypes<R * (T1, T2)>::Info::Info (IgModule *module,
						    IgModuleDescriptor *details,
						    const std::string &tag)
    : IgPluginInfo (module, details->token (1), tag),
      m_factory (0)
{ ASSERT (details->tokens () == 2); finish (false); }

template <class R, class T1, class T2>
inline
IgPluginFactoryImplTypes<R * (T1, T2)>::Info::~Info (void)
{ detach (); }

template <class R, class T1, class T2>
inline R *
IgPluginFactoryImplTypes<R * (T1, T2)>::Info::create (T1 a1, T2 a2)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    return factory ()->create (a1, a2);
}

template <class R, class T1, class T2>
inline void
IgPluginFactoryImplTypes<R * (T1, T2)>::Info::attach (Factory *factory)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    ASSERT (! m_factory);
    ASSERT (factory); 
    m_factory = factory;
}

template <class R, class T1, class T2>
inline void
IgPluginFactoryImplTypes<R * (T1, T2)>::Info::detach (void)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    delete m_factory;
    m_factory = 0;
}

template <class R, class T1, class T2>
inline typename IgPluginFactoryImplTypes<R * (T1, T2)>::Factory *
IgPluginFactoryImplTypes<R * (T1, T2)>::Info::factory (void) const
{
    ASSERT (module ());
    ASSERT (! name ().empty ());

    if (! m_factory)
	module ()->attach ();

    if (! m_factory)
	noFactory ();

    return m_factory;
}

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3>
inline
IgPluginFactoryImplTypes<R * (T1, T2, T3)>::Factory::~Factory (void)
{}

template <class R, class T1, class T2, class T3>
template <class T>
inline R *
IgPluginFactoryImplTypes<R * (T1, T2, T3)>::AutoFactory<T>::create (T1 a1, T2 a2, T3 a3)
{ return new T (a1, a2, a3); }

template <class R, class T1, class T2, class T3>
inline
IgPluginFactoryImplTypes<R * (T1, T2, T3)>::Info::Info (IgModule *module,
						        const std::string &name,
						        const std::string &tag)
    : IgPluginInfo (module, name, tag),
      m_factory (0)
{ finish (true); }

template <class R, class T1, class T2, class T3>
inline
IgPluginFactoryImplTypes<R * (T1, T2, T3)>::Info::Info (IgModule *module,
						        IgModuleDescriptor *details,
						        const std::string &tag)
    : IgPluginInfo (module, details->token (1), tag),
      m_factory (0)
{ ASSERT (details->tokens () == 2); finish (false); }

template <class R, class T1, class T2, class T3>
inline
IgPluginFactoryImplTypes<R * (T1, T2, T3)>::Info::~Info (void)
{ detach (); }

template <class R, class T1, class T2, class T3>
inline R *
IgPluginFactoryImplTypes<R * (T1, T2, T3)>::Info::create (T1 a1, T2 a2, T3 a3)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    return factory ()->create (a1, a2, a3);
}

template <class R, class T1, class T2, class T3>
inline void
IgPluginFactoryImplTypes<R * (T1, T2, T3)>::Info::attach (Factory *factory)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    ASSERT (! m_factory);
    ASSERT (factory); 
    m_factory = factory;
}

template <class R, class T1, class T2, class T3>
inline void
IgPluginFactoryImplTypes<R * (T1, T2, T3)>::Info::detach (void)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    delete m_factory;
    m_factory = 0;
}

template <class R, class T1, class T2, class T3>
inline typename IgPluginFactoryImplTypes<R * (T1, T2, T3)>::Factory *
IgPluginFactoryImplTypes<R * (T1, T2, T3)>::Info::factory (void) const
{
    ASSERT (module ());
    ASSERT (! name ().empty ());

    if (! m_factory)
	module ()->attach ();

    if (! m_factory)
	noFactory ();

    return m_factory;
}

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3, class T4>
inline
IgPluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Factory::~Factory (void)
{}

template <class R, class T1, class T2, class T3, class T4>
template <class T>
inline R *
IgPluginFactoryImplTypes<R * (T1, T2, T3, T4)>::AutoFactory<T>::create (T1 a1, T2 a2, T3 a3, T4 a4)
{ return new T (a1, a2, a3, a4); }

template <class R, class T1, class T2, class T3, class T4>
inline
IgPluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::Info (IgModule *module,
							    const std::string &name,
							    const std::string &tag)
    : IgPluginInfo (module, name, tag),
      m_factory (0)
{ finish (true); }

template <class R, class T1, class T2, class T3, class T4>
inline
IgPluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::Info (IgModule *module,
							    IgModuleDescriptor *details,
							    const std::string &tag)
    : IgPluginInfo (module, details->token (1), tag),
      m_factory (0)
{ ASSERT (details->tokens () == 2); finish (false); }

template <class R, class T1, class T2, class T3, class T4>
inline
IgPluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::~Info (void)
{ detach (); }

template <class R, class T1, class T2, class T3, class T4>
inline R *
IgPluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::create (T1 a1, T2 a2, T3 a3, T4 a4)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    return factory ()->create (a1, a2, a3, a4);
}

template <class R, class T1, class T2, class T3, class T4>
inline void
IgPluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::attach (Factory *factory)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    ASSERT (! m_factory);
    ASSERT (factory); 
    m_factory = factory;
}

template <class R, class T1, class T2, class T3, class T4>
inline void
IgPluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::detach (void)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    delete m_factory;
    m_factory = 0;
}

template <class R, class T1, class T2, class T3, class T4>
inline typename IgPluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Factory *
IgPluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::factory (void) const
{
    ASSERT (module ());
    ASSERT (! name ().empty ());

    if (! m_factory)
	module ()->attach ();

    if (! m_factory)
	noFactory ();

    return m_factory;
}

#endif // IGUANA_FRAMEWORK_IG_PLUGIN_DB_VIEW_H
