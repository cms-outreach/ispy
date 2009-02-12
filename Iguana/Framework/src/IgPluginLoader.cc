//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgPluginLoader.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgCapabilityDB.h"
#include "classlib/utils/DebugAids.h"
#include <algorithm>
#include <cstring>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgPluginLoader, "Services/Global/Plugin Loader");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgPluginLoader::IgPluginLoader (IgState *in)
    : m_state (in)
{
    ASSERT (m_state);
    m_state->put (s_key, this);
}

void
IgPluginLoader::load (const std::string &name, bool prefix /* = false */)
{ load (m_state, name, prefix); }

void
IgPluginLoader::load (IgState *state, const std::string &name,
		      bool prefix /* = false */)

{
    IgExtensionDB *edb = IgExtensionDB::get ();
    IgCapabilityDB *cdb = IgCapabilityDB::get ();

    // If asked for an exact match, get the element or the capability
    // if it doesn't already exist in the state.  This assumes the
    // extensions' state key names match their plug-in catalog names.
    // For capabilities we don't care since the whole point is to just
    // attach the plug-in, and extra load attempts are simply ignored.
    if (! prefix && ! state->get (IgStateKey::index (name)))
    {
	edb->create (name, state);
	cdb->load (name);
    }

    // Otherwise, if we are looking for a prefix, load all extensions
    // and capabilities whose name begins with that prefix.  (FIXME:
    // Use regex match instead?)  (FIXME: std::string `compare'
    // methods are not very standard so avoid them.)
    else if (prefix)
    {
	const char	*pfx = name.c_str ();
	size_t		n = name.length ();

	IgExtensionDB::Iterator ext = edb->begin ();
	IgExtensionDB::Iterator lastext = edb->end ();
	for ( ; ext != lastext; ++ext)
	    if (! strncmp ((*ext)->name ().c_str (), pfx, n))
		load (state, (*ext)->name ());

	IgCapabilityDB::Iterator cap = cdb->begin ();
	IgCapabilityDB::Iterator lastcap = cdb->end();
	for ( ; cap != lastcap; ++cap)
	    if (! strncmp ((*cap)->name ().c_str (), pfx, n))
		load (state, (*cap)->name ());
    }
}
