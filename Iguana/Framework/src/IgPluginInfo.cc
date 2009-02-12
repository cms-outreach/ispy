//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgPluginInfo.h"
#include "Iguana/Framework/interface/IgModuleDescriptor.h"
#include "Iguana/Framework/interface/IgPluginManager.h"
#include "Iguana/Framework/interface/IgModule.h"
#include "debug.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Log.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgPluginInfo::IgPluginInfo (IgModule *module,
			    const std::string &name,
			    const std::string &category)
    : m_module (module),
      m_name (name),
      m_category (category)
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
    LOG (0, trace, LFplugin_manager, "defining " << m_category
	 << ' ' << m_name << '\n');
}

IgPluginInfo::~IgPluginInfo (void)
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
    m_module->detachInfo (this);
}

void
IgPluginInfo::finish (bool needCache)
{
    // Derived constructor is complete, tell the module I exist.  This
    // adds myself to the module and propagates the notification to
    // the relevant factory.  Note that the factory must already exist, so
    // it will get notified immediately.  However it might still be in
    // the middle of construction, which is why we propagate the
    // notification through the base classes and not directly to the
    // factory from the derived class.
    m_module->addInfo (this, needCache);
}

IgModule *
IgPluginInfo::module (void) const
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
    return m_module;
}

const std::string &
IgPluginInfo::name (void) const
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
    return m_name;
}

const std::string &
IgPluginInfo::category (void) const
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
    return m_category;
}

void
IgPluginInfo::cache (IgModuleDescriptor *to) const
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
    ASSERT (to);
    new IgModuleDescriptor (to, m_category, m_name);
}

void
IgPluginInfo::detach (void)
{
    ASSERT (m_module);
    ASSERT (! m_name.empty ());
    ASSERT (! m_category.empty ());
}

void
IgPluginInfo::noFactory (void) const
{
    m_module->manager ()->feedback
	(IgPluginManager::ErrorNoFactory,
	 m_name + " (" + m_module->libraryName ().name () + ")");
    ASSERT (false);
}
