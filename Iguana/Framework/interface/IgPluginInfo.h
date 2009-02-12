#ifndef IGUANA_FRAMEWORK_IG_PLUGIN_DB_ITEM_H
# define IGUANA_FRAMEWORK_IG_PLUGIN_DB_ITEM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgModule;
class IgModuleDescriptor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Abstract base class for information on features provided by a module.

    The derived class should provide:
      - A typedef @c Object for the type of things created through
        this factory category.  Normally this would be some abstract
	base class, but it can be anything you want.

      - An abstract base class @c Factory with method @c create().
        The @c create() parameters define the constructor arguments
        passed to the instantiated objects.  The list can be anything
        you want.  The return value of the @c create() method should
        be a "Object *".  There can be multiple @c create() methods if
        multiple constructor argument lists are neded.  If you don't
	want to create any objects at all, i.e. the category describes
	a capability or some other side-effect, typedef @c Factory to
	@c void.

      - A templated class @c AutoFactory which inherits @c Factory.
        An instance of this templated class will be created for the
	actual plug-in types; the template argument @c T will be the
	actual type, which must inherit from @c Object.   Override
	the @c create() method to do "return new T (args)".

      - FIXME */
class IgPluginInfo
{
public:
    // FIXME: Derived class must provide...
    IgPluginInfo (IgModule *module,
		  const std::string &name,
		  const std::string &category);
    virtual ~IgPluginInfo (void);

    IgModule *		module (void) const;
    const std::string &	name (void) const;
    const std::string &	category (void) const;

    virtual void	cache (IgModuleDescriptor *to) const;
    virtual void	detach (void);

protected:
    void		finish (bool needCache);
    void		noFactory (void) const;

private:
    IgModule		*m_module;	//< Provider module.
    std::string		m_name;		//< Object name.
    const std::string	&m_category;	//< Cache category label.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_PLUGIN_DB_ITEM_H
