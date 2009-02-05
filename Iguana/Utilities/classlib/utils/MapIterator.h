#ifndef CLASSLIB_MAP_ITERATOR_H
# define CLASSLIB_MAP_ITERATOR_H

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

/** An iterator adaptor to extract the value part from the (key,
    value) pairs returned by the @c std::map iterators.  */
template <class M> class MapValueIterator
{
public:
    // typedef typename iterator::iterator_category	iterator_category;
    // typedef typename iterator::difference_type	difference_type;
    typedef std::bidirectional_iterator_tag		iterator_category;
    typedef typename M::const_iterator			base_iterator;
    typedef typename M::mapped_type			value_type;
    typedef typename M::difference_type			difference_type;
    typedef const value_type &				reference;
    typedef const value_type *				pointer;

    MapValueIterator (void);
    MapValueIterator (base_iterator i);
    // default copy constructor
    // default destructor
    // default assignment operator

    bool		operator== (const MapValueIterator &x) const;
    bool		operator!= (const MapValueIterator &x) const;

    reference		operator* (void) const;
    pointer		operator-> (void) const;

    MapValueIterator &	operator++ (void);
    MapValueIterator	operator++ (int);
    MapValueIterator &	operator-- (void);
    MapValueIterator	operator-- (int);

private:
    base_iterator	m_iterator;
};

/** An iterator adaptor to extract the value part from the (key,
    value) pairs returned by the @c std::map iterators.  */
template <class M> class MapKeyIterator
{
public:
    // typedef typename iterator::iterator_category	iterator_category;
    // typedef typename iterator::difference_type	difference_type;
    typedef std::bidirectional_iterator_tag		iterator_category;
    typedef typename M::const_iterator			base_iterator;
    typedef typename M::key_type			value_type;
    typedef typename M::difference_type			difference_type;
    typedef const value_type &				reference;
    typedef const value_type *				pointer;

    MapKeyIterator (void);
    MapKeyIterator (base_iterator i);
    // default copy constructor
    // default destructor
    // default assignment operator

    bool		operator== (const MapKeyIterator &x) const;
    bool		operator!= (const MapKeyIterator &x) const;

    reference		operator* (void) const;
    pointer		operator-> (void) const;

    MapKeyIterator &	operator++ (void);
    MapKeyIterator	operator++ (int);
    MapKeyIterator &	operator-- (void);
    MapKeyIterator	operator-- (int);

private:
    base_iterator	m_iterator;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

template <class M> inline
MapValueIterator<M>::MapValueIterator (void)
{}

template <class M> inline
MapValueIterator<M>::MapValueIterator (base_iterator i)
    : m_iterator (i)
{}

template <class M> inline bool
MapValueIterator<M>::operator== (const MapValueIterator &x) const
{ return m_iterator == x.m_iterator; }

template <class M> inline bool
MapValueIterator<M>::operator!= (const MapValueIterator &x) const
{ return m_iterator != x.m_iterator; }

template <class M> inline typename MapValueIterator<M>::reference
MapValueIterator<M>::operator* (void) const
{ return m_iterator->second; }

template <class M> inline typename MapValueIterator<M>::pointer
MapValueIterator<M>::operator-> (void) const
{ return &m_iterator->second; }

template <class M> inline MapValueIterator<M> &
MapValueIterator<M>::operator++ (void)
{ ++m_iterator; return *this; }

template <class M> inline MapValueIterator<M>
MapValueIterator<M>::operator++ (int)
{ return MapValueIterator (m_iterator++); }

template <class M> inline MapValueIterator<M> &
MapValueIterator<M>::operator-- (void)
{ --m_iterator; return *this; }

template <class M> inline MapValueIterator<M>
MapValueIterator<M>::operator-- (int)
{ return MapValueIterator (m_iterator--); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class M> inline
MapKeyIterator<M>::MapKeyIterator (void)
{}

template <class M> inline
MapKeyIterator<M>::MapKeyIterator (base_iterator i)
    : m_iterator (i)
{}

template <class M> inline bool
MapKeyIterator<M>::operator== (const MapKeyIterator &x) const
{ return m_iterator == x.m_iterator; }

template <class M> inline bool
MapKeyIterator<M>::operator!= (const MapKeyIterator &x) const
{ return m_iterator != x.m_iterator; }

template <class M> inline typename MapKeyIterator<M>::reference
MapKeyIterator<M>::operator* (void) const
{ return m_iterator->first; }

template <class M> inline typename MapKeyIterator<M>::pointer
MapKeyIterator<M>::operator-> (void) const
{ return &m_iterator->first; }

template <class M> inline MapKeyIterator<M> &
MapKeyIterator<M>::operator++ (void)
{ ++m_iterator; return *this; }

template <class M> inline MapKeyIterator<M>
MapKeyIterator<M>::operator++ (int)
{ return MapKeyIterator (m_iterator++); }

template <class M> inline MapKeyIterator<M> &
MapKeyIterator<M>::operator-- (void)
{ --m_iterator; return *this; }

template <class M> inline MapKeyIterator<M>
MapKeyIterator<M>::operator-- (int)
{ return MapKeyIterator (m_iterator--); }

} // namespace lat
#endif // CLASSLIB_MAP_ITERATOR_H
