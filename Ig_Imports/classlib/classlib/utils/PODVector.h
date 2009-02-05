#ifndef CLASSLIB_PODVECTOR_H
# define CLASSLIB_PODVECTOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <iterator>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Container like @c std::vector except that its representation #Data
    is guaranteed to be a structure that can be stored and initialised
    as a part of another POD structure.  No operation on @c T may
    throw.  Full erase guarantees complete memory release.  Since
    #Data is a POD structure, all methods are outside it in the shell
    class and have to be passed a #Data reference. */
template <class T>
struct PODVector
{
    struct Data
    {
	T *begin;
	T *end;
	T *limit;
    };

    typedef T				value_type;
    typedef value_type			*pointer;
    typedef const value_type		*const_pointer;
    typedef value_type			*iterator;
    typedef const value_type		*const_iterator;
    typedef value_type			&reference;
    typedef const value_type		&const_reference;
    typedef STDC::size_t		size_type;
    typedef STDC::ptrdiff_t		difference_type;

    typedef
    REVERSE_ITERATOR(reverse_iterator,
		     random_access_iterator_tag,
		     iterator,
		     value_type,
		     reference,
		     pointer,
		     difference_type)	reverse_iterator;
    typedef
    REVERSE_ITERATOR(reverse_iterator,
		     random_access_iterator_tag,
		     const_iterator,
		     value_type,
		     const_reference,
		     const_pointer,
		     difference_type)	const_reverse_iterator;

    static iterator			begin (Data &vector);
    static const_iterator		begin (const Data &vector);
    static iterator			end (Data &vector);
    static const_iterator		end (const Data &vector);

    static reverse_iterator		rbegin (Data &vector);
    static const_reverse_iterator	rbegin (const Data &vector);
    static reverse_iterator		rend (Data &vector);
    static const_reverse_iterator	rend (const Data &vector);

    static size_type			size (const Data &vector);
    static size_type			max_size (const Data &vector);
    static size_type			capacity (const Data &vector);
    static bool				empty (const Data &vector);

    static reference			front (Data &vector);
    static const_reference		front (const Data &vector);
    static reference			back (Data &vector);
    static const_reference		back (const Data &vector);

    static void				swap (Data &vector, Data &other);

    static void				reserve (Data &vector, size_type n);
    static void				push_back (Data &vector, const T &x);
    static iterator			insert (Data &vector,
						iterator pos, const T &x);
    static iterator			insert (Data &vector,
						iterator pos);
# if HAVE_MEMBER_TEMPLATES
    // template <class InIter>
    // static void				insert (Data &vector, iterator pos,
    // 						InIter first, InIter last);
# else
    // static void				insert (Data &vector, iterator pos,
    // 						const_iterator first,
    // 						const_iterator last);
# endif
    static void				insert (Data &vector, iterator pos,
						size_type n, const T &x);
    static void				insert (Data &vector, iterator pos,
						int n, const T &x);
    static void				insert (Data &vector, iterator pos,
						long n, const T &x);

