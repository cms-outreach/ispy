#ifndef CLASSLIB_AUTO_LOAD_H
# define CLASSLIB_AUTO_LOAD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/SharedLibrary.h"
# include "classlib/utils/Error.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A library used to resolve #AutoLoad entries.  */
class AutoLoadLib
{
public:
    AutoLoadLib (const char *libname);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    bool			exists (void);
    SharedLibrary		*library (void);
    SharedLibrary::Function	function (const char *symname);
    SharedLibrary::Data		data (const char *symname);

private:
    const char			*m_libname;
    SharedLibrary		*m_library;
};

/** A run-time error in using an #AutoLoad.  */
class AutoLoadError : public Error
{
public:
    AutoLoadError (const char *lib, const char *sym, Error *next = 0);

    virtual std::string		explainSelf (void) const;
    virtual Error *		clone (void) const;
    virtual void		rethrow (void);

private:
    const char			*m_libname;
    const char			*m_symname;
};

/** Function or variable loaded on first dereference at run-time.

    #AutoLoad defines a function or data smart pointer for entries
    that should be looked up at run-time by dynamically loading a
    library and resolving a symbol in it.

    Typically one defines a smart pointer object with the appropriate
    type matching the entity, data or function.  The entity is used
    by dereferencing this smart pointer.

    Use this interface judiciously, hiding dependencies behind an
    auto-load facility is generally frowned upon.  */
template <class Type>
class AutoLoad
{
public:
    AutoLoad (AutoLoadLib &library, const char *symname);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    bool		exists (void);
    Type &		operator* (void);
    Type *		operator-> (void);

private:
    void		loadme (void);

    AutoLoadLib	&	m_library;
    const char		*m_symname;
    Type		*m_symbol;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

// Helper functions to look up a symbol in an auto-load library.  This
// works by function overloading: if "Type" is a data entity, and thus
// "Type *" is automatically convertible to "void *", one function is
// selected, otherwise "Type" must be a function type and we go for the
// other alternative.
template <class Type>
inline Type *
AutoLoadLookup (AutoLoadLib &lib, const char *symbol, void *)
{ return (Type *) lib.data (symbol); }

template <class Type>
inline Type *
AutoLoadLookup (AutoLoadLib &lib, const char *symbol, ...)
{ return (Type *) lib.function (symbol); }

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Define an auto-loaded function or variable names @a symname, to
    be looked up in @a library on first dereference.  */
template <class Type>
inline
AutoLoad<Type>::AutoLoad (AutoLoadLib &library, const char *funcname)
    : m_library (library),
      m_symname (funcname),
      m_symbol (0)
{}

/** Load the implementation if we haven't done so yet.  Throws an
    #AutoLoadError if the library fails to load or there is a symbol
    lookup failure (non-existence of the symbol is not a failure).  */
template <class Type>
inline void
AutoLoad<Type>::loadme (void)
{ if (! m_symbol) m_symbol = AutoLoadLookup<Type> (m_library, m_symname, (Type *) 0); }

/** Return true if the auto-load entity is available: the library
    can be loaded and the symbol is found in the library.  */
template <class Type>
inline bool
AutoLoad<Type>::exists (void)
{ try { loadme (); } catch (AutoLoadError &) {} return m_symbol; }

/** Return a reference to the the auto-loaded entity.  Loads the
    library and resolves the symbol; if either operation fails,
    throws an #AutoLoadError.  */
template <class Type>
inline Type &
AutoLoad<Type>::operator* (void)
{ loadme (); return *m_symbol; }

/** Return a pointer to the the auto-loaded entity.  Loads the
    library and resolves the symbol; if either operation fails,
    throws an #AutoLoadError.  */
template <class Type>
inline Type *
AutoLoad<Type>::operator-> (void)
{ loadme (); return m_symbol; }

} // namespace lat
#endif // CLASSLIB_AUTO_LOAD_H
