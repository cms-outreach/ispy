//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgStateKey.h"
#include "classlib/utils/DebugAids.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

unsigned IgStateKey::s_count = 0;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Create and register a new state key with name @a label.  */
IgStateKeyDef::IgStateKeyDef (const std::string &label)
    : IgStateKey (create (label)),
      m_label (label)
{}

/** Unregister the state key.  */
IgStateKeyDef::~IgStateKeyDef (void)
{ remove (m_label); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Internal constructor to create a key for a known value.  */
IgStateKey::IgStateKey (unsigned value)
    : m_value (value)
{}

/** Return the name associated with this state key.  */
std::string
IgStateKey::label (void) const
{
    Map &keys = keymap ();
    for (Map::iterator pos = keys.begin (); pos != keys.end (); ++pos)
	if (pos->second == m_value)
	    return pos->first;

    // This can occur for the unknown key (~0U, see #index())
    return "<UNDEFINED>";
}

//////////////////////////////////////////////////////////////////////
/** Get the key map.  This is statically allocated and guaranteed to
    outlast most destructors.  */
IgStateKey::Map &
IgStateKey::keymap (void)
{
    static char	buf [sizeof (Map)];
    static Map	*bufmap = new (buf) Map;
    return *bufmap;
}

/** Register a new state key and allocate an index for it. */
unsigned
IgStateKey::create (const std::string &label)
{
    // FIXME: Thread safety.

    // The logic here accommodates for keys that are getting recreated
    // due to dynamic loading and unloading of a library.  Once a key
    // has been created, we always keep it in the same index.  If the
    // library is unloaded, we keep the key in the dictionary but mark
    // it unused by turning on the 32nd (~ top) bit.  If the library
    // is subsequently re-loaded, we give it the same index back and
    // turn the bit off to indicate the index is in use again.
    ASSERT (! label.empty ());
    Map &keys = keymap ();
    if (! keys.count (label))
    {
	// Not seen this one before.  Allocate a new index for this.
	// Make sure we don't overrun to the area of our mark bits (if
	// this triggers, we'll be screwed anyway as we have far too
	// many state elements by then in every state).
	ASSERT (s_count < 0x8000000);
	unsigned value = s_count++;
	keys [label] = value;
	return value;
    }
    else
    {
	// We've seen this before.  Make sure the key index is not
	// active and then give it back.
	ASSERT (keys [label] & 0x8000000);
	unsigned &value = keys [label];
	value &= ~0x8000000;
	return value;
    }
}

/** Unregister a state key. */
void
IgStateKey::remove (const std::string &label)
{
    // See comments in #create() for the logic.  FIXME: Thread safety.
    ASSERT (! label.empty ());
    Map &keys = keymap ();
    ASSERT (keys.count (label));
    ASSERT (! (keys [label] & 0x8000000));
    keys [label] |= 0x8000000;
}

/** Find what index key the label corresponds.  Returns the index or
    ~0U (which is safe to use with #IgState::get() as it will always
    return null for that index.) */
IgStateKey
IgStateKey::index (const std::string &label)
{
    // FIXME: Thread safety.
    ASSERT (! label.empty ());
    Map		  &keys = keymap ();
    Map::iterator pos = keys.find (label);
    return pos == keys.end () ? ~0U : pos->second;
}
