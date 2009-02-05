#ifndef CLASSLIB_SEARCH_PATH_H
# define CLASSLIB_SEARCH_PATH_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/iobase/Filename.h"
# include <string>
# include <list>
# undef SearchPath // Ugh, defined by windows to SearchPath[UA]

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SearchPath;
class logstream;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

logstream &operator<< (logstream &out, const SearchPath &path);

//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Directory search path for looking up files.  */
class SearchPath
{
    typedef std::list<Filename>			DirList;
public:
    typedef DirList::iterator			iterator;
    typedef DirList::const_iterator		const_iterator;
    typedef DirList::reverse_iterator		reverse_iterator;
    typedef DirList::const_reverse_iterator	const_reverse_iterator;

    typedef DirList::size_type			size_type;
    typedef DirList::reference			reference;
    typedef DirList::const_reference		const_reference;

    SearchPath (void);
    explicit SearchPath (const std::string &path);
# if HAVE_MEMBER_TEMPLATES
    template <class I> SearchPath (I first, I last);
# else
    SearchPath (const_iterator first, const_iterator last);
# endif
    // default copy constructor
    // default destructor
    // default assignment operator

    // general queries
    bool			operator== (const SearchPath &x);
    bool			empty (void) const;
    size_type			size (void) const;

    // member access
    iterator			begin (void);
    const_iterator		begin (void) const;
    iterator			end (void);
    const_iterator		end (void) const;

    reverse_iterator		rbegin (void);
    const_reverse_iterator	rbegin (void) const;
    reverse_iterator		rend (void);
    const_reverse_iterator	rend (void) const;

    reference			front (void);
    const_reference		front (void) const;
    reference			back (void);
    const_reference		back (void) const;

    // alterations
    void			swap (SearchPath &x);

    void			push_front (const Filename &dir);
    void			push_back (const Filename &dir);
    iterator			insert (iterator pos, const Filename &dir);
    void			insert (iterator pos, const_iterator first,
					const_iterator last);

    void			pop_front (void);
    void			pop_back (void);
    iterator			erase (iterator pos);
    iterator			erase (iterator first, iterator last);
    void			clear (void);

    void			splice (iterator pos, SearchPath &x);
    void			splice (iterator pos, SearchPath &x,
					iterator i);
    void			splice (iterator pos, SearchPath &x,
					iterator first, iterator last);

    void			remove (const Filename &dir);
    void			reverse (void);

    // file search services (FIXME: should these accept mode to
    // describe what one should search for: existence, read, write,
    // execute, combinations?)
    Filename			expand (const Filename &name) const;
    const_iterator		search (const Filename &name) const;
    const_iterator		search (const_iterator from,
					const Filename &name) const;

