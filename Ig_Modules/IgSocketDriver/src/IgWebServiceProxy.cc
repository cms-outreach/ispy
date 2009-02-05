//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSocketDriver/interface/IgWebServiceProxy.h"
#include "Ig_Modules/IgSocketDriver/interface/IgWebService.h"
#include "Ig_Modules/IgSocketDriver/interface/IgWebServiceDB.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgWebServiceProxy::IgWebServiceProxy (IgState *state, 
				      PluginInfo pluginInfo)
    : m_state (state),
      m_pluginInfo (pluginInfo),
      m_webService (0)
{
}

IgWebService *
IgWebServiceProxy::webService (void)
{
    if (! m_webService)
    {
	m_webService = m_pluginInfo->create (m_state); 
    }    

    ASSERT (m_webService);    
    return m_webService;    
}
