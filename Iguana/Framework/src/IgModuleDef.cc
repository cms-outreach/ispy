//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgModuleDef.h"
#include "Iguana/Framework/interface/IgModule.h"
#include "debug.h"
#include "classlib/utils/DebugAids.h"
#include <algorithm>
#include <functional>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgModuleDef::IgModuleDef (void)
    : m_module (0)
{}

IgModuleDef::~IgModuleDef (void)
{
    if (m_module)
	LOG (0, warning, LFplugin_manager, "Attached IgModuleDef destroyed\n");
}

void
IgModuleDef::attach (void)
{}

void
IgModuleDef::detach (void)
{}

void
IgModuleDef::query (void)
{}

void
IgModuleDef::bind (IgModule *module)
{
    ASSERT (! m_module);
    ASSERT (module);
    m_module = module;
}

void
IgModuleDef::release (void)
{
    ASSERT (m_module);
    m_module = 0;
}

IgModule *
IgModuleDef::module (void) const
{
    ASSERT (m_module);
    return m_module;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgSimpleModuleDef::query (void)
{
    std::for_each (onQueryMD.begin (), onQueryMD.end (),
		   std::bind2nd (std::mem_fun_ref (&MDAction::operator()), this));
    std::for_each (onQuery.begin (), onQuery.end (),
		   std::mem_fun_ref (&Action::operator()));
}

void
IgSimpleModuleDef::attach (void)
{
    std::for_each (onAttachMD.begin (), onAttachMD.end (),
		   std::bind2nd (std::mem_fun_ref (&MDAction::operator()), this));
    std::for_each (onAttach.begin (), onAttach.end (),
		   std::mem_fun_ref (&Action::operator()));
}

void
IgSimpleModuleDef::detach (void)
{
    std::for_each (onDetachMD.begin (), onDetachMD.end (),
		   std::bind2nd (std::mem_fun_ref (&MDAction::operator()), this));
    std::for_each (onDetach.begin (), onDetach.end (),
		   std::mem_fun_ref (&Action::operator()));
}