    static void				pop_back (Data &vector);
    static iterator			erase (Data &vector, iterator pos);
    static iterator			erase (Data &vector, iterator first,
					       iterator last);
    static void				resize (Data &vector,
						size_type new_size,
						const T &x);
    static void				resize (Data &vector,
						size_type new_size);
    static void				clear (Data &vector);

private:
    static void				insert_grow (Data &vector,
						     iterator pos, const T &x);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

template <class T>
inline typename PODVector<T>::iterator
PODVector<T>::begin (Data &vector)
{ return vector.begin; }

template <class T>
inline typename PODVector<T>::const_iterator
PODVector<T>::begin (const Data &vector)
{ return vector.begin; }

template <class T>
inline typename PODVector<T>::iterator
PODVector<T>::end (Data &vector)
{ return vector.end; }

template <class T>
inline typename PODVector<T>::const_iterator
PODVector<T>::end (const Data &vector)
{ return vector.end; }

//////////////////////////////////////////////////////////////////////
template <class T>
inline typename PODVector<T>::reverse_iterator
PODVector<T>::rbegin (Data &vector)
{ return vector.begin; }

template <class T>
inline typename PODVector<T>::const_reverse_iterator
PODVector<T>::rbegin (const Data &vector)
{ return vector.begin; }

template <class T>
inline typename PODVector<T>::reverse_iterator
PODVector<T>::rend (Data &vector)
{ return vector.end; }

template <class T>
inline typename PODVector<T>::const_reverse_iterator
PODVector<T>::rend (const Data &vector)
{ return vector.end; }

//////////////////////////////////////////////////////////////////////
template <class T>
inline typename PODVector<T>::size_type
PODVector<T>::size (const Data &vector)
{ return vector.begin ? vector.end - vector.begin : 0; }

template <class T>
inline typename PODVector<T>::size_type
PODVector<T>::max_size (const Data & /* vector */)
{ return size_type(-1) / sizeof (T); }

template <class T>
inline typename PODVector<T>::size_type
PODVector<T>::capacity (const Data &vector)
{ return vector.begin ? vector.limit - vector.begin : 0; }

template <class T>
inline bool
PODVector<T>::empty (const Data &vector)
{ return vector.begin == vector.end; }

//////////////////////////////////////////////////////////////////////
template <class T>
inline typename PODVector<T>::reference
PODVector<T>::front (Data &vector)
{ return *vector.begin; }

template <class T>
inline typename PODVector<T>::const_reference
PODVector<T>::front (const Data &vector)
{ return *vector.begin; }

template <class T>
inline typename PODVector<T>::reference
PODVector<T>::back (Data &vector)
{ return *(vector.end-1); }

template <class T>
inline typename PODVector<T>::const_reference
PODVector<T>::back (const Data &vector)
{ return *(vector.end-1); }

//////////////////////////////////////////////////////////////////////
template <class T>
inline void
PODVector<T>::swap (Data &vector, Data &other)
{
    Data tmp = vector;
    vector = other;
    other = tmp;
}

//////////////////////////////////////////////////////////////////////
template <class T>
inline void
PODVector<T>::reserve (Data &vector, size_type new_size)
{
    if (capacity (vector) < new_size)
    {
	size_type	old_size = size (vector);
	iterator	begin = static_cast<iterator> (::operator new (new_size * sizeof (T)));
  
	std::uninitialized_copy (vector.begin, vector.end, begin);
	for (iterator first = vector.begin, last = vector.end;
	     first != last;
	     ++first)
	    first->T::~T ();
	::operator delete (vector.begin);

	vector.begin = begin;
	vector.end   = begin + old_size;
	vector.limit = begin + new_size;
    }
}

template <class T>
inline void
PODVector<T>::insert_grow (Data &vector, iterator pos, const T &x)
{
    if (vector.end != vector.limit)
    {
	new (vector.end) T (*(vector.end-1));
	++vector.end;
	std::copy_backward (pos, vector.end-2, vector.end-1);
	*pos = x;
    }
    else
    {
	size_type	old_size = size (vector);
	size_type	new_size = old_size ? 2*old_size : 1;
	iterator	begin;
	iterator	end;

	begin = static_cast<iterator> (::operator new (new_size * sizeof (T)));
	end = std::uninitialized_copy (vector.begin, pos, begin);
	new (end) T (x);
	++end;
	end = std::uninitialized_copy (pos, vector.end, end);

	for (iterator first = vector.begin, last = vector.end;
	     first != last;
	     ++first)
	    first->T::~T ();
	::operator delete (vector.begin);

	vector.begin = begin;
	vector.end   = end;
	vector.limit = begin + new_size;
    }
}

template <class T>
inline void
PODVector<T>::push_back (Data &vector, const T &value)
{
    if (vector.end != vector.limit)
    {
	new (vector.end) T (value);
	++vector.end;
    }
    else
	insert_grow (vector, vector.end, value);
}

template <class T>
inline typename PODVector<T>::iterator
PODVector<T>::insert (Data &vector, iterator pos, const T &x)
{
    size_type index = pos - begin (vector);
    if (vector.end != vector.limit && pos != vector.end)
    {
	new (vector.end) T (x);
	++vector.end;
    }
    else
	insert_grow (vector, pos, x);

    return vector.begin + index;
}

template <class T>
inline typename PODVector<T>::iterator
PODVector<T>::insert (Data &vector, iterator pos)
{ return insert (vector, pos, T()); }

# if HAVE_MEMBER_TEMPLATES
// template <class T>
// template <class InIter>
// inline void
// PODVector<T>::insert (Data &vector, iterator pos, InIter first, InIter last)
// { FIXME; }
# else
// template <class T>
// inline void
// PODVector<T>::insert (Data &vector, iterator pos, const_iterator first,
//			 const_iterator last)
// { FIXME; }
# endif

// template <class T>
// inline typename PODVector<T>::iterator
// PODVector<T>::insert (Data &vector, iterator pos, size_type n, const T &x)
// { FIXME: }

template <class T>
inline void
PODVector<T>::insert (Data &vector, iterator pos, int n, const T &x)
{ insert (vector, pos, size_type (n), x); }

template <class T>
inline void
PODVector<T>::insert (Data &vector, iterator pos, long n, const T &x)
{ insert (vector, pos, size_type (n), x); }

//////////////////////////////////////////////////////////////////////
template <class T>
inline void
PODVector<T>::pop_back (Data &vector)
{ --vector.end; vector.end->T::~T (); }

template <class T>
inline typename PODVector<T>::iterator
PODVector<T>::erase (Data &vector, iterator pos)
{
    if (pos+1 != vector.end)
	std::copy (pos+1, vector.end, pos);
    --vector.end;
    vector.end->T::~T ();

    if (vector.begin == vector.end)
    {
	::operator delete (vector.begin);
	vector.begin = vector.end = vector.limit = 0;
	return 0;
    }
    else
	return pos;
}

template <class T>
inline typename PODVector<T>::iterator
PODVector<T>::erase (Data &vector, iterator first, iterator last)
{
    for (first = std::copy (last, vector.end, first);
	 first != vector.end;
	 ++first)
	first->T::~T ();
    vector.end -= last - first;
    return first;
}

template <class T>
inline void
PODVector<T>::resize (Data &vector, size_type new_size, const T &x)
{
    if (new_size < size ())
	erase (vector.begin + new_size, vector.end);
    else
	insert (vector.end, new_size-size(), x);
}

template <class T>
inline void
PODVector<T>::resize (Data &vector, size_type new_size)
{ resize (vector, new_size, T()); }

template <class T>
inline void
PODVector<T>::clear (Data &vector)
{ erase (vector.begin, vector.end); }

} // namespace lat
#endif // CLASSLIB_PODVECTOR_H
