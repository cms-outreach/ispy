#ifndef IGUANA_FRAMEWORK_IG_MODULE_DEF_H
# define IGUANA_FRAMEWORK_IG_MODULE_DEF_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgPluginDef.h"
# include "classlib/utils/Callback.h"
# include <string>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

# if 0 // For doxygen
/** Determines whether plug-ins are in libraries or in the program.

    This macro controls the behaviour of #DEFINE_IGUANA_MODULE and
    #DEFINE_IGUANA_PLUGIN macros.  If set, the macros produce static
    initialisers to register to #IgModule::builtin().  If unset, the
    macros generate a shared library entry point through which the
    module definition can be accessed by #IgModule.  */
#  define IGUANA_BUILTIN_PLUGINS 0
# endif // 0 for doxygen

/** @def DEFINE_IGUANA_MODULE()
  @brief Define module entry point to register this #IgModule.
  @sa    #IGUANA_BUILTIN_PLUGINS.  */

/** @def DEFINE_IGUANA_PLUGIN(factory,type,name)
  @brief Register @a type into @a factory as @a name.
  @sa    #IGUANA_BUILTIN_PLUGINS.  */

/** @def DEFINE_IG_PLUGIN_DEFAULT_NAME(factory,type)
  @brief Register @a type into @a factory as stringized @a type name.
  @sa    #IGUANA_BUILTIN_PLUGINS.  */

/** @def DEFINE_IG_PLUGIN_NAME(factory,name)
  @brief Register @a name into @a factory without any object type.
  @sa    #IGUANA_BUILTIN_PLUGINS.  */

/** @def IG_PLUGIN_API
  @brief Windows DLL API used for module entry point.
         Automatically determined from #IGUANA_BUILTIN_PLUGINS.  */


# define DEFINE_IGUANA_PLUGIN(factory,type,name)		\
     IG_MODULE_QUERY_MDACTION					\
	((&IgPluginDef<factory, type>::declareName, std::string (name)));\
     IG_MODULE_ATTACH_MDACTION					\
	((&IgPluginDef<factory, type>::installFactoryName,std::string (name)))

