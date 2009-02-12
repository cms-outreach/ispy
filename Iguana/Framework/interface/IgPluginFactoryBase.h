#ifndef IGUANA_FRAMEWORK_IG_PLUGIN_DBVIEW_BASE_H
# define IGUANA_FRAMEWORK_IG_PLUGIN_DBVIEW_BASE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgModule;
class IgPluginInfo;
class IgModuleDescriptor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Abstract base class to bridge #IgManager and #IgPluginFactory.  */
class IgPluginFactoryBase
{
public:
    IgPluginFactoryBase (const std::string &tag);
    virtual ~IgPluginFactoryBase (void);

    // Category tag
    const std::string &	category (void) const;

    // Notifications from IgManager.
    virtual void	restore (IgModule *module, IgModuleDescriptor *info) =0;
    virtual void	addInfo (IgPluginInfo *info) = 0;
    virtual void	removeInfo (IgPluginInfo *info) = 0;

protected:
    // Notifications from derived class
    virtual void	rebuild (void);

private:
    std::string	m_tag;

    // undefined, no semantics
    IgPluginFactoryBase (const IgPluginFactoryBase &);
    IgPluginFactoryBase &operator= (const IgPluginFactoryBase &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_PLUGIN_DBVIEW_BASE_H