    // system information
    static const char *		separator (void);

private:
    DirList			m_list;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Construct an empty search path. */
inline
SearchPath::SearchPath (void)
{}

# if HAVE_MEMBER_TEMPLATES
/** Construct a path from an iterator range.  */
template <class I> inline
SearchPath::SearchPath (I first, I last)
    : m_list (first, last)
{}
# else // ! HAVE_MEMBER_TEMPLATES
/** Construct a path from an iterator range.  */
inline
SearchPath::SearchPath (const_iterator first, const_iterator last)
    : m_list (first, last)
{}
# endif // HAVE_MEMBER_TEMPLATES

/** Compare two search paths.  Returns @c true if both paths contain
    the same list of file names: the same names in the same order, and
    with equally long lists of names.  */
inline bool
SearchPath::operator== (const SearchPath &x)
{ return m_list == x.m_list; }

/** Return true if the search path is empty.  */
inline bool
SearchPath::empty (void) const
{ return m_list.empty (); }

/** Return the number of directories in this search path.  */
inline SearchPath::size_type
SearchPath::size (void) const
{ return m_list.size (); }

/** Return an iterator to beginning of the search path's list of directories.  */
inline SearchPath::iterator
SearchPath::begin (void)
{ return m_list.begin (); }

/** Return an iterator to beginning of the search path's list of directories.  */
inline SearchPath::const_iterator
SearchPath::begin (void) const
{ return m_list.begin (); }

/** Return an iterator to end of the search path's list of directories.  */
inline SearchPath::iterator
SearchPath::end (void)
{ return m_list.end (); }

/** Return an iterator to end of the search path's list of directories.  */
inline SearchPath::const_iterator
SearchPath::end (void) const
{ return m_list.end (); }

/** Return a reverse iterator to the tail end of the search path's list of directories.  */
inline SearchPath::reverse_iterator
SearchPath::rbegin (void)
{ return m_list.rbegin (); }

/** Return a reverse iterator to the tail end of the search path's list of directories.  */
inline SearchPath::const_reverse_iterator
SearchPath::rbegin (void) const
{ return m_list.rbegin (); }

/** Return a reverse iterator to the front end of the search path's list of directories.  */
inline SearchPath::reverse_iterator
SearchPath::rend (void)
{ return m_list.rend (); }

/** Return a reverse iterator to the front end of the search path's list of directories.  */
inline SearchPath::const_reverse_iterator
SearchPath::rend (void) const
{ return m_list.rend (); }

/** Return the first directory in the path.  You may call this method
    only if the list is non-empty.  */
inline SearchPath::reference
SearchPath::front (void)
{ return m_list.front (); }

/** Return the first directory in the path.  You may call this method
    only if the list is non-empty.  */
inline SearchPath::const_reference
SearchPath::front (void) const
{ return m_list.front (); }

/** Return the last directory in the path.  You may call this method
    only if the list is non-empty.  */
inline SearchPath::reference
SearchPath::back (void)
{ return m_list.back (); }

/** Return the last directory in the path.  You may call this method
    only if the list is non-empty.  */
inline SearchPath::const_reference
SearchPath::back (void) const
{ return m_list.back (); }

/** Swap this directory lists of this search path and @a x. */
inline void
SearchPath::swap (SearchPath &x)
{ m_list.swap (x.m_list); }

/** Add a directory @a dir to the front of the search path. */
inline void
SearchPath::push_front (const Filename &dir)
{ m_list.push_front (dir); }

/** Add a directory @a dir to the back of the search path. */
inline void
SearchPath::push_back (const Filename &dir)
{ m_list.push_back (dir); }

/** Insert a directory @a dir to the search path at position @a pos.
    Inserting at @a pos == #end() is identical to calling #push_back().  */
inline SearchPath::iterator
SearchPath::insert (iterator pos, const Filename &dir)
{ return m_list.insert (pos, dir); }

/** Insert a range of directories from @a first to @a last at position
    @a pos.  Inserting at @a pos == #end() is identical to calling
    #push_back() for each of the directories in the given range.  */
inline void
SearchPath::insert (iterator pos, const_iterator first, const_iterator last)
{ m_list.insert (pos, first, last); }

/** Remove the first directory in the search path.  The path must not
    be empty.  */
inline void
SearchPath::pop_front (void)
{ m_list.pop_front (); }

/** Remove the last directory in the search path.  The path must not
    be empty.  */
inline void
SearchPath::pop_back (void)
{ m_list.pop_back (); }

/** Remove a directory at position @a pos from the search path.  The
    position must be greater than #begin() and less than #end().  */
inline SearchPath::iterator
SearchPath::erase (iterator pos)
{ return m_list.erase (pos); }

/** Remove a range of directories from @a first to @a last from the
    search path.  The range may be empty, and may include #end().  */
inline SearchPath::iterator
SearchPath::erase (iterator first, iterator last)
{ return m_list.erase (first, last); }

/** Remove all the directories from the path. */
inline void
SearchPath::clear (void)
{ m_list.clear (); }

/** Transfer the contents of the search path @a x into this, inserting
    the directories into this path in front of the position @a pos.  */
inline void
SearchPath::splice (iterator pos, SearchPath &x)
{ m_list.splice (pos, x.m_list); }

/** Transfer the contents of the search path @a x into this, inserting
    the directories into this path in front of the position @a pos.
    Take directories from @a x starting from position @a i; directories
    before @a i are not transferred.  */
inline void
SearchPath::splice (iterator pos, SearchPath &x, iterator i)
{ m_list.splice (pos, x.m_list, i); }

/** Transfer the contents of the search path @a x into this, inserting
    the directories into this path in front of the position @a pos.
    Take directories from @a x starting from the range @a first to
    @a last (but not including @a last); directories outside the range
    are not transferred.  */
inline void
SearchPath::splice (iterator pos, SearchPath &x, iterator first, iterator last)
{ m_list.splice (pos, x.m_list, first, last); }

/** Remove from this search path all directories equal to @a dir.  */
inline void
SearchPath::remove (const Filename &dir)
{ m_list.remove (dir); }

/** Reverse the search path, making it go from front to back.  */
inline void
SearchPath::reverse (void)
{ m_list.reverse (); }

/** Search for a file @a name in the search path.  The @a name must
    be relative, non-empty filename, but may include directory
    components.  The method finds the first directory in which the
    specified file exists and returns an iterator to the directory.
    The only check that is made is that the filename exists, no check
    is made to ensure it is accessible, nor is the type checked -- so
    the returned name may well be a directory instead of a file.  */
inline SearchPath::const_iterator
SearchPath::search (const Filename &name) const
{ return search (begin (), name); }

} // namespace lat
#endif // CLASSLIB_SEARCH_PATH_H
