//<<<<<< INCLUDES                                                       >>>>>>

#include "classlib/utils/XTypeInfo.h"
#include "classlib/utils/Log.h"
#include <algorithm>
#include <utility>
#if HAVE_NEW
# include <new>
#elif HAVE_NEW_H
# include <new.h>
#endif

namespace lat {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

logflag LFxtype = { 0, "xtypeinfo", true, -1 };

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

XTypeInfo::Monitor::~Monitor (void)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

XTypeInfo::TypeTable &
XTypeInfo::types (void)
{
    // FIXME: work around for compilers that destruct function
    //        local statics too early; may have alignment issues...
    static char		data [sizeof(TypeTable)];
    static TypeTable	*table = new (&data[0]) TypeTable;
    return *table;
}

XTypeInfo::ExtensionKey &
XTypeInfo::extensions (void)
{
    static ExtensionKey key = 0;
    return key;
}

XTypeInfo::HookList &
XTypeInfo::hooks (void)
{
    // FIXME: work around for compilers that destruct function
    //        local statics too early; may have alignment issues...
    static char		data [sizeof(HookList)];
    static HookList	*list = new (&data[0]) HookList;
    return *list;
}

void
XTypeInfo::countBases (ClassDef *def, int &count)
{
    for (BaseDef *base = def->m_basesBegin; base != def->m_basesEnd; ++base)
	if (base->m_distance == 1)
	{
	    ++count;
	    countBases (base->m_basedef, count);
	}
}

void
XTypeInfo::collectBases (ClassDef *def, BaseDef *first, BaseDef *&next,
			   int distance)
{
    for (BaseDef *base = def->m_basesBegin; base != def->m_basesEnd; ++base)
    {
	if (base->m_distance != 1)
	    continue;

	BaseDef *previous = first;
	while (previous != next && previous->m_basedef != base->m_basedef)
	    ++previous;

	bool nearer = (previous != next
		       && previous->m_distance > base->m_distance+distance);

	if (previous == next || nearer)
	{
	    if (nearer)
	    {
		std::copy (previous+1, next, previous);
		--next;
	    }
	    *next = *base;
	    next->m_distance = distance;
	    ++next;
	}

	collectBases (base->m_basedef, first, next, distance+1);
    }
}

void
XTypeInfo::registerType (XTypeInfo *type)
{
    ASSERT (type);
    ASSERT (type->m_def);
    ASSERT (! type->m_activeDef);
    ASSERT (! type->m_nextEqual);

    ClassDef *&def = types () [type->m_std];
    LOG (0, trace, LFxtype,
	 "registering type `" << type->m_std.name () << "'"
	 << (def ? " (duplicate definition)\n" : "\n"));

    if (def)
    {
	ASSERT (def->m_types);
	type->m_activeDef = def;
	type->m_nextEqual = def->m_types;
	def->m_types = type;
	return;
    }
    else
    {
	def = type->m_activeDef = type->m_def;
	def->m_types = type;

	// Build complete base class list
	ASSERT (def->m_basesBegin);
	ASSERT (def->m_basesEnd);
	ASSERT (def->m_basesEnd >= def->m_basesEnd);

	// Determine how many base classes we need at most.  There
	// might be fewer bases than what we compute if some of them
	// are virtual, but since the number is likely to be quite
	// small anyhow, we don't care to make the effort to get a
	// more exact value.

	int baseCount = 0;
	countBases (def, baseCount);

	BaseDef *begin = baseCount ? new BaseDef [baseCount] : 0;
	BaseDef *end = begin;
	collectBases (def, begin, end, 1);

	def->m_basesBegin = begin;
	def->m_basesEnd = end;

	// Maintain inheritance links from base classes to derived ones.
	for (BaseDef *base = begin; base != end; ++base)
	{
	    if (base->m_distance != 1) continue;

	    ClassDef *baseclass = base->m_basedef;
	    if (baseclass->m_derivedEnd == baseclass->m_derivedLimit)
	    {
	        // reallocate some more storage; the growth rate is aggressive
		// to reallocate as rarely as possible, but still maintaining
		// reasonably low overhead (most classes have fewer than four
		// base classes anyway).
	        STDC::size_t oldSize = baseclass->m_derivedEnd - baseclass->m_derivedBegin;
	        STDC::size_t newSize = ((oldSize + 3) & ~STDC::size_t(3)) * 2;
		if (newSize == 0) newSize = 4;
		
	        ClassDef **derived = new ClassDef * [newSize];

		std::copy (baseclass->m_derivedBegin, baseclass->m_derivedEnd, derived);
		delete [] baseclass->m_derivedBegin;
		baseclass->m_derivedBegin = derived;
		baseclass->m_derivedEnd   = derived + oldSize;
		baseclass->m_derivedLimit = derived + newSize;
	    }
	    *baseclass->m_derivedEnd++ = def;
	}
	
	HookList::iterator first = hooks ().begin ();
	HookList::iterator last = hooks ().end ();
	for ( ; first != last; ++first)
	    (*first)->typeHook (type);
    }
}
    
void
XTypeInfo::deregisterType (XTypeInfo *type)
{
    ASSERT (type);
    ASSERT (type->m_activeDef);
    ASSERT (type->m_activeDef->m_types);

    ClassDef	*def = type->m_activeDef;
    XTypeInfo	*previous = 0;
    XTypeInfo	*current = def->m_types;
    XTypeInfo	*next = type->m_nextEqual;

    // notify hooks if this is the last `XTypeInfo' instance for
    // the `ClassDef' corresponding to the `XTypeInfo'.
    if (current == type && ! next)
    {
	HookList::iterator first = hooks ().begin ();
	HookList::iterator last = hooks ().end ();
	for ( ; first != last; ++first)
	    (*first)->typeUnhook (type);

	Extension *ext = def->m_extensionsBegin;
	while (ext != def->m_extensionsEnd)
	{
	    ASSERT (! *ext);
	    ++ext;
	}
    }

    // disconnect ClassDef from the type information
    while (current && current != type)
    {
	previous = current;
	current = current->m_nextEqual;
    }

    ASSERT (current == type);
    if (previous) previous->m_nextEqual = next;
    else          def->m_types = next;

    type->m_nextEqual = 0;
    type->m_activeDef = 0;

    if (! next)
    {
	// Remove inheritance links from base classes to derived ones.
	for (BaseDef *base = def->m_basesBegin; base != def->m_basesEnd; ++base)
	{
	    if (base->m_distance != 1) continue;

	    ClassDef *baseclass = base->m_basedef;
	    ClassDef **pos = std::find (baseclass->m_derivedBegin,
					baseclass->m_derivedEnd,
					type->m_def); // FIXME: ->m_def?
	    ASSERT (pos != baseclass->m_derivedEnd);
	    std::copy (pos+1, baseclass->m_derivedEnd, pos);
	    --baseclass->m_derivedEnd;
	}

	TypeTable::iterator place = types ().find (type->m_std);
	ASSERT (place != types ().end ());
	LOG (1, trace, LFxtype,
	     "deregistered type `" << type->m_std.name () << "'\n");
	types ().erase (place);
    }
}

// FIXME: This needs to be outside functions due to mutually incompatible
// bugs in GCC 3.1 and Solaris CC 5.3.  GCC can't grok the first form in
// a function-local scope but does accept a "using namespace std::rel_ops;".
// However, Solaris CC doesn't find the operator with the latter.
# if HAVE_NAMESPACE_STD_REL_OPS
    using std::rel_ops::operator!=;
# elif HAVE_REL_OPS_IN_STD
    using std::operator!=;
# endif

void
XTypeInfo::addMonitor (Monitor *who, bool notify)
{
# if HAVE_NAMESPACE_STD_REL_OPS
    // FIXME (see above): using namespace std::rel_ops;
# elif HAVE_REL_OPS_IN_STD
    // FIXME (see above): using std::operator!=;
# endif

    ASSERT (who);
    hooks ().push_back (who);

    if (notify)
    {
	TypeIterator type = beginTypes ();
	TypeIterator last = endTypes ();
	for ( ; type != last; ++type) 
	{
	    ASSERT (*type);
	    who->typeHook (*type);
	}
    }
}

void
XTypeInfo::removeMonitor (Monitor *who, bool notify)
{
# if HAVE_NAMESPACE_STD_REL_OPS
    // FIXME (see above): using namespace std::rel_ops;
# elif HAVE_REL_OPS_IN_STD
    // FIXME (see above): using std::operator!=;
# endif

    ASSERT (who);
    if (notify)
    {
	TypeIterator type = beginTypes ();
	TypeIterator last = endTypes ();
	for ( ; type != last; ++type) 
	{
	    ASSERT (*type);
	    who->typeUnhook (*type);
	}
    }

    HookList::iterator pos = std::find (hooks ().begin (), hooks ().end (),
					who);
    ASSERT (pos != hooks ().end ());
    hooks ().erase (pos);
}

XTypeInfo *
XTypeInfo::find (const std::type_info &std_info)
{
    TypeTable::iterator place = types ().find (std_info);
    if (place == types ().end ())
    {
	LOG (0, warning, LFxtype, "inventing a XTypeInfo for an unknown type `"
	     << std_info.name () << "' (in XTypeInfo::find())\n");

	static BaseDef	empty = { 0, 0, 0, 0, 0 };
	ClassDef	*def = new ClassDef;

	def->m_types = 0;
	def->m_basesBegin = 0;
	def->m_basesEnd = 0;
	def->m_derivedBegin = 0;
	def->m_derivedEnd = 0;
	def->m_derivedLimit = 0;
	def->m_extensionsBegin = 0;
	def->m_extensionsEnd = 0;

	return new XTypeInfo (std_info, def, &empty);
    }
    else
    	return (*place).second->m_types; // FIXME: for Sun CC 4.2
}

void *
XTypeInfo::cast (const XTypeInfo * /*to*/,
		  const XTypeInfo * /*from*/,
		  void * /*object*/)
{
    // FIXME
    abort ();

//     ASSERT (to);
//     ASSERT (from);
//     ASSERT (false);
    return 0; // return object; // unreachable
}

XTypeInfo::ExtensionKey
XTypeInfo::allocateExtension (void)
{ return extensions()++; }

XTypeInfo::XTypeInfo (const std::type_info &std_info,
			ClassDef *def, BaseDef *bases)
    : m_std (std_info),
      m_def (def),
      m_activeDef (0),
      m_nextEqual (0)
{
    ASSERT (def);
    ASSERT (! def->m_types);
    
    def->m_basesBegin = bases;
    def->m_basesEnd   = bases;
    while (def->m_basesEnd && def->m_basesEnd->m_basedef)
	++def->m_basesEnd;

    if (extensions ()) {
	def->m_extensionsBegin = new Extension [extensions ()];
	def->m_extensionsEnd   = def->m_extensionsBegin + extensions ();
       
	std::fill (def->m_extensionsBegin, def->m_extensionsEnd,
		   Extension (0));
    }

    registerType (this);
}

XTypeInfo::~XTypeInfo (void)
{
    deregisterType (this);
}

const std::type_info &
XTypeInfo::standard (void) const
{ return m_std; }

XTypeInfo::Extension
XTypeInfo::getExtension (ExtensionKey which) const
{
    ASSERT (which < extensions ());
    
    // new extensions may have been allocated since this type information
    // object was created, so be prepared to create a new slot
    if (m_def->m_extensionsBegin == 0
        || size_t (m_def->m_extensionsEnd-m_def->m_extensionsBegin) <= which)
    {
	STDC::size_t oldSize = m_def->m_extensionsEnd - m_def->m_extensionsBegin;
	STDC::size_t newSize = ((which + 1 + 3) & ~size_t(3));
	    
	Extension *extensions = new Extension [newSize];
		
	std::copy (m_def->m_extensionsBegin, m_def->m_extensionsEnd, extensions);
	std::fill (extensions + oldSize, extensions + newSize, Extension (0));

	delete [] m_def->m_extensionsBegin;
	m_def->m_extensionsBegin = extensions;
	m_def->m_extensionsEnd   = extensions + newSize;
    }
    
    return m_def->m_extensionsBegin [which];
}

XTypeInfo::Extension
XTypeInfo::setExtension (ExtensionKey which, Extension value)
{
    Extension old = getExtension (which);
    m_def->m_extensionsBegin [which] = value;
    return old;
}

XTypeInfo::BaseIterator
XTypeInfo::beginBases (void) const
{ return BaseIterator (m_def->m_basesBegin); }

XTypeInfo::BaseIterator
XTypeInfo::endBases (void) const
{ return BaseIterator (m_def->m_basesEnd); }

XTypeInfo::BaseIterator
XTypeInfo::base (const XTypeInfo *type) const
{
    BaseDef *base = m_def->m_basesBegin;
    while (base != m_def->m_basesEnd && base->m_basedef != type->m_def) 
	++base;

    return BaseIterator (base);
}

bool
XTypeInfo::hasBase (const XTypeInfo *type) const
{
# if HAVE_NAMESPACE_STD_REL_OPS
    // FIXME (see above): using namespace std::rel_ops;
# elif HAVE_REL_OPS_IN_STD
    // FIXME (see above): using std::operator!=;
# endif
    return base (type) != endBases ();
}

XTypeInfo::DerivedIterator
XTypeInfo::beginDerived (void) const
{ return DerivedIterator (m_def->m_derivedBegin); }

XTypeInfo::DerivedIterator
XTypeInfo::endDerived (void) const
{ return DerivedIterator (m_def->m_derivedEnd); }

XTypeInfo::DerivedIterator
XTypeInfo::derived (const XTypeInfo *type) const
{
    ClassDef **derived = m_def->m_derivedBegin;
    while (derived != m_def->m_derivedEnd && *derived != type->m_def) 
	++derived;

    return DerivedIterator (derived);
}

bool
XTypeInfo::hasDerived (const XTypeInfo *type) const
{
# if HAVE_NAMESPACE_STD_REL_OPS
    // FIXME (see above): using namespace std::rel_ops;
# elif HAVE_REL_OPS_IN_STD
    // FIXME (see above): using std::operator!=;
# endif

    return derived (type) != endDerived ();
}

} // namespace lat