# define DEFINE_IG_PLUGIN_DEFAULT_NAME(factory,type)		\
     IG_MODULE_QUERY_MDACTION					\
	((&IgPluginDef<factory, type>::declareName, std::string (#type)));\
     IG_MODULE_ATTACH_MDACTION					\
	((&IgPluginDef<factory, type>::installFactoryName,std::string (#type)))

# define DEFINE_IG_PLUGIN_NAME(factory,name)			\
     IG_MODULE_QUERY_MDACTION					\
	((&IgPluginDef<factory, void>::declareName, std::string (name)))

# define IG_MODULE_SYM(x,y)	IG_MODULE_SYM2(x,y)
# define IG_MODULE_SYM2(x,y)	x ## y

# ifndef IGUANA_BUILTIN_PLUGINS
#  ifdef _WIN32
#   define IG_PLUGIN_API __declspec(dllexport)
#  else
#   define IG_PLUGIN_API
#  endif

# if __GNUC__
#  define IG_MODULE_USED __attribute__ ((unused))
# else
#  define IG_MODULE_USED
# endif

#  define DEFINE_IGUANA_MODULE()					\
     namespace { IgSimpleModuleDef IGUANA_THIS_MODULE; }		\
     extern "C" IG_PLUGIN_API IgModuleDef *IGUANA_MODULE (void)		\
	{ return &IGUANA_THIS_MODULE; }					\
     extern "C" IG_PLUGIN_API IgModuleDef *IGUANA_MODULE (void)

#  define IG_MODULE_QUERY_ACTION(cbargs)				\
     static bool IG_MODULE_USED IG_MODULE_SYM (moduleInitQueryAction, __LINE__) = \
	(IGUANA_THIS_MODULE.onQuery.push_back(lat::CreateCallback cbargs), true)
#  define IG_MODULE_QUERY_MDACTION(cbargs)				\
     static bool IG_MODULE_USED IG_MODULE_SYM (moduleInitQueryAction, __LINE__) = \
	(IGUANA_THIS_MODULE.onQueryMD.push_back(lat::CreateCallback cbargs), true)

#  define IG_MODULE_ATTACH_ACTION(cbargs)				\
     static bool IG_MODULE_USED IG_MODULE_SYM (moduleInitAttachAction, __LINE__) = \
	(IGUANA_THIS_MODULE.onAttach.push_back(lat::CreateCallback cbargs), true)
#  define IG_MODULE_ATTACH_MDACTION(cbargs)				\
     static bool IG_MODULE_USED IG_MODULE_SYM (moduleInitAttachAction, __LINE__) = \
	(IGUANA_THIS_MODULE.onAttachMD.push_back(lat::CreateCallback cbargs), true)

#  define IG_MODULE_DETACH_ACTION(cbargs)				\
     static bool IG_MODULE_USED IG_MODULE_SYM (moduleInitDetachAction, __LINE__) = \
	(IGUANA_THIS_MODULE.onDetach.push_back(lat::CreateCallback cbargs), true)
#  define IG_MODULE_DETACH_MDACTION(cbargs)				\
     static bool IG_MODULE_USED IG_MODULE_SYM (moduleInitDetachAction, __LINE__) = \
	(IGUANA_THIS_MODULE.onDetachMD.push_back(lat::CreateCallback cbargs), true)

# else

#  define DEFINE_IGUANA_MODULE()					\
     namespace { IgSimpleModuleDef IGUANA_THIS_MODULE; }		\
     static bool IG_MODULE_SYM (moduleInitAction, __LINE__) =		\
	(IGUANA_THIS_MODULE.bind (IgModule::builtin ()), true)

#  define IG_MODULE_QUERY_ACTION(cbargs)				\
     static bool moduleInitQueryAction ## __LINE__ =			\
	(lat::CreateCallback cbargs->call (), true)
#  define IG_MODULE_QUERY_MDACTION(cbargs)				\
     static bool IG_MODULE_SYM (moduleInitQueryAction, __LINE__) =	\
	(lat::CreateCallback cbargs->call (&IGUANA_THIS_MODULE), true)

#  define IG_MODULE_ATTACH_ACTION(cbargs)				\
     static bool IG_MODULE_SYM (moduleInitAttachAction, __LINE__) =	\
	(lat::CreateCallback cbargs->call (), true)
#  define IG_MODULE_ATTACH_MDACTION(cbargs)				\
     static bool IG_MODULE_SYM (moduleInitDetachAction, __LINE__) =	\
	(lat::CreateCallback cbargs->call (&IGUANA_THIS_MODULE), true)

#  define IG_MODULE_DETACH_ACTION(cbargs)   // FIXME: Execute on destruction
#  define IG_MODULE_DETACH_MDACTION(cbargs) // FIXME: Execute on destruction
# endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgModule;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A base class for modules.

    This class is a bridge between #IgModule and a dynamically loaded
    shared library.  A #IgModule exists for a shared library even when
    it is not resident in memory.  A #IgModuleDef exists for each module
    that is loaded in memory, usually just the simple proxy generated
    with #DEFINE_IGUANA_MODULE().

    The purpose of this class is to allow a module to appear as an
    object.  The class is not meant to be generally visible however,
    #DEFINE_IGUANA_MODULE() and #DEFINE_SEAL_PLUGIN() macros should be
    sufficient to most users.

    A module has three entry points each of which is a virtual method
    in this class: #attach(), #detach() and #query().  The methods are
    invoked by respective methods in the #IgModule, thus causing them to
    be executed in the context of the loaded shared library.

    Subclasses should override at least #query() to describe the
    properties of the module.  The implementation should invoke @c
    declare() in the appropriate #IgPluginFactory subclasses to register
    the capabilities of the module.  #DEFINE_IGUANA_PLUGIN() arranges
    such calls to take place.

    The subclass can also override the #attach() and #detach() methods
    to execute actions.  Plug-ins registered in #query() must be given
    factories in #attach() by invoking the @c installFactory() method
    in the appropriate #IgPluginFactory.  #DEFINE_IGUANA_PLUGIN() arranges
    that to happen.  Additional actions can be arranged to execute at
    #attach() time with #IG_MODULE_ATTACH_ACTION(), for example to
    initialize external third-party libraries.  There is no normally
    no reas to override #detach(); it's purpose is to undo #attach(),
    but as factories don't need to be removed, there is often little
    to do.  Use #IG_MODULE_DETACH_ACTION() to register actions to be
    executed in #detach().

    Subclasses should not define a constructor or a destructor, or
    should at the very least make them very simple.  */
class IgModuleDef
{
public:
    IgModuleDef (void);
    virtual ~IgModuleDef (void);

    // Module implementation interface
    virtual void	query (void);
    virtual void	attach (void);
    virtual void	detach (void);

public:
    // Plug-in architecture interface: for internal use only!  (Public
    // because DEFINE_IGUANA_MODULE currently needs access to these.)
    void		bind (IgModule *module);
    void		release (void);

    // For use by factories.
    IgModule *		module (void) const;

private:
    IgModule		*m_module; //< The module owning me.

    // undefined semantics
    IgModuleDef (const IgModuleDef &);
    IgModuleDef &operator= (const IgModuleDef &);
};

/** #IgModuleDef used by #DEFINE_IGUANA_MODULE().  */
class IgSimpleModuleDef : public IgModuleDef
{
public:
    typedef lat::Callback1<IgModuleDef *>	MDAction;
    typedef std::list<MDAction>			MDActionList;
    typedef lat::Callback			Action;
    typedef std::list<Action>			ActionList;

    virtual void	query (void);
    virtual void	attach (void);
    virtual void	detach (void);

    MDActionList	onQueryMD;
    MDActionList	onAttachMD;
    MDActionList	onDetachMD;
    ActionList		onQuery;
    ActionList		onAttach;
    ActionList		onDetach;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_MODULE_DEF_H
