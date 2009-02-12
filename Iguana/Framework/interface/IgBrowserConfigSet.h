#ifndef IGUANA_FRAMEWORK_IG_BROWSER_CONFIG_SET_H
# define IGUANA_FRAMEWORK_IG_BROWSER_CONFIG_SET_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include <string>
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgBrowserConfigSet;

class IgBrowserPropertySetManager;
class IgBrowserPropertyNameSet;
class IgBrowserPropertyName;
class IgBrowserPropertyValue;

class IgBrowserConfigItem;
class IgBrowserConfigItemFactory;

class IgBrowserConfigBus;
class IgBrowserConfigSiteSet;
class IgBrowserConfigSite;
class IgBrowserConfigBrowserSet;
class IgBrowserConfigBrowser;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Incomplete, work in progress.  */
class IGUANA_FRAMEWORK_API IgBrowserPropertySetManager
{
public:
    virtual ~IgBrowserPropertySetManager (void);
    // default constructor
    // default copy constructor
    // default assignment operator

    virtual IgBrowserPropertyNameSet	property_names (void) const = 0;
    virtual std::string			get_property (const IgBrowserPropertyName &key) = 0;
    virtual void			set_property (const IgBrowserPropertyName &key,
						      const IgBrowserPropertyValue &value) = 0;
    virtual void			clear_property (const IgBrowserPropertyName &key) = 0;
    virtual void			clear_properties (void) = 0;
    virtual void			reset_properties (void) = 0;
};

/** Incomplete, work in progress.  */
class IGUANA_FRAMEWORK_API IgBrowserConfigSet
{
    typedef std::vector<IgBrowserConfigItem *>	item_set;
    typedef item_set::const_iterator		item_iterator;
    typedef item_set::size_type			size_type;

public:
    IgBrowserConfigSet (void);
    ~IgBrowserConfigSet (void);

    // file interface
    static void			associate_factory (const std::string &element_type,
						   IgBrowserConfigItemFactory *factory);
    static IgBrowserConfigSet *	instantiate (const Filename &file);
    void			read (const Filename &file);
    void			write (const Filename &file);

    // attributes
    void			name (const std::string &value);
    std::string			name (void) const;

    // contents
    item_iterator		begin_items (void) const;
    item_iterator		end_items (void) const;
    size_type			items (void) const;
    void			add_item (IgBrowserConfigItem *item);
    void			remove_item (IgBrowserConfigItem *item);
    IgBrowserConfigItem *	lookup_item (const std::string &name);

private:
    std::string			m_name;
    item_set			m_items;
};

/** Incomplete, work in progress.  */
class IGUANA_FRAMEWORK_API IgBrowserConfigItem : public IgBrowserPropertySetManager
{
public:
    // properties (by base class)
    // attributes
    virtual std::string		name (void) const = 0;

    // contents
    virtual IgBrowserConfigItem *lookup_item (const std::string &name) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_BROWSER_CONFIG_SET_H
